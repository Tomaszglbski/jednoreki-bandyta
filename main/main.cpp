#include "PubSubBus.h"
#include "MotorAO.h"
#include "bootloader_random.h"

extern "C" void app_main(void) { 
    
    bootloader_random_enable();
}