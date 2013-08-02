/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
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
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QSignalSpy>
#include <QTest>
#include <QVariant>
#include <QDBusVariant>
#include <QDBusServiceWatcher>
#include "qmenumodel/qdbusmenumodel.h"

using namespace std;
using namespace testing;
using namespace QtDBusTest;
using namespace QtDBusMock;

namespace {

class TestNetworkPrompt: public Test {
protected:
	TestNetworkPrompt() :
			dbusMock(dbusTestRunner), menuFactory(new MenuFactory()) {

		dbusMock.registerNetworkManager();
		dbusTestRunner.startServices();

		menuPrinter.setProcessChannelMode(QProcess::MergedChannels);
		menuPrinter.start(MENU_PRINTER);
		menuPrinter.waitForStarted();
	}

	virtual ~TestNetworkPrompt() {
		menuPrinter.terminate();
		menuPrinter.waitForFinished();
	}

	DBusTestRunner dbusTestRunner;

	DBusMock dbusMock;

	QProcess networkManagerService;

	shared_ptr<MenuFactory> menuFactory;

	QProcess menuPrinter;
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
	networkManager.AddWiFiConnection(
			"/org/freedesktop/NetworkManager/Devices/device", "connection",
			"the ssid", "wpa-psk").waitForFinished();

	NetworkPrompt networkPrompt(dbusTestRunner.sessionConnection(),
			dbusTestRunner.systemConnection(), menuFactory);
	networkPrompt.check();

	OrgFreedesktopDBusMockInterface &networkManagerMock(
			dbusMock.mockInterface(NM_DBUS_SERVICE, NM_DBUS_PATH,
					NM_DBUS_INTERFACE, QDBusConnection::SystemBus));
	QList<MethodCall> methodCalls(
			networkManagerMock.GetMethodCalls("ActivateConnection"));

	QFile expected(EXPECTED_MENU_WITH_EXISTING_CONNECTION);
	expected.open(QIODevice::ReadOnly);
	EXPECT_EQ(string(expected.readAll().data()),
			string(menuPrinter.readAll().data()));

//	ASSERT_EQ(1, methodCalls.size());
//	const QVariantList &args(methodCalls.first().args());
//	ASSERT_EQ(3, args.size());
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
			"/org/freedesktop/NetworkManager/Devices/device", "ap1", "ssid1",
			"11:22:33:44:55:66", 1, 0, 0, 's', 0).waitForFinished();
	networkManager.AddAccessPoint(
			"/org/freedesktop/NetworkManager/Devices/device", "ap2", "ssid2",
			"22:33:44:55:66:77", 0, 0, 0, 's', 0).waitForFinished();

	NetworkPrompt networkPrompt(dbusTestRunner.sessionConnection(),
			dbusTestRunner.systemConnection(), menuFactory);
	networkPrompt.check();

	OrgFreedesktopDBusMockInterface &networkManagerMock(
			dbusMock.mockInterface(NM_DBUS_SERVICE, NM_DBUS_PATH,
					NM_DBUS_INTERFACE, QDBusConnection::SystemBus));
	QList<MethodCall> methodCalls(
			networkManagerMock.GetMethodCalls("AddAndActivateConnection"));

	QFile expected(EXPECTED_MENU_WITHOUT_EXISTING_CONNECTION);
	expected.open(QIODevice::ReadOnly);
	EXPECT_EQ(string(expected.readAll().data()),
			string(menuPrinter.readAll().data()));

//	ASSERT_EQ(1, methodCalls.size());
//	ASSERT_EQ(3, methodCalls.first().args().size());

}

} // namespace
