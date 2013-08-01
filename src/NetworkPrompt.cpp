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

#include <NetworkManagerDeviceWirelessInterface.h>
#include <NetworkManagerAccessPointInterface.h>
#include <NetworkPrompt.h>
#include <WiFiMenu.h>

#include <NetworkManager.h>

NetworkPrompt::NetworkPrompt(const QDBusConnection &sessionConnection,
		const QDBusConnection &systemConnection, QObject *parent) :
		QObject(parent), m_sessionConnection(sessionConnection), m_systemConnection(
				systemConnection), m_networkManager(NM_DBUS_SERVICE,
				NM_DBUS_PATH, m_systemConnection) {
}

NetworkPrompt::~NetworkPrompt() {
}

QDBusObjectPath NetworkPrompt::activateAccessPoint(
		const QDBusObjectPath& accessPointPath,
		const OrgFreedesktopNetworkManagerDeviceInterface& device,
		const QDBusObjectPath& devicePath) {

	qDebug() << "access point: " << accessPointPath.path();
	QList<QDBusObjectPath> connectionPaths(device.availableConnections());

	if (connectionPaths.empty()) {
		// if no existing connections, then add a new one
		return m_networkManager.AddAndActivateConnection(QVariantMap(),
				devicePath, accessPointPath);
	} else {
		// we already have a connection, so just activate it
		return m_networkManager.ActivateConnection(connectionPaths.first(),
				devicePath, accessPointPath);
	}
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

		WiFiMenuPtr wifiMenu(new WiFiMenu());

		for (const QDBusObjectPath &devicePath : devicePaths) {
			OrgFreedesktopNetworkManagerDeviceInterface deviceInterface(
					NM_DBUS_SERVICE, devicePath.path(), m_systemConnection);
			if (deviceInterface.deviceType() == NM_DEVICE_TYPE_WIFI) {

				OrgFreedesktopNetworkManagerDeviceWirelessInterface wifiDeviceInterface(
						NM_DBUS_SERVICE, devicePath.path(), m_systemConnection);

				DevicePtr device(new Device());
				device->setPath(devicePath.path().toStdString());
				wifiMenu->addDevice(device);

				QDBusPendingReply<void> reply(
						wifiDeviceInterface.RequestScan(QVariantMap()));
				reply.waitForFinished();

				QList<QDBusObjectPath> accessPointPaths(
						wifiDeviceInterface.GetAccessPoints());

				for (const QDBusObjectPath &accessPointPath : accessPointPaths) {
					OrgFreedesktopNetworkManagerAccessPointInterface accessPointInterface(
							NM_DBUS_SERVICE, accessPointPath.path(),
							m_systemConnection);

					AccessPointPtr accessPoint(new AccessPoint());
					accessPoint->setSsid(accessPointInterface.ssid().data());
					accessPoint->setAdhoc(
							accessPointInterface.mode()
									== NM_802_11_MODE_ADHOC);
					accessPoint->setSecure(
							accessPointInterface.flags()
									== NM_802_11_AP_FLAGS_PRIVACY);
					accessPoint->setBssid(
							accessPointInterface.hwAddress().toStdString());
					accessPoint->setPath(accessPointPath.path().toStdString());
					device->addAccessPoint(accessPoint);
				}

			}
		}

		MenuExporter exporter(wifiMenu);
//		activateAccessPoint(accessPointPath, device, devicePath);
	}
}

