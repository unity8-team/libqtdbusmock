
set(CMAKE_AUTOMOC OFF)
include(FindGMock)
set(CMAKE_AUTOMOC ON)

find_package("Valgrind" REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})

add_definitions(-DMENU_PRINTER="${CMAKE_CURRENT_BINARY_DIR}/menu-printer")
add_definitions(-DEXPECTED_MENU_WITH_EXISTING_CONNECTION="${CMAKE_CURRENT_SOURCE_DIR}/data/expectedMenuWithExistingConnection.txt")
add_definitions(-DEXPECTED_MENU_WITHOUT_EXISTING_CONNECTION="${CMAKE_CURRENT_SOURCE_DIR}/data/expectedMenuWithoutExistingConnection.txt")

add_executable(
	menu-printer
	MenuPrinter.cpp
)

qt5_use_modules(
	menu-printer
	Core
)

target_link_libraries(
	menu-printer
	${QMENUMODEL_LIBRARIES}
	indicator-network-prompt
)

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
