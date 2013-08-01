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
		m_sessionConnection("DBusTestRunner-session"), m_systemConnection(
				"DBusTestRunner-system") {

	// session bus setup

	m_sessionDBus.setProcessChannelMode(QProcess::MergedChannels);
	m_sessionDBus.start("dbus-daemon",
			QStringList() << "--config-file" << DBUS_SESSION_CONFIG_FILE
					<< "--print-address");
	Q_ASSERT(m_sessionDBus.waitForStarted());

	m_sessionDBus.waitForReadyRead();
	m_sessionBus = m_sessionDBus.readAll().trimmed();

	qputenv("DBUS_SESSION_BUS_ADDRESS", m_sessionBus.toUtf8());
	qDebug() << "DBUS_SESSION_BUS_ADDRESS =" << m_sessionBus;

	m_sessionConnection = QDBusConnection::connectToBus(m_sessionBus,
			m_sessionBus);

	// system bus setup

	m_systemDBus.setProcessChannelMode(QProcess::MergedChannels);
	m_systemDBus.start("dbus-daemon",
			QStringList() << "--config-file" << DBUS_SYSTEM_CONFIG_FILE
					<< "--print-address");
	Q_ASSERT(m_systemDBus.waitForStarted());

	m_systemDBus.waitForReadyRead();
	m_systemBus = m_systemDBus.readAll().trimmed();

	qputenv("DBUS_SYSTEM_BUS_ADDRESS", m_systemBus.toUtf8());
	qDebug() << "DBUS_SYSTEM_BUS_ADDRESS =" << m_systemBus;

	m_systemConnection = QDBusConnection::connectToBus(m_systemBus,
			m_systemBus);
}

DBusTestRunner::~DBusTestRunner() {
	m_services.clear();

	m_sessionDBus.terminate();
	Q_ASSERT(m_sessionDBus.waitForFinished());

	m_systemDBus.terminate();
	Q_ASSERT(m_systemDBus.waitForFinished());
}

void DBusTestRunner::registerService(DBusServicePtr service) {
	m_services.push_back(service);
}

void DBusTestRunner::startServices() {
	for (DBusServicePtr service : m_services) {
		switch (service->busType()) {
		case QDBusConnection::SystemBus:
			service->start(m_systemConnection);
			break;
		case QDBusConnection::SessionBus:
			service->start(m_sessionConnection);
			break;
		case QDBusConnection::ActivationBus:
			qWarning() << "Unknown bus type";
			break;
		}
	}
}

const QDBusConnection & DBusTestRunner::sessionConnection() const {
	return m_sessionConnection;
}

const QString & DBusTestRunner::sessionBus() const {
	return m_sessionBus;
}

const QDBusConnection & DBusTestRunner::systemConnection() const {
	return m_systemConnection;
}

const QString & DBusTestRunner::systemBus() const {
	return m_systemBus;
}
