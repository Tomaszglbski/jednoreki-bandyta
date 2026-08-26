#include <stdio.h>
#include "ActiveObject.h"

#include "esp_log.h"

ActiveObject::ActiveObject(const char* const pcName, Publisher &bus) : name(pcName), bus(bus) {

    uint32_t qLength = EVENT_QUANTITY;
    uint8_t itemSize = sizeof(Signal);

    MessageQueue = xQueueCreate(qLength, itemSize); 
}

esp_err_t ActiveObject::startAOTask( const UBaseType_t priority, const uint32_t stackDepth){

    xTaskCreate([](void* pvParameters)-> void {
        static_cast<ActiveObject*>(pvParameters)->run();},
                name,
                stackDepth,
                this,
                priority,
                &Task);

    ESP_LOGI(name, "Task created");
    return ESP_OK;
}

void ActiveObject::run(){

    ESP_LOGI(name, "Task running");
    static Signal signal;
     
    for(;;){
        
        if (xQueueReceive(MessageQueue, &signal, portMAX_DELAY) != errQUEUE_EMPTY){
            
            StateMachineHandler(signal);
        } 
    }
}

esp_err_t ActiveObject::receiveEvent(const Signal signalID){


    if (xQueueSend (MessageQueue, &signalID, 0) == errQUEUE_FULL){
        ESP_LOGE(name, "The Object's Queue is FULL!");
        return ESP_FAIL;
    };
    
    return ESP_OK;
}

esp_err_t ActiveObject::publishEvent(const Signal signalID){

    bus.publishEvent(signalID);
    
    ESP_LOGI(name, "%s event published", signalToString(signalID));
    return ESP_OK;
}



