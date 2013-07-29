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

void DBusMock::registerNetworkManager() {
	m_testRunner.registerService(
			DBusServicePtr(
					new QProcessDBusService(NM_DBUS_INTERFACE,
							QDBusConnection::SystemBus, "python3",
							QStringList() << "-m" << "dbusmock" << "--template"
									<< "networkmanager")));
}

NetworkManagerMockInterface & DBusMock::networkManagerMock() {
	if (m_networkManagerMock.isNull()) {
		m_networkManagerMock.reset(
				new NetworkManagerMockInterface(NM_DBUS_INTERFACE, NM_DBUS_PATH,
						m_testRunner.connection()));
	}
	return *m_networkManagerMock;
}
