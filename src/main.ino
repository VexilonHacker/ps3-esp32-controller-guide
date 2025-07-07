#include <Arduino.h>
#include <Ps3Controller.h>

String Master_mac = "12:37:AC:4D:B2:E0";
void notify()
{
    // Analog sticks
    int lx = Ps3.data.analog.stick.lx;
    int ly = Ps3.data.analog.stick.ly;
    int rx = Ps3.data.analog.stick.rx;
    int ry = Ps3.data.analog.stick.ry;

    // Accelerometer
    int ax = Ps3.data.sensor.accelerometer.x;
    int ay = Ps3.data.sensor.accelerometer.y;
    int az = Ps3.data.sensor.accelerometer.z;

    // Output: lx ly rx ry | ax ay az
    Serial.printf("LX: %d | LY: %d | RX: %d | RY: %d | AX: %d | AY: %d | AZ: %d\n", 
                  lx, ly, rx, ry, ax, ay, az);
}

void setup()
{
    Serial.begin(115200);
    Ps3.attach(notify);
    Ps3.begin(Master_mac.c_str());
}

void loop()
{
}

