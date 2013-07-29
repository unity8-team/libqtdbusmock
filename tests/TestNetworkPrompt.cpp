/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Pete Woods <pete.woods@canonical.com>
 */

#include <NetworkPrompt.h>
#include <libqtdbustest/DBusTestRunner.h>
#include <libqtdbusmock/DBusMock.h>
#include <libqtdbusmock/MethodCall.h>
#include <libqtdbusmock/MockInterface.h>
#include <libqtdbusmock/PropertiesInterface.h>
#include <NetworkManager.h>
#include <stdexcept>
#include <gtest/gtest.h>
#include <QSignalSpy>
#include <QTest>
#include <QVariant>
#include <QDBusVariant>

using namespace std;
using namespace testing;
using namespace QtDBusTest;
using namespace QtDBusMock;

namespace {

class TestNetworkPrompt: public Test {
protected:
	TestNetworkPrompt() :
			dbusMock(dbusTestRunner) {

		dbusMock.registerNetworkManager();

		dbusMock.registerCustomMock("com.canonical.Unity.SystemDialog",
				"/com/canonical/Unity/SystemDialog",
				"com.canonical.Unity.SystemDialog",
				QDBusConnection::SessionBus);

		dbusTestRunner.startServices();
	}

	virtual ~TestNetworkPrompt() {
	}

	DBusTestRunner dbusTestRunner;

	DBusMock dbusMock;

	QProcess networkManagerService;

};

TEST_F(TestNetworkPrompt, ActivatesWithExistingConnection) {
	OrgFreedesktopDBusPropertiesInterface properties(NM_DBUS_INTERFACE,
			NM_DBUS_PATH, dbusTestRunner.systemConnection());
	properties.Set(NM_DBUS_INTERFACE, "WirelessEnabled", QVariant(true)).waitForFinished();
	properties.Set(NM_DBUS_INTERFACE, "State", QVariant(NM_STATE_DISCONNECTED)).waitForFinished();

	NetworkManagerMockInterface &networkManager(
			dbusMock.networkManagerInterface());
	networkManager.AddWiFiDevice("device", "eth1", NM_DEVICE_STATE_DISCONNECTED).waitForFinished();
	networkManager.AddAccessPoint(
			"/org/freedesktop/NetworkManager/Devices/device", "ap", "ssid",
			"11:22:33:44:55:66", 0, 0, 0, 's', 0).waitForFinished();
	networkManager.AddMockConnection(
			"/org/freedesktop/NetworkManager/Devices/device", "connection",
			"the ssid").waitForFinished();

	NetworkPrompt networkPrompt(dbusTestRunner.sessionConnection(),
			dbusTestRunner.systemConnection());
	networkPrompt.check();

	OrgFreedesktopDBusMockInterface &networkManagerMock(
			dbusMock.mockInterface(NM_DBUS_SERVICE, NM_DBUS_PATH,
					NM_DBUS_INTERFACE, QDBusConnection::SystemBus));
	QList<MethodCall> methodCalls(
			networkManagerMock.GetMethodCalls("ActivateConnection"));

	ASSERT_EQ(1, methodCalls.size());
	ASSERT_EQ(3, methodCalls.first().args().size());
}

TEST_F(TestNetworkPrompt, ActivatesWithoutExistingConnection) {
	OrgFreedesktopDBusPropertiesInterface properties(NM_DBUS_INTERFACE,
			NM_DBUS_PATH, dbusTestRunner.systemConnection());
	properties.Set(NM_DBUS_INTERFACE, "WirelessEnabled", QVariant(true)).waitForFinished();
	properties.Set(NM_DBUS_INTERFACE, "State", QVariant(NM_STATE_DISCONNECTED)).waitForFinished();

	NetworkManagerMockInterface &networkManager(
			dbusMock.networkManagerInterface());
	networkManager.AddWiFiDevice("device", "eth1", NM_DEVICE_STATE_DISCONNECTED).waitForFinished();
	networkManager.AddAccessPoint(
			"/org/freedesktop/NetworkManager/Devices/device", "ap", "ssid",
			"11:22:33:44:55:66", 0, 0, 0, 's', 0).waitForFinished();

	NetworkPrompt networkPrompt(dbusTestRunner.sessionConnection(),
			dbusTestRunner.systemConnection());
	networkPrompt.check();

	OrgFreedesktopDBusMockInterface &networkManagerMock(
			dbusMock.mockInterface(NM_DBUS_SERVICE, NM_DBUS_PATH,
					NM_DBUS_INTERFACE, QDBusConnection::SystemBus));
	QList<MethodCall> methodCalls(
			networkManagerMock.GetMethodCalls("AddAndActivateConnection"));

	ASSERT_EQ(1, methodCalls.size());
	ASSERT_EQ(3, methodCalls.first().args().size());
}

} // namespace
