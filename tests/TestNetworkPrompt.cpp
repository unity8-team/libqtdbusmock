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

#include <DBusTest.h>
#include <NetworkPrompt.h>

#include <stdexcept>
#include <gtest/gtest.h>

using namespace std;
using namespace testing;

namespace {

class TestNetworkPrompt: public DBusTest {
protected:
	TestNetworkPrompt() {
	}

	virtual ~TestNetworkPrompt() {
	}

};

TEST_F(TestNetworkPrompt, Foo) {
	NetworkPrompt networkPrompt(*connection);
}

} // namespace
