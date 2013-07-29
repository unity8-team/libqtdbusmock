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

#include <libqtdbustest/DBusTestRunner.h>

using namespace QtDBusTest;

DBusTestRunner::DBusTestRunner() :
		m_connection("DBusTestRunner") {

	m_dbus.setProcessChannelMode(QProcess::MergedChannels);
	m_dbus.start("dbus-daemon",
			QStringList() << "--config-file" << DBUS_SYSTEM_CONFIG_FILE
					<< "--print-address");
	Q_ASSERT(m_dbus.waitForStarted());

	m_dbus.waitForReadyRead();
	QByteArray readAll(m_dbus.readAll());
	m_bus = readAll.trimmed();

	qputenv("DBUS_SYSTEM_BUS_ADDRESS", m_bus.toUtf8());

	m_connection = QDBusConnection::connectToBus(m_bus, m_bus);
}

DBusTestRunner::~DBusTestRunner() {
	m_services.clear();

	m_dbus.terminate();
	Q_ASSERT(m_dbus.waitForFinished());
}

void DBusTestRunner::registerService(DBusServicePtr service) {
	m_services.insert(service->interface(), service);
}

void DBusTestRunner::startServices() {
	for (DBusServicePtr service : m_services.values()) {
		service->start(m_connection, m_bus);
	}
}

const QDBusConnection & DBusTestRunner::connection() const {
	return m_connection;
}

const QString & DBusTestRunner::bus() const {
	return m_bus;
}

