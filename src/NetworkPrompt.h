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

#ifndef NETWORKPROMPT_H_
#define NETWORKPROMPT_H_

#include <NetworkManagerInterface.h>
#include <UnitySystemDialogInterface.h>

#include <QObject>
#include <QDBusConnection>

class NetworkPrompt: public QObject {
public:
	explicit NetworkPrompt(const QDBusConnection &sessionConnection,
			const QDBusConnection &systemConnection, QObject *parent = 0);

	virtual ~NetworkPrompt();

	virtual void check();

protected:
	QDBusConnection m_sessionConnection;

	QDBusConnection m_systemConnection;

	OrgFreedesktopNetworkManagerInterface m_networkManager;

	ComCanonicalUnitySystemDialogInterface m_systemDialog;
};

#endif // NETWORKPROMPT_H_
