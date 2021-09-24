
set(COMPONENT_SRCS "bt_app_av.c" "bt_app_core.c" "main.c" "tuning_http_server.c" "wifi_handler.c" INCLUDE_DIRS "." )
set(COMPONENT_ADD_INCLUDEDIRS "include")

register_component()

spiffs_create_partition_image(www "${CMAKE_CURRENT_SOURCE_DIR}/../frontend" FLASH_IN_PROJECT)