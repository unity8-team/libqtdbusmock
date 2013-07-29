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

#include <libqtdbustest/QProcessDBusService.h>

#include <QDebug>
#include <QSignalSpy>
#include <QDBusServiceWatcher>

using namespace QtDBusTest;

QProcessDBusService::QProcessDBusService(const QString &interface,
		QDBusConnection::BusType busType, const QString &program,
		const QStringList &arguments) :
		DBusService(interface, busType), m_program(program), m_arguments(
				arguments) {
}

QProcessDBusService::~QProcessDBusService() {
	m_process.terminate();
	Q_ASSERT(m_process.waitForFinished());

	m_process.waitForReadyRead();
	qDebug() << m_process.readAll();
}

void QProcessDBusService::start(const QDBusConnection &connection,
		const QString &bus) {
	QDBusServiceWatcher watcher(m_interface, connection,
			QDBusServiceWatcher::WatchForRegistration);
	QSignalSpy spy(&watcher,
			SIGNAL(serviceOwnerChanged(const QString &,const QString &,const QString &)));

	QProcessEnvironment env(QProcessEnvironment::systemEnvironment());
	switch (m_busType) {
	case QDBusConnection::SystemBus:
		env.insert("DBUS_SYSTEM_BUS_ADDRESS", bus);
		break;
	case QDBusConnection::SessionBus:
		env.insert("DBUS_SESSION_BUS_ADDRESS", bus);
		break;
	case QDBusConnection::ActivationBus:
		qWarning() << "Unknown bus type";
		break;
	}

	m_process.setProcessChannelMode(QProcess::MergedChannels);
	m_process.setProcessEnvironment(env);
	m_process.start(m_program, m_arguments);

	spy.wait();
	Q_ASSERT(!spy.empty());
	QVariantList arguments(spy.takeFirst());
	Q_ASSERT(m_interface == arguments.first().toString());
}
