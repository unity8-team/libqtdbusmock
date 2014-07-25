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

#include <libqtdbusmock/DBusMock.h>
#include <libqtdbustest/QProcessDBusService.h>

#include <NetworkManager.h>

using namespace QtDBusTest;

namespace QtDBusMock {

class DBusMockPrivate {
public:
	DBusMockPrivate(DBusTestRunner &testRunner) :
			m_testRunner(testRunner) {
	}

	QtDBusTest::DBusTestRunner &m_testRunner;

	QScopedPointer<NetworkManagerMockInterface> m_networkManagerMock;

	QMap<QString, QSharedPointer<OrgFreedesktopDBusMockInterface> > m_mockInterfaces;
};

DBusMock::DBusMock(DBusTestRunner &testRunner) :
		d(new DBusMockPrivate(testRunner)) {
}

DBusMock::~DBusMock() {
}

void DBusMock::registerMetaTypes() {
	Method::registerMetaType();
	MethodCall::registerMetaType();
	NamedMethodCall::registerMetaType();
}

void DBusMock::registerTemplate(const QString &service,
                                const QString &templateName,
                                QDBusConnection::BusType busType) {
	d->m_testRunner.registerService(
			DBusServicePtr(
					new QProcessDBusService(service,
							busType, "python3",
							QStringList() << "-m" << "dbusmock" << "--template"
									<< templateName)));
}

void DBusMock::registerNetworkManager() {
	registerTemplate(NM_DBUS_SERVICE, "networkmanager", QDBusConnection::SystemBus);
}

void DBusMock::registerCustomMock(const QString &name, const QString &path,
		const QString &interface, QDBusConnection::BusType busType) {
	QStringList args;
	args << "-m" << "dbusmock";
	if (busType == QDBusConnection::SystemBus) {
		args << "-s";
	}
	args << name << path << interface;
	d->m_testRunner.registerService(
			DBusServicePtr(
					new QProcessDBusService(name, busType, "python3", args)));
}

NetworkManagerMockInterface & DBusMock::networkManagerInterface() {
	if (d->m_networkManagerMock.isNull()) {
		d->m_networkManagerMock.reset(
				new NetworkManagerMockInterface(NM_DBUS_INTERFACE, NM_DBUS_PATH,
						d->m_testRunner.systemConnection()));
	}
	return *d->m_networkManagerMock;
}

OrgFreedesktopDBusMockInterface & DBusMock::mockInterface(const QString &name,
		const QString &path, const QString &interface,
		QDBusConnection::BusType busType) {
	Q_UNUSED(interface);
	QString id(name);
	id.append(':');
	id.append(path);
	auto it(d->m_mockInterfaces.find(id));
	if (it == d->m_mockInterfaces.end()) {
		switch (busType) {
		case QDBusConnection::SystemBus:
			it = d->m_mockInterfaces.insert(id,
					QSharedPointer<OrgFreedesktopDBusMockInterface>(
							new OrgFreedesktopDBusMockInterface(name, path,
									d->m_testRunner.systemConnection())));
			break;
		case QDBusConnection::SessionBus:
			it = d->m_mockInterfaces.insert(id,
					QSharedPointer<OrgFreedesktopDBusMockInterface>(
							new OrgFreedesktopDBusMockInterface(name, path,
									d->m_testRunner.sessionConnection())));
			break;
		case QDBusConnection::ActivationBus:
			qWarning() << "Unknown bus type";
			break;
		}
	}
	return **it;
}

}
