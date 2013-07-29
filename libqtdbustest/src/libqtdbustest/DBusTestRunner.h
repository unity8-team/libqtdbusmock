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

#include <libqtdbustest/DBusService.h>
#include <QtDBus>
#include <QMap>
#include <QString>

#ifndef QTDBUSTEST_DBUSTESTRUNNER_H_
#define QTDBUSTEST_DBUSTESTRUNNER_H_

namespace QtDBusTest {

class DBusTestRunner {
public:
	DBusTestRunner();

	virtual ~DBusTestRunner();

	virtual void startServices();

	virtual const QDBusConnection & connection() const;

	virtual const QString & bus() const;

	virtual void registerService(DBusServicePtr service);

protected:
	void startService(const DBusService &service);

	QString m_bus;

	QDBusConnection m_connection;

	QProcess m_dbus;

	QMap<QString, DBusServicePtr> m_services;
};

}

#endif // QTDBUSTEST_DBUSTESTRUNNER_H_
