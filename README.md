## Quick Guide: Connect PS3 Controller to ESP32 on Linux

### 1. Install sixpair  
- On Arch Linux, `sixpair` is deprecated and moved to `bluez-deprecated-tools`.  
- Install it using:  
  `sudo pacman -S bluez-deprecated-tools`  
- For other distros, use your package manager to install the equivalent package.

### 2. Set PS3 Controller Master MAC Address  
- Find your ESP32 Bluetooth MAC address.  
- Run this command to configure your PS3 controller to pair with your ESP32:  
  `sudo sixpair YOUR_ESP32_MAC_ADDR`  

---

## For Windows Users

- You can use the **SixaxisPairTool** to set your PS3 controller's master Bluetooth address.  
- Download it here:  
  [https://www.filehorse.com/download-sixaxispairtool/](https://www.filehorse.com/download-sixaxispairtool/)  
- Run the tool, connect your PS3 controller via USB, and set the master MAC address to your ESP32’s Bluetooth MAC.  
- After that, connect your PS3 controller wirelessly to the ESP32.

---

## Important Note About Master MAC Address

- The PS3 controller stores the master Bluetooth MAC address internally after you run `sixpair`.  
- This MAC address must be a **valid unicast address** (the least significant bit of the first byte must be 0).  
- Interestingly, this MAC address **does not have to match the actual MAC address of your ESP32**.  
- For example, using a random valid unicast MAC like `12:37:AC:4D:B2:E0` works fine if you set it on both the controller (with sixpair) and in your ESP32 code.  
- The ESP32 library often does not strictly verify that the MAC matches the device's hardware MAC, as long as the PS3 controller “thinks” it is connecting to its master.  
- This flexibility lets you standardize the master MAC address in your code without worrying about the actual ESP32 Bluetooth MAC.

---

## Examples of Master MAC Address Usage

### Example 1: Using your ESP32’s actual Bluetooth MAC address

1. Find your ESP32 Bluetooth MAC address (run this on ESP32):

    ```cpp
    #include <esp_system.h>

    void setup() {
      Serial.begin(115200);
      while (!Serial) {}
      uint8_t mac[6];
      esp_read_mac(mac, ESP_MAC_BT);
      delay(3000);
      Serial.printf("ESP32 Bluetooth MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }

    void loop() {}
    ```

2. Use the printed MAC in your `sixpair` command:

    ```
    sudo sixpair XX:XX:XX:XX:XX:XX
    ```

3. Use the same MAC in your ESP32 code:

    ```cpp
    String Master_mac = "XX:XX:XX:XX:XX:XX";  // Replace with your ESP32 MAC
    ```

---

### Example 2: Using a custom random valid MAC address

1. Pick a random unicast MAC address (first byte LSB = 0), e.g.:

    ```
    12:37:AC:4D:B2:E0
    ```

2. Set this MAC on the PS3 controller:

    ```
    sudo sixpair 12:37:AC:4D:B2:E0
    ```

3. Use the same MAC in your ESP32 code:

    ```cpp
    String Master_mac = "12:37:AC:4D:B2:E0";
    ```

---

## Test code 

```cpp 
#include <Arduino.h>
#include <Ps3Controller.h>

String Master_mac = "12:37:AC:4D:B2:E0";  // Or your ESP32 MAC here

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
    // nothing here
}
```

###  Quick Note: PS3 Controller Wireless Setup for Gaming on Linux
1. Connect the controller via USB and run:
   ```bash
   sudo sixpair
   ```

2. Edit the Bluetooth input config:
   ```bash
   sudo nano /etc/bluetooth/input.conf
   ```
   Set:
   ```ini
   ClassicBondedOnly=false
   ```

3. Restart Bluetooth:
   ```bash
   sudo systemctl restart bluetooth
   ```

> Requires BlueZ and Sixpair to be installed

