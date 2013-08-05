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

#include <libqtdbusmock/NamedMethodCall.h>

using namespace QtDBusMock;

NamedMethodCall::NamedMethodCall() :
		m_timestamp(0) {
}

NamedMethodCall::NamedMethodCall(const NamedMethodCall &other) :
		m_timestamp(other.m_timestamp), m_methodName(other.m_methodName), m_args(
				other.m_args) {
}

NamedMethodCall& NamedMethodCall::operator=(const NamedMethodCall &other) {
	m_timestamp = other.m_timestamp;
	m_methodName = other.m_methodName;
	m_args = other.m_args;

	return *this;
}

NamedMethodCall::~NamedMethodCall() {
}

void NamedMethodCall::registerMetaType() {
	qRegisterMetaType<NamedMethodCall>("QtDBusMock::NamedMethodCall");

	qDBusRegisterMetaType<NamedMethodCall>();
	qDBusRegisterMetaType<QList<NamedMethodCall>>();
}

const QString & NamedMethodCall::methodName() const {
	return m_methodName;
}

const quint64 & NamedMethodCall::timestamp() const {
	return m_timestamp;
}

const QVariantList & NamedMethodCall::args() const {
	return m_args;
}

void NamedMethodCall::setMethodName(const QString &methodName) {
	m_methodName = methodName;
}

void NamedMethodCall::setTimestamp(quint64 timestamp) {
	m_timestamp = timestamp;
}

void NamedMethodCall::setArgs(const QVariantList &args) {
	m_args = args;
}

QDBusArgument &operator<<(QDBusArgument &argument,
		const NamedMethodCall& methodCall) {

	argument.beginStructure();
	argument << methodCall.timestamp() << methodCall.methodName()
			<< methodCall.args();
	argument.endStructure();

	return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument,
		NamedMethodCall &methodCall) {

	quint64 timestamp;
	QString methodName;
	QVariantList args;

	argument.beginStructure();
	argument >> timestamp >> methodName >> args;
	argument.endStructure();

	methodCall.setTimestamp(timestamp);
	methodCall.setMethodName(methodName);
	methodCall.setArgs(args);

	return argument;
}
