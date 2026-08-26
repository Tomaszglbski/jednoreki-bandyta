#include "MotorAO.h"
#include "esp_random.h"

//Motor timeout in uS
#define MOTOR_TIMEOUT_TIME(rand) (4000000U + ((rand % 11U) * 100000U))

MotorAO::MotorAO(const char* const pcName, Publisher &bus, uint16_t pwm_duty, uint16_t pwm_frq) : ActiveObject(pcName, bus), 
                                                                                                  currState(MotorState::IDLE),
                                                                                                  motorDriver(),
                                                                                                  timer(nullptr){
                                                                                                  
    motorDriver.init(pwm_duty, pwm_frq);
    
    esp_timer_create_args_t timerConfig{
        [](void* arg) -> void {
            static_cast<ActiveObject*>(arg)->receiveEvent(Signal::SPIN_TIMEOUT);
        },
        this,
        ESP_TIMER_TASK,
        "Motor Timer",
        false
    };

    esp_timer_create(&timerConfig, &timer);
}

void MotorAO::StateMachineHandler(const Signal signalID){


    switch (currState)
    {
        case MotorState::IDLE:
            switch (signalID)
            {
            case Signal::LEVER_PULLED:
                motorDriver.startSpinning();
                currState = MotorState::SPINNING;
                publishEvent(Signal::START_SPIN);
                esp_timer_stop(timer);
                esp_timer_start_once(timer, MOTOR_TIMEOUT_TIME(esp_random()));
                break;
            
            default:
                break;
            }
        break;

        case MotorState::SPINNING:
            switch (signalID)
            {
            case Signal::SPIN_TIMEOUT:
                motorDriver.stopSpinning();
                currState = MotorState::IDLE;
                publishEvent(Signal::STOP_SPIN);
                break;
            
            default:
                break;
            }
        break;
        
        default:
        break;
    }
}
