#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <esp_modbus_common.h>

#include "esp_err.h"
#include "mbcontroller.h"

class MotorDriver {
   public:
    MotorDriver();
    ~MotorDriver();

    esp_err_t init(uint16_t pwm_duty, uint16_t pwm_frq);
    esp_err_t startSpinning();
    esp_err_t stopSpinnig();

   private:
    void* master_handler = nullptr;
    mb_communication_info_t comm_info;
};

#endif
