#include "events.h"

const char* signalToString(Signal sig) {
    
    switch(sig) {
        case Signal::START_SPIN: return "START_SPIN";
        case Signal::STOP_SPIN:  return "STOP_SPIN";
        default:           return "SIGNAL ID ERROR";
    }
}