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

#include <libqtdbusmock/DBusMock.h>
#include <QCoreApplication>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace std;
using namespace testing;
using namespace QtDBusTest;
using namespace QtDBusMock;

namespace {

class TestDBusMock: public Test {
protected:
	TestDBusMock() :
			dbusMock(dbusTestRunner) {
	}

	virtual ~TestDBusMock() {
	}

	bool processListContains(const char *str) {
		QProcess pgrep;
		pgrep.start("ps",
				QStringList() << "--no-headers" << "--ppid"
						<< QString::number(QCoreApplication::applicationPid())
						<< "-o" << "args");
		pgrep.waitForFinished();
		pgrep.waitForReadyRead();

		return pgrep.readAll().contains(str);
	}

	DBusTestRunner dbusTestRunner;

	DBusMock dbusMock;
};

TEST_F(TestDBusMock, StartsDBusMockSession) {
	dbusMock.registerCustomMock("test.name", "/test/object", "test.Interface",
			QDBusConnection::SessionBus);
	dbusTestRunner.startServices();

	EXPECT_TRUE(
			processListContains(
					"python3 -m dbusmock test.name /test/object test.Interface"));
}

TEST_F(TestDBusMock, StartsDBusMockSystem) {
	dbusMock.registerCustomMock("test.name2", "/test/object2",
			"test.Interface2", QDBusConnection::SystemBus);
	dbusTestRunner.startServices();

	EXPECT_TRUE(
			processListContains(
					"python3 -m dbusmock -s test.name2 /test/object2 test.Interface2"));
}

TEST_F(TestDBusMock, StartsDBusMockWithTemplate) {
	dbusMock.registerNetworkManager();
	dbusTestRunner.startServices();

	EXPECT_TRUE(
			processListContains(
					"python3 -m dbusmock --template networkmanager"));
}

} // namespace
