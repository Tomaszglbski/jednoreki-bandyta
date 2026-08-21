#ifndef EV_INTERFACES_H
#define EV_INTERFACES_H

#include "esp_err.h"

class Subscriber {
    public:
        virtual esp_err_t receiveEvent(const Signal signalID) = 0;
        virtual ~Subscriber() = default;
};

class Publisher {
    public:
        virtual esp_err_t publishEvent(const Signal signalID) = 0;
        virtual ~Publisher() = default;
};

#endif