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

#ifndef NETWORKPROMPT_H_
#define NETWORKPROMPT_H_

#include <NetworkManagerInterface.h>
#include <NetworkManagerDeviceInterface.h>
#include <WiFiMenu.h>

#include <QObject>
#include <QDBusConnection>

class NetworkPrompt: public QObject {
public:
	explicit NetworkPrompt(const QDBusConnection &sessionConnection,
			const QDBusConnection &systemConnection, MenuFactoryPtr menuFactory,
			QObject *parent = 0);

	virtual ~NetworkPrompt();

	virtual void check();

protected:
	QDBusConnection m_sessionConnection;

	QDBusConnection m_systemConnection;

	OrgFreedesktopNetworkManagerInterface m_networkManager;

	MenuFactoryPtr m_menuFactory;

	QDBusObjectPath activateAccessPoint(const QDBusObjectPath& accessPointPath,
			const OrgFreedesktopNetworkManagerDeviceInterface& device,
			const QDBusObjectPath& devicePath);
};

#endif // NETWORKPROMPT_H_
