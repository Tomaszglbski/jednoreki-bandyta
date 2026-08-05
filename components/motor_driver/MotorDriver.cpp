#include "MotorDriver.h"

#include <esp_modbus_common.h>

#include "driver/uart.h"
#include "esp_err.h"
#include "esp_log.h"
#include "mbcontroller.h"
#include "modbus_data_dictionary.h"
#include "sdkconfig.h"

#define PWM_ENABLE  0x01
#define PWM_DISABLE 0x00

static const char* TAG = "MotorDriver";

MotorDriver::MotorDriver() {
    // Initialization of Modbus master for serial port
    comm_info.port = static_cast<uart_port_t>(CONFIG_MODBUS_UART_PORT);
    comm_info.mode = MB_MODE_RTU;
    comm_info.baudrate = CONFIG_MODBUS_UART_BAUDRATE;
    comm_info.parity = UART_PARITY_DISABLE;
}

MotorDriver::~MotorDriver() {
    esp_err_t err = mbc_master_destroy();

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "mb constroller destruction fail.");
    }
}

esp_err_t MotorDriver::init(uint16_t pwm_duty, uint16_t pwm_frq) {
    // setting up modbus master api
    esp_err_t err = mbc_master_set_descriptor(device_parameters, num_device_parameters);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "mb controller set descriptor fail.");
        return err;
    }

    err = mbc_master_init(MB_PORT_SERIAL_MASTER, &master_handler);
    if (master_handler == nullptr || err != ESP_OK) {
        ESP_LOGE(TAG, "mb controller initialization fail.");
        return err;
    }

    err = mbc_master_setup((void*)&comm_info);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "mb controller setup fail.");
        return err;
    }

    err = mbc_master_start();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "mb controller start fail, err=%x.", err);
        return err;
    }

    // setting up UART pins
    err = uart_set_pin(static_cast<uart_port_t>(CONFIG_MODBUS_UART_PORT), CONFIG_MODBUS_TX_GPIO, CONFIG_MODBUS_RX_GPIO,
                       UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "uart_set_pin FAIL");
        return err;
    }

    // setting pwm frequency
    uint16_t set_frq = pwm_frq;
    uint8_t type = 0;

    const mb_parameter_descriptor_t* param_descriptor = nullptr;

    err = mbc_master_get_cid_info(CID_PWM_FRQ, &param_descriptor);

    if ((err == ESP_OK) && (param_descriptor != nullptr)) {
        err = mbc_master_set_parameter(param_descriptor->cid, const_cast<char*>(param_descriptor->param_key),
                                       reinterpret_cast<uint8_t*>(&set_frq), &type);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "PWM frequency set to %d", set_frq);
        } else {
            ESP_LOGE(TAG, "Failed to set PWM frequency (set parameter error)");
        }
    } else {
        ESP_LOGE(TAG, "Failed to set PWM frequency (get_cid_info error)");
    }

    // setting pwm duty ratio
    uint16_t set_duty = pwm_duty;

    err = mbc_master_get_cid_info(CID_PWM_DR, &param_descriptor);

    if ((err == ESP_OK) && (param_descriptor != nullptr)) {
        err = mbc_master_set_parameter(param_descriptor->cid, const_cast<char*>(param_descriptor->param_key),
                                       reinterpret_cast<uint8_t*>(&set_duty), &type);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "PWM duty ratio set to %d \%", set_duty);
        } else {
            ESP_LOGE(TAG, "Failed to set PWM duty ratio (set parameter error)");
        }
    } else {
        ESP_LOGE(TAG, "Failed to set PWM duty ratio (get_cid_info error)");
    }

    return err;
}

esp_err_t MotorDriver::startSpinning() {
    const mb_parameter_descriptor_t* param_descriptor = nullptr;

    esp_err_t err = mbc_master_get_cid_info(CID_PWM_OUT_TOGGLE, &param_descriptor);

    uint16_t set_pwm = PWM_ENABLE;
    uint8_t type = 0;
    if ((err == ESP_OK) && (param_descriptor != nullptr)) {
        err = mbc_master_set_parameter(param_descriptor->cid, const_cast<char*>(param_descriptor->param_key),
                                       reinterpret_cast<uint8_t*>(&set_pwm), &type);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "PWM enabled)");
        } else {
            ESP_LOGE(TAG, "Failed to enable PWM");
        }
    } else {
        ESP_LOGE(TAG, "Failed to enable PWM (get_cid_info error)");
    }

    return err;
}

esp_err_t MotorDriver::stopSpinnig() {
    const mb_parameter_descriptor_t* param_descriptor = nullptr;

    esp_err_t err = mbc_master_get_cid_info(CID_PWM_OUT_TOGGLE, &param_descriptor);

    uint16_t set_pwm = PWM_DISABLE;
    uint8_t type = 0;
    if ((err == ESP_OK) && (param_descriptor != nullptr)) {
        err = mbc_master_set_parameter(param_descriptor->cid, const_cast<char*>(param_descriptor->param_key),
                                       reinterpret_cast<uint8_t*>(&set_pwm), &type);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "PWM disabled)");
        } else {
            ESP_LOGE(TAG, "Failed to disable PWM");
        }
    } else {
        ESP_LOGE(TAG, "Failed to disable PWM (get_cid_info error)");
    }

    return err;
}
