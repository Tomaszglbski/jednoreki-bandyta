#ifndef AO_INTERFACE_H
#define AO_INTERFACE_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "events.h"
#include "event_interfaces.h"


class ActiveObject : public Subscriber{
public:

    ActiveObject(const char* const pcName, Publisher &bus);
    virtual ~ActiveObject() = default;

    esp_err_t startAOTask(const UBaseType_t priority, const uint32_t stackDepth);
    esp_err_t receiveEvent(const Signal signalID) override;

protected:

    const char* const name;

    esp_err_t publishEvent(const Signal signalID);

private:

    Publisher &bus;
    TaskHandle_t Task;
    QueueHandle_t MessageQueue;

    void run();
    virtual esp_err_t StateMachinehandler(const Signal signalID) = 0;
};

#endif