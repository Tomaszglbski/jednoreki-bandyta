#ifndef MODBUS_DATA_DICT_H
#define MODBUS_DATA_DICT_H

#include "limits.h"
#include "mbcontroller.h"

// Enumeration of modbus slave addresses accessed by master device
enum {
    MB_DEVICE_ADDR1 = 1,
    MB_SLAVE_COUNT,
};

// Enumeration of all supported CIDs for device
enum {
    CID_PID = 0,        // product ID
    CID_VID,            // version ID
    CID_DEV_ADDR,       // Slave address
    CID_SW_VER,         // software version
    CID_PWM_DR,         // PWM duty ration
    CID_PWM_FRQ,        // PWM frequency
    CID_PWM_OUT_TOGGLE  // PWM enable/disable
};

extern const mb_parameter_descriptor_t device_parameters[];
extern const uint16_t num_device_parameters;

#endif