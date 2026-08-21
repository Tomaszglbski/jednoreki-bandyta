#ifndef EVENTS_H
#define EVENTS_H

#define EVENT_QUANTITY 10
#define EVENT_SIZE 1

#include <cstdint>

#define EVENT_COUNT (static_cast<size_t>(Signal::SIGNAL_COUNT))

enum class Signal : uint8_t{

/*Motor related signals*/    
    START_SPIN,
    STOP_SPIN,

/*Last enum entry - event quanity*/
    SIGNAL_COUNT
};

const char* signalToString(Signal sig);




#endif  