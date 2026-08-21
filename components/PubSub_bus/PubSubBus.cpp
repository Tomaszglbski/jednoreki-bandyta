#include "PubSubBus.h"

#include "esp_log.h"
#include <assert.h>

static const char* TAG = "PubSub bus";

PubSubBus::PubSubBus() {}

esp_err_t PubSubBus::addSubscriber(Subscriber *subscriber, uint8_t priority, Signal sig){

    assert((priority > 0) && (priority <= 16));

    subscribers[priority - 1] = subscriber;
    eventSubscriptions[static_cast<std::size_t>(sig)] |= (1U <<  (priority - 1));
        
    ESP_LOGI(TAG, "new subscriber: priority %d, event: %s", priority, signalToString(sig));
    return ESP_OK;

}

esp_err_t PubSubBus::publishEvent(const Signal sig){

    uint16_t subMask = eventSubscriptions[static_cast<std::size_t>(sig)];

    if (subMask == 0){

        ESP_LOGE(TAG, "Event: %s has no subscribers", signalToString(sig));
        return ESP_OK;

    }else{

        while(subMask != 0){

            uint32_t lZ = __builtin_clz((uint32_t)(subMask));
            uint16_t bitIdx = (uint16_t)(31U - lZ); 
            
            subscribers[bitIdx]->receiveEvent(sig);

            ESP_LOGI(TAG, "Event %s sent to priority %d", signalToString(sig), (bitIdx + 1));

            subMask &= ~((uint16_t) (1U << bitIdx));
        }
    }

    return ESP_OK;
}