/*
 * Project T3-2C
 * Description: IFTTT trigger when TSL2591 detects light
 * Author: Wian van Rooyen
 * Date: 04/06/2020
 */

#include <Adafruit_TSL2591.h>

//Adafruit_TSL2591 tsl;
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

const unsigned long PERIOD = 3000;
const char *EVENT_NAME = "Sun Light";


bool sensorReady = false;
bool lightState = false;
unsigned long lastPublish = 0;
char buf[256];

void setup() {
    Serial.begin(9600);
    
    sensorReady = tsl.begin();
}

void loop() {
    if (millis() - lastPublish >= PERIOD && sensorReady)
    {
        lastPublish = millis();
        
        uint32_t lum = tsl.getFullLuminosity();
        uint16_t ir, full;
        ir = lum >> 16;
        full = lum & 0xFFFF;
        
        snprintf(buf, sizeof(buf), "%f", full);
        
        if ((full > 1000) && (lightState == false))
        {
            Particle.publish(EVENT_NAME, "Start", PRIVATE);
            //Particle.publish("Luminosity", buf, PRIVATE);
            lightState = true;
        }
        else if ((full < 1000) && (lightState == true))
        {
            Particle.publish(EVENT_NAME, "End", PRIVATE);
            //Particle.publish("Luminosity", buf, PRIVATE);
            lightState = false;
        }
    }
}