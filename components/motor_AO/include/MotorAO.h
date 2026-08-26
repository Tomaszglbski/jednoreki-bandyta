#ifndef MOTOR_AO_H
#define MOTOR_AO_H

#include "ActiveObject.h"
#include "MotorDriver.h"
#include "esp_timer.h"

enum class MotorState : uint8_t {

    IDLE,
    SPINNING   
};

class MotorAO : public ActiveObject{

public:
    
    MotorAO(const char* const pcName, Publisher &bus, uint16_t pwm_duty, uint16_t pwm_frq);

private: 

    void StateMachineHandler(const Signal signalID) override;
    MotorState currState;
    
    MotorDriver motorDriver;
    esp_timer_handle_t timer;
};



#endif