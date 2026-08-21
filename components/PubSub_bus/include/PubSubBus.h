#ifndef PUB_SUB_BUS_H
#define PUB_SUB_BUS_H

#include <cstdint>

#include "esp_err.h"
#include "events.h"
#include "event_interfaces.h"


#define MAX_SUBSCRIBERS 16


class PubSubBus : public Publisher{

public:
    PubSubBus();
    virtual ~PubSubBus() = default;
    
    esp_err_t addSubscriber(Subscriber *subscriber, uint8_t priority, Signal sig);
    esp_err_t publishEvent(const Signal signalID) override;
private:

    Subscriber* subscribers[MAX_SUBSCRIBERS];

    uint16_t eventSubscriptions[EVENT_COUNT];

};



#endif