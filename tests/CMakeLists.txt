
set(CMAKE_AUTOMOC OFF)
include(FindGMock)
set(CMAKE_AUTOMOC ON)

find_package("Valgrind" REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})

set(
	UNIT_TESTS_SRC
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
#	indicator-network-prompt
	${QTDBUSMOCK_LIBRARIES}
	${QTDBUSTEST_LIBRARIES}
	${GTEST_LIBRARIES}
	${GMOCK_LIBRARIES}
)

add_valgrind_test(
	unit-tests
	unit-tests
)
