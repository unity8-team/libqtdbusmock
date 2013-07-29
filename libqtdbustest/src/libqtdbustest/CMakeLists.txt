
add_definitions(-DDBUS_SYSTEM_CONFIG_FILE="${DATA_DIR}/system.conf")
add_definitions(-DDBUS_SESSION_CONFIG_FILE="${DATA_DIR}/session.conf")

set(
	QTDBUSTEST_SRC
	DBusTestRunner.cpp
	DBusService.cpp
	QProcessDBusService.cpp
)

add_library(
	qtdbustest
	SHARED
	${QTDBUSTEST_SRC}
)

qt5_use_modules(
	qtdbustest
	Core
	DBus
	Test
)
