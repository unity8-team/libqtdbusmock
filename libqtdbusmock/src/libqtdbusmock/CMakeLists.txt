
set(
	QTDBUSMOCK_SRC
	DBusMock.cpp
	MethodCall.cpp
)

set_source_files_properties(
	"${DATA_DIR}/org.freedesktop.DBus.Mock.xml"
	PROPERTIES
	NO_NAMESPACE YES
	INCLUDE "libqtdbusmock/MethodCall.h"
)

set_source_files_properties(
	"${DATA_DIR}/org.freedesktop.DBus.Properties.xml"
	PROPERTIES
	NO_NAMESPACE YES
)

set_source_files_properties(
	"${DATA_DIR}/nm-manager-mock.xml"
	PROPERTIES
	NO_NAMESPACE YES
	CLASSNAME NetworkManagerMockInterface
)

qt5_add_dbus_interface(
	QTDBUSMOCK_SRC
	"${DATA_DIR}/org.freedesktop.DBus.Mock.xml"
	MockInterface
)	

qt5_add_dbus_interface(
	QTDBUSMOCK_SRC
	"${DATA_DIR}/org.freedesktop.DBus.Properties.xml"
	PropertiesInterface
)

qt5_add_dbus_interface(
	QTDBUSMOCK_SRC
	"${DATA_DIR}/nm-manager-mock.xml"
	NetworkManagerMockInterface
)

add_library(
	qtdbusmock
	SHARED
	${QTDBUSMOCK_SRC}
)

qt5_use_modules(
	qtdbusmock
	Core
	DBus
	Test
)
