
set(
	INDICATOR_NETWORK_PROMPT_SOURCES
	NetworkPrompt.cpp
)

set_source_files_properties(
	"${DATA_DIR}/nm-device.xml"
	"${DATA_DIR}/nm-device-wifi.xml"
	"${DATA_DIR}/nm-manager.xml"
	PROPERTIES
	NO_NAMESPACE YES
)

qt5_add_dbus_interface(
	INDICATOR_NETWORK_PROMPT_SOURCES
	"${DATA_DIR}/nm-manager.xml"
	NetworkManagerInterface
)

qt5_add_dbus_interface(
	INDICATOR_NETWORK_PROMPT_SOURCES
	"${DATA_DIR}/nm-device.xml"
	NetworkManagerDeviceInterface
)

qt5_add_dbus_interface(
	INDICATOR_NETWORK_PROMPT_SOURCES
	"${DATA_DIR}/nm-device-wifi.xml"
	NetworkManagerDeviceWirelessInterface
)

add_library(
	indicator-network-prompt
	STATIC
	${INDICATOR_NETWORK_PROMPT_SOURCES}
)

qt5_use_modules(
	indicator-network-prompt
	Core
	DBus
)

add_executable(
	indicator-network-prompt-bin
	main.cpp
)

set_target_properties(
	indicator-network-prompt-bin
	PROPERTIES
	OUTPUT_NAME indicator-network-prompt
)

qt5_use_modules(
	indicator-network-prompt-bin
	Core
)

target_link_libraries(
	indicator-network-prompt-bin
	indicator-network-prompt
)


