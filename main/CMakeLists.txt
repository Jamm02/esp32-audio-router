idf_component_register(SRCS "main.c"
		           "tuning_http_server.c" 
			   "wifi_handler.c"
                    INCLUDE_DIRS ".")
spiffs_create_partition_image(www "${CMAKE_CURRENT_SOURCE_DIR}/../frontend" FLASH_IN_PROJECT)
			