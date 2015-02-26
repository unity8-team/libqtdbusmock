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

#ifndef QTDBUSMOCK_DBUSMOCK_H_
#define QTDBUSMOCK_DBUSMOCK_H_

#include <libqtdbustest/DBusTestRunner.h>
#include <libqtdbusmock/MockInterface.h>
#include <libqtdbusmock/NetworkManagerMockInterface.h>
#include <libqtdbusmock/OfonoMockInterface.h>
#include <libqtdbusmock/OfonoModemInterface.h>
#include <libqtdbusmock/OfonoSimManagerInterface.h>
#include <libqtdbusmock/OfonoNetworkRegistrationInterface.h>
#include <libqtdbusmock/MockInterfaceClasses.h>
#include <libqtdbusmock/URfkillInterface.h>
#include <libqtdbusmock/URfkillKillswitchInterface.h>

namespace QtDBusMock {

class DBusMockPrivate;

class Q_DECL_EXPORT DBusMock {
public:
	DBusMock(QtDBusTest::DBusTestRunner &testRunner);

	virtual ~DBusMock();

	Q_DECL_EXPORT
	static void registerMetaTypes();

	virtual void registerTemplate(const QString &service,
	                              const QString &templateName,
	                              QDBusConnection::BusType busType);

	virtual void registerNetworkManager();

	virtual void registerOfono();

	virtual void registerURfkill();

	virtual void registerCustomMock(const QString &name, const QString &path,
			const QString &interface, QDBusConnection::BusType);

	virtual NetworkManagerMockInterface & networkManagerInterface();

	virtual OrgFreedesktopDBusMockInterface & mockInterface(const QString &name,
			const QString &path, const QString &interface,
			QDBusConnection::BusType busType);

	virtual OfonoMockInterface & ofonoInterface();

	virtual OfonoModemInterface & ofonoModemInterface(int modemIndex);

	virtual OfonoSimManagerInterface & ofonoSimManagerInterface(int modemIndex);

	virtual OfonoNetworkRegistrationInterface & ofonoNetworkRegistrationInterface(int modemIndex);

	virtual OrgFreedesktopURfkillInterface & urfkillInterface();

	virtual OrgFreedesktopURfkillKillswitchInterface & urfkillKillswitchInterface(const QString& device);

private:
	QSharedPointer<DBusMockPrivate> d;
};

}

#endif /* QTDBUSMOCK_DBUSMOCK_H_ */
