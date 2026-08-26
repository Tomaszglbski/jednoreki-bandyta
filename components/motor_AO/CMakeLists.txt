idf_component_register(SRCS "MotorAO.cpp" "modbus_data_dictionary.cpp" "MotorDriver.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES esp-modbus
                    REQUIRES active_object)
