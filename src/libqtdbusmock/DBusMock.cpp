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

	QScopedPointer<OfonoMockInterface> m_ofonoInterface;

	QScopedPointer<OfonoSimManagerInterface> m_ofonoSimManagerInterface;

	QScopedPointer<OrgFreedesktopURfkillInterface> m_urfkillInterface;

	QMap<QString, QSharedPointer<OrgFreedesktopURfkillKillswitchInterface> > m_urfkillKillSwitchInterfaces;

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

void DBusMock::registerOfono() {
	registerTemplate("org.ofono", "ofono", QDBusConnection::SystemBus);
}

void DBusMock::registerURfkill() {
	registerTemplate("org.freedesktop.URfkill", "urfkill", QDBusConnection::SystemBus);
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

OfonoMockInterface & DBusMock::ofonoInterface() {
	if (d->m_ofonoInterface.isNull()) {
		d->m_ofonoInterface.reset(
				new OfonoMockInterface("org.ofono", "/",
						d->m_testRunner.systemConnection()));
	}
	return *d->m_ofonoInterface;
}

OfonoSimManagerInterface & DBusMock::ofonoSimManagerInterface(int modemIndex) {
	if (d->m_ofonoSimManagerInterface.isNull()) {
		d->m_ofonoSimManagerInterface.reset(
				new OfonoSimManagerInterface("org.ofono", "/ril_" + QString::number(modemIndex),
						d->m_testRunner.systemConnection()));
	}
	return *d->m_ofonoSimManagerInterface;
}

OrgFreedesktopURfkillInterface & DBusMock::urfkillInterface() {
	if (d->m_urfkillInterface.isNull()) {
		d->m_urfkillInterface.reset(
				new OrgFreedesktopURfkillInterface(
						"org.freedesktop.URfkill", "/org/freedesktop/URfkill",
						d->m_testRunner.systemConnection()));
	}
	return *d->m_urfkillInterface;
}

OrgFreedesktopURfkillKillswitchInterface & DBusMock::urfkillKillswitchInterface(const QString& device)
{
	QSharedPointer<OrgFreedesktopURfkillKillswitchInterface> interface = d->m_urfkillKillSwitchInterfaces["device"];
	if (!interface) {
		interface.reset(
				new OrgFreedesktopURfkillKillswitchInterface(
						"org.freedesktop.URfkill",
						QString("/org/freedesktop/URfkill/%1").arg(device),
						d->m_testRunner.systemConnection()));
		d->m_urfkillKillSwitchInterfaces["device"] = interface;
	}
	return *interface;
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
