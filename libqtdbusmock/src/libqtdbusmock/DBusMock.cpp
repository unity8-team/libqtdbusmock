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

#include <libqtdbusmock/DBusMock.h>
#include <libqtdbustest/QProcessDBusService.h>

#include <NetworkManager.h>

using namespace QtDBusMock;
using namespace QtDBusTest;

DBusMock::DBusMock(DBusTestRunner &testRunner) :
		m_testRunner(testRunner) {
}

DBusMock::~DBusMock() {
}

void DBusMock::registerMetaTypes() {
	Method::registerMetaType();
	MethodCall::registerMetaType();
	NamedMethodCall::registerMetaType();
}

void DBusMock::registerNetworkManager() {
	m_testRunner.registerService(
			DBusServicePtr(
					new QProcessDBusService(NM_DBUS_INTERFACE,
							QDBusConnection::SystemBus, "python3",
							QStringList() << "-m" << "dbusmock" << "--template"
									<< "networkmanager")));
}

void DBusMock::registerCustomMock(const QString &name, const QString &path,
		const QString &interface, QDBusConnection::BusType busType) {
	m_testRunner.registerService(
			DBusServicePtr(
					new QProcessDBusService(NM_DBUS_INTERFACE, busType,
							"python3",
							QStringList() << "-m" << "dbusmock" << name << path
									<< interface)));
}

NetworkManagerMockInterface & DBusMock::networkManagerInterface() {
	if (m_networkManagerMock.isNull()) {
		m_networkManagerMock.reset(
				new NetworkManagerMockInterface(NM_DBUS_INTERFACE, NM_DBUS_PATH,
						m_testRunner.systemConnection()));
	}
	return *m_networkManagerMock;
}

OrgFreedesktopDBusMockInterface & DBusMock::mockInterface(const QString &name,
		const QString &path, const QString &interface,
		QDBusConnection::BusType busType) {
	auto it(m_mockInterfaces.find(name));
	if (it == m_mockInterfaces.end()) {
		switch (busType) {
		case QDBusConnection::SystemBus:
			it = m_mockInterfaces.insert(name,
					QSharedPointer<OrgFreedesktopDBusMockInterface>(
							new OrgFreedesktopDBusMockInterface(interface, path,
									m_testRunner.systemConnection())));
			break;
		case QDBusConnection::SessionBus:
			it = m_mockInterfaces.insert(name,
					QSharedPointer<OrgFreedesktopDBusMockInterface>(
							new OrgFreedesktopDBusMockInterface(interface, path,
									m_testRunner.sessionConnection())));
			break;
		case QDBusConnection::ActivationBus:
			qWarning() << "Unknown bus type";
			break;
		}
	}
	return **it;
}

