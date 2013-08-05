
set(
	QTDBUSMOCK_SRC
	DBusMock.cpp
	Method.cpp
	MethodCall.cpp
	NamedMethodCall.cpp
)

set(
	QTDBUSMOCK_PUBLIC_HEADERS
	"${CMAKE_CURRENT_BINARY_DIR}/config.h"
	DBusMock.h
	Method.h
	MethodCall.h
	NamedMethodCall.h
)

set_source_files_properties(
	"${DATA_DIR}/org.freedesktop.DBus.Mock.xml"
	PROPERTIES
	NO_NAMESPACE YES
	INCLUDE "libqtdbusmock/MockInterfaceClasses.h"
)

set_source_files_properties(
	"${DATA_DIR}/org.freedesktop.DBus.Properties.xml"
	PROPERTIES
	NO_NAMESPACE YES
	INCLUDE "libqtdbusmock/ExportInterfaces.h"
)

set_source_files_properties(
	"${DATA_DIR}/nm-manager-mock.xml"
	PROPERTIES
	NO_NAMESPACE YES
	CLASSNAME NetworkManagerMockInterface
	INCLUDE "libqtdbusmock/ExportInterfaces.h"
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

set_target_properties(
	qtdbusmock
	PROPERTIES
	VERSION ${API_VERSION}.0.0
	SOVERSION ${ABI_VERSION}
	PUBLIC_HEADER "${QTDBUSMOCK_PUBLIC_HEADERS}"
)

install(
	TARGETS qtdbusmock
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/libqtdbusmock-${API_VERSION}/libqtdbusmock"
)

# Package config

set(LIBQTDBUSMOCK_PC "${CMAKE_CURRENT_BINARY_DIR}/libqtdbusmock-${API_VERSION}.pc")
configure_file("libqtdbusmock.pc.in" "${LIBQTDBUSMOCK_PC}" @ONLY)

install(
	FILES "${LIBQTDBUSMOCK_PC}"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)
