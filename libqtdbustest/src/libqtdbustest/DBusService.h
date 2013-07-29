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

#ifndef QTDBUSTEST_DBUSSERVICE_H_
#define QTDBUSTEST_DBUSSERVICE_H_

#include <QDBusConnection>
#include <QSharedPointer>

namespace QtDBusTest {

class DBusService;

typedef QSharedPointer<DBusService> DBusServicePtr;

class DBusService {
public:
	DBusService(const QString &interface, QDBusConnection::BusType busType);

	virtual ~DBusService();

	virtual const QString & interface() const;

	virtual QDBusConnection::BusType busType() const;

	virtual void start(const QDBusConnection &connection) = 0;

protected:
	QString m_interface;

	QDBusConnection::BusType m_busType;
};

}

#endif /* QTDBUSTEST_DBUSSERVICE_H_ */
