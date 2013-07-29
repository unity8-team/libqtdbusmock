
set(CMAKE_AUTOMOC OFF)
include(FindGMock)
set(CMAKE_AUTOMOC ON)

find_package("Valgrind" REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})

add_definitions(-DDBUS_SYSTEM_CONFIG_FILE="${CMAKE_CURRENT_SOURCE_DIR}/data/system.conf")
add_definitions(-DDBUS_SESSION_CONFIG_FILE="${CMAKE_CURRENT_SOURCE_DIR}/data/session.conf")

set(
	UNIT_TESTS_SRC
	TestNetworkPrompt.cpp
	main.cpp
)

add_executable(
	unit-tests
	${UNIT_TESTS_SRC}
)

qt5_use_modules(
	unit-tests
	Core
	DBus
	Test
)

target_link_libraries(
	unit-tests
	indicator-network-prompt
	${QTDBUSMOCK_LIBRARIES}
	${QTDBUSTEST_LIBRARIES}
	${GTEST_LIBRARIES}
	${GMOCK_LIBRARIES}
)

add_valgrind_test(
	unit-tests
	unit-tests
)
