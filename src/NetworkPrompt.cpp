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

#include <NetworkManagerDeviceInterface.h>
#include <NetworkManagerDeviceWirelessInterface.h>
#include <NetworkPrompt.h>

#include <NetworkManager.h>

NetworkPrompt::NetworkPrompt(const QDBusConnection &sessionConnection,
		const QDBusConnection &systemConnection, QObject *parent) :
		QObject(parent), m_sessionConnection(sessionConnection), m_systemConnection(
				systemConnection), m_networkManager(NM_DBUS_SERVICE,
				NM_DBUS_PATH, m_systemConnection), m_systemDialog(
				"com.canonical.Unity.SystemDialog",
				"/com/canonical/Unity/SystemDialog", m_sessionConnection) {
}

NetworkPrompt::~NetworkPrompt() {
}

void NetworkPrompt::check() {
	// not on internet
	// and wi-fi is on
	// and networks are available
	// and "prompt when not connected" enabled
	// and at least two hours or one system sleep
	// and (if on the phone) you try to use the internet unsuccessfully or using cellular data

	// assuming we only get pinged when the prompt preference is enabled
	// and by someone trying to use the internet
	if (m_networkManager.wirelessEnabled()
			&& m_networkManager.state() == NM_STATE_DISCONNECTED) {
		qDebug()
				<< "it seems we're not connected to any networks, and wifi is enabled";

		QList<QDBusObjectPath> devicePaths(m_networkManager.GetDevices());

		for (const QDBusObjectPath &devicePath : devicePaths) {
			OrgFreedesktopNetworkManagerDeviceInterface device(NM_DBUS_SERVICE,
					devicePath.path(), m_systemConnection);
			if (device.deviceType() == NM_DEVICE_TYPE_WIFI) {

				OrgFreedesktopNetworkManagerDeviceWirelessInterface wifiDevice(
						NM_DBUS_SERVICE, devicePath.path(), m_systemConnection);

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
								m_networkManager.AddAndActivateConnection(
										QVariantMap(), devicePath,
										accessPointPath));
					} else {
						// we already have a connection, so just activate it
						QDBusObjectPath networkConnection(
								m_networkManager.ActivateConnection(
										connectionPaths.first(), devicePath,
										accessPointPath));
					}
				}
			}
		}
	}
}

