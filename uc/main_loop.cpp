#include <Arduino.h>
#include <Wire.h>

#define SLAVE_ADDRESS 0x03

int msg = 0;
int state = 0;

void receiveData(int byteCount);
void sendData();

struct Position {
    float x;
    float y;
};

struct Position pos;
uint8_t dist_sens[5] = {1, 3, 7, 11, 13};

void setup() 
{
    pinMode(13, OUTPUT);
    
    Serial.begin(9600);

    Wire.begin(SLAVE_ADDRESS);
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
}

void loop() 
{
    // toggle led with state
    if (state) {
        digitalWrite(13, HIGH);
    } else {
        digitalWrite(13, LOW);
    }

    // do some calculation
    for (uint32_t i=0; i<1000000; i++);

    // position inceased dummy
    pos.x += 0.1;
    pos.y += 0.15;

    // sensor array values dummy
    for (uint8_t i=0; i<sizeof(dist_sens); i++)
        dist_sens[i] += i;

    delay(100);
}

// callback for receiving data
void receiveData(int byteCount)
{
    while(Wire.available()) {
        msg = Wire.read();
 
        state = (msg == 1) ? 1 : 0;
     }
}

// callback for sending data
void sendData()
{
    switch (state) {
    case 0:
        Wire.write(dist_sens, 5);
        break;

    case 1:
        Wire.write((uint8_t *)&pos, sizeof(pos));
        break;

    default:
        // wrong request
        break;
    }
}
