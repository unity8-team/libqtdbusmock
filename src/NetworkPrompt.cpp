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

#include <NetworkManagerInterface.h>
#include <NetworkManagerDeviceInterface.h>
#include <NetworkManagerDeviceWirelessInterface.h>
#include <NetworkPrompt.h>

#include <NetworkManager.h>

NetworkPrompt::NetworkPrompt(const QDBusConnection &connection, QObject *parent) :
		QObject(parent), m_connection(connection) {

	OrgFreedesktopNetworkManagerInterface networkManager(NM_DBUS_SERVICE,
			NM_DBUS_PATH, connection);

	// not on internet
	// and wi-fi is on
	// and networks are available
	// and "prompt when not connected" enabled
	// and at least two hours or one system sleep
	// and (if on the phone) you try to use the internet unsuccessfully or using cellular data

	// assuming we only get pinged when the prompt preference is enabled
	// and by someone trying to use the internet
	if (networkManager.wirelessEnabled()
			&& networkManager.state() == NM_STATE_DISCONNECTED) {
		qDebug()
				<< "it seems we're not connected to any networks, and wifi is enabled";

		QList<QDBusObjectPath> devicePaths(networkManager.GetDevices());

		for (const QDBusObjectPath &devicePath : devicePaths) {
			OrgFreedesktopNetworkManagerDeviceInterface device(NM_DBUS_SERVICE,
					devicePath.path(), connection);
			if (device.deviceType() == NM_DEVICE_TYPE_WIFI) {

				OrgFreedesktopNetworkManagerDeviceWirelessInterface wifiDevice(
						NM_DBUS_SERVICE, devicePath.path(), connection);

				QDBusPendingReply<void> reply(
						wifiDevice.RequestScan(QVariantMap()));
				QList<QDBusObjectPath> accessPointPaths(
						wifiDevice.GetAccessPoints());

				for (const QDBusObjectPath &accessPointPath : accessPointPaths) {
					qDebug() << "access point: " << accessPointPath.path();
					QList<QDBusObjectPath> connectionPaths(
							device.availableConnections());
					if (connectionPaths.empty()) {
						// if no existing connections, then add a new one
						QDBusObjectPath networkConnection(
								networkManager.AddAndActivateConnection(
										QVariantMap(), devicePath,
										accessPointPath));
					} else {
						// we already have a connection, so just activate it
						QDBusObjectPath networkConnection(
								networkManager.ActivateConnection(
										connectionPaths.first(), devicePath,
										accessPointPath));
					}
				}
			}
		}
	}
}

NetworkPrompt::~NetworkPrompt() {
}

