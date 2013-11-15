/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of version 3 of the GNU Lesser General Public License as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Pete Woods <pete.woods@canonical.com>
 */

#include <libqtdbusmock/MethodCall.h>

using namespace QtDBusMock;

MethodCall::MethodCall() :
		m_timestamp(0) {
}

MethodCall::MethodCall(const MethodCall &other) :
		m_timestamp(other.m_timestamp), m_args(other.m_args) {
}

MethodCall& MethodCall::operator=(const MethodCall &other) {
	m_timestamp = other.m_timestamp;
	m_args = other.m_args;

	return *this;
}

MethodCall::~MethodCall() {
}

void MethodCall::registerMetaType() {
	qRegisterMetaType<MethodCall>("QtDBusMock::MethodCall");

	qDBusRegisterMetaType<MethodCall>();
	qDBusRegisterMetaType<QList<MethodCall>>();
}

const quint64 & MethodCall::timestamp() const {
	return m_timestamp;
}

const QVariantList & MethodCall::args() const {
	return m_args;
}

void MethodCall::setTimestamp(quint64 timestamp) {
	m_timestamp = timestamp;
}

void MethodCall::setArgs(const QVariantList &args) {
	m_args = args;
}

QDBusArgument &operator<<(QDBusArgument &argument,
		const MethodCall& methodCall) {

	argument.beginStructure();
	argument << methodCall.timestamp() << methodCall.args();
	argument.endStructure();

	return argument;
}

static void transform(QVariantList &list);

static void transform(QVariantMap &map);

static QVariant transform(const QDBusArgument & value) {
	switch (value.currentType()) {
	case QDBusArgument::ArrayType: {
		value.beginArray();
		QVariantList list = transform(value).toList();
		value.endArray();
		return list;
	}
	case QDBusArgument::MapType: {
		QVariantMap map;
		value >> map;
		transform(map);
		return map;
	}
	case QDBusArgument::StructureType: {
		value.beginStructure();
		QVariantList list;
		while (!value.atEnd()) {
			list << value.asVariant();
		}
		value.endStructure();
		return list;
		break;
	}
	default:
		qDebug() << "Unhandled type" << value.currentType()
				<< value.currentSignature();
	}
	return QVariant();
}

static void transform(QVariant &variant) {
	if (variant.canConvert<QVariantList>()) {
		QVariantList list = variant.toList();
		transform(list);
		variant = list;
	} else if (variant.canConvert<QDBusArgument>()) {
		QDBusArgument value(variant.value<QDBusArgument>());
		variant = transform(value);
	}
}

static void transform(QVariantMap &map) {
	for (auto it(map.begin()); it != map.end(); ++it) {
		transform(*it);
	}
}

static void transform(QVariantList &list) {
	for (auto it(list.begin()); it != list.end(); ++it) {
		transform(*it);
	}
}

const QDBusArgument &operator>>(const QDBusArgument &argument,
		MethodCall &methodCall) {

	quint64 timestamp;
	QVariantList args;

	argument.beginStructure();
	argument >> timestamp >> args;
	argument.endStructure();

	transform(args);

	methodCall.setTimestamp(timestamp);
	methodCall.setArgs(args);

	return argument;
}
