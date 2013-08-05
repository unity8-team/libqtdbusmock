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

#include <libqtdbusmock/Method.h>

using namespace QtDBusMock;

Method::Method() {
}

Method::Method(const Method &other) :
		m_name(other.m_name), m_inSig(other.m_inSig), m_outSig(other.m_outSig), m_code(
				other.m_code) {
}

Method& Method::operator=(const Method &other) {
	m_name = other.m_name;
	m_inSig = other.m_inSig;
	m_outSig = other.m_outSig;
	m_code = other.m_code;

	return *this;
}

Method::~Method() {
}

void Method::registerMetaType() {
	qRegisterMetaType<Method>("QtDBusMock::Method");

	qDBusRegisterMetaType<Method>();
	qDBusRegisterMetaType<QList<Method>>();
}

const QString & Method::name() const {
	return m_name;
}

void Method::setName(const QString &name) {
	m_name = name;
}
const QString & Method::inSig() const {
	return m_inSig;
}

void Method::setInSig(const QString &inSig) {
	m_inSig = inSig;
}
const QString & Method::outSig() const {
	return m_outSig;
}

void Method::setOutSig(const QString &outSig) {
	m_outSig = outSig;
}
const QString & Method::code() const {
	return m_code;
}

void Method::setCode(const QString &code) {
	m_code = code;
}

QDBusArgument &operator<<(QDBusArgument &argument, const Method& method) {

	argument.beginStructure();
	argument << method.name() << method.inSig() << method.outSig()
			<< method.code();
	argument.endStructure();

	return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, Method &method) {
	QString name;
	QString inSig;
	QString outSig;
	QString code;

	argument.beginStructure();
	argument >> name >> inSig >> outSig >> code;
	argument.endStructure();

	method.setName(name);
	method.setInSig(inSig);
	method.setOutSig(outSig);
	method.setCode(code);

	return argument;
}
