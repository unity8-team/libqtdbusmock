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

#ifndef WIFIMENU_H_
#define WIFIMENU_H_

#include <memory>

class MenuExporterPrivate;
class WiFiMenuPrivate;
class AccessPointPrivate;
class DevicePrivate;

class MenuExporter;
class WiFiMenu;
class Device;
class AccessPoint;

typedef std::shared_ptr<MenuExporter> MenuExporterPtr;
typedef std::shared_ptr<WiFiMenu> WiFiMenuPtr;
typedef std::shared_ptr<Device> DevicePtr;
typedef std::shared_ptr<AccessPoint> AccessPointPtr;

class MenuExporter {
public:
	explicit MenuExporter(WiFiMenuPtr menu);

	virtual ~MenuExporter();

protected:
	std::unique_ptr<MenuExporterPrivate> d;
};

class WiFiMenu {
	friend class MenuExporter;
	friend class MenuExporterPrivate;
public:
	explicit WiFiMenu();

	virtual ~WiFiMenu();

	virtual void addDevice(DevicePtr device);

protected:
	std::unique_ptr<WiFiMenuPrivate> d;
};

class Device {
	friend class WiFiMenu;

public:
	explicit Device();

	virtual ~Device();

	virtual void addAccessPoint(AccessPointPtr accessPoint);

	virtual void setPath(const std::string &path);

protected:
	std::unique_ptr<DevicePrivate> d;
};

class AccessPoint {
	friend class Device;
public:
	explicit AccessPoint();

	virtual ~AccessPoint();

	virtual void setSsid(const char *ssid);

	virtual void setAdhoc(bool adhoc);

	virtual void setSecure(bool secure);

	virtual void setBssid(const std::string &bssid);

	virtual void setPath(const std::string &path);

protected:
	std::unique_ptr<AccessPointPrivate> d;
};

#endif /* WIFIMENU_H_ */
