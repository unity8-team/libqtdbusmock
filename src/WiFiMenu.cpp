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

#include <WiFiMenu.h>

#include <glibmm/variant.h>
#include <glibmm/main.h>
#include <glibmm/main.h>

#include <giomm/dbusownname.h>
#include <giomm/dbusconnection.h>
#include <giomm/menu.h>

#include <iostream>

using namespace std;
using namespace Glib;
using namespace Gio;
using namespace Gio::DBus;

typedef Variant<Glib::ustring> StringVariant;

class WiFiMenuPrivate {
public:
	WiFiMenuPrivate() :
			m_menu(Menu::create()) {

	}

	RefPtr<Menu> m_menu;
};

class MenuExporterPrivate {
public:
	MenuExporterPrivate(WiFiMenuPtr menu) :
			m_menu(menu), m_exportId(0) {
	}

	~MenuExporterPrivate() {
		m_connection->unexport_menu_model(m_exportId);
		m_connection->close_sync();
	}

	void bus_acquired_slot(const RefPtr<Connection> &connection, ustring name) {
		m_connection = connection;
		cout << "create menu" << name << endl;

		m_exportId = connection->export_menu_model("/com/canonical/testmenu",
				m_menu->d->m_menu);

		//TODO here we pass the menu model to the snap-decision thingy

		m_mainloop->quit();
	}

	RefPtr<MainLoop> m_mainloop;

	WiFiMenuPtr m_menu;

	RefPtr<Connection> m_connection;

	unsigned int m_exportId;
};

class DevicePrivate {
public:
	DevicePrivate() :
			m_item(MenuItem::create("Select wireless network", ustring())), m_menu(
					Menu::create()) {
		m_item->set_section(m_menu);
	}

	RefPtr<MenuItem> m_item;

	RefPtr<Menu> m_menu;
};

class AccessPointPrivate {
public:
	AccessPointPrivate() :
			m_item(MenuItem::create(ustring(), ustring())) {
	}

	RefPtr<MenuItem> m_item;
};

MenuExporter::MenuExporter(WiFiMenuPtr menu) :
		d(new MenuExporterPrivate(menu)) {
	own_name(BUS_TYPE_SESSION, "com.canonical.testmenu",
			sigc::mem_fun(*d, &MenuExporterPrivate::bus_acquired_slot));
	d->m_mainloop = MainLoop::create();
	d->m_mainloop->run();

	//    actions = Gio.SimpleActionGroup.new()
	//    actions.add_action(Gio.SimpleAction.new("bar", None))
	//    bus.export_action_group(BUS_OBJECT_PATH, actions)
}

MenuExporter::~MenuExporter() {
}

WiFiMenu::WiFiMenu() :
		d(new WiFiMenuPrivate()) {
}

WiFiMenu::~WiFiMenu() {
}

void WiFiMenu::addDevice(DevicePtr device) {
	d->m_menu->append_item(device->d->m_item);
}

Device::Device() :
		d(new DevicePrivate()) {

//	var busy_action_id = device.get_path() + "::is-busy";
//	var device_item = new MenuItem("Select wireless network", null);
	d->m_item->set_attribute_value("type",
			StringVariant::create("x-canonical-system-settings"));
	d->m_item->set_attribute_value("x-canonical-type",
			StringVariant::create(
					"unity.widget.systemsettings.tablet.sectiontitle"));
	d->m_item->set_attribute_value("x-canonical-children-display",
			StringVariant::create("inline"));
//	device_item.set_attribute("x-canonical-busy-action", "s", busy_action_id);
}

Device::~Device() {
}

void Device::addAccessPoint(AccessPointPtr accessPoint) {
	d->m_menu->append_item(accessPoint->d->m_item);
}

void Device::setPath(const std::string &path) {
	d->m_item->set_attribute_value("x-canonical-wifi-device-path",
			StringVariant::create(path));
}

AccessPoint::AccessPoint() :
		d(new AccessPointPrivate()) {

//	var strength_action_id = ap.get_path() + "::strength";
//	var activate_action_id = ap.get_path();

	d->m_item->set_attribute_value("type",
			StringVariant::create("x-canonical-system-settings"));
	d->m_item->set_attribute_value("x-canonical-type",
			StringVariant::create(
					"unity.widgets.systemsettings.tablet.accesspoint"));

//	item.set_attribute("x-canonical-wifi-ap-strength-action", "s",
//			strength_action_id);
//	item.set_attribute("action", "s", activate_action_id);
}

AccessPoint::~AccessPoint() {
}

void AccessPoint::setSsid(const char *ssid) {
	d->m_item->set_label(ustring(ssid));
}

void AccessPoint::setAdhoc(bool adhoc) {
	d->m_item->set_attribute_value("x-canonical-wifi-ap-is-adhoc",
			Variant<bool>::create(adhoc));
}

void AccessPoint::setSecure(bool secure) {
	d->m_item->set_attribute_value("x-canonical-wifi-ap-is-secure",
			Variant<bool>::create(secure));
}

void AccessPoint::setBssid(const std::string &bssid) {
	d->m_item->set_attribute_value("x-canonical-wifi-ap-bssid",
			StringVariant::create(bssid));
}

void AccessPoint::setPath(const std::string &path) {
	d->m_item->set_attribute_value("x-canonical-wifi-ap-dbus-path",
			StringVariant::create(path));
}
