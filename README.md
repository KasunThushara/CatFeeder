# CatFeeder
![nodered mqqt](https://github.com/KasunThushara/CatFeeder/assets/73091563/d787e6ca-3eb0-4cd0-9fed-ba89c885e8f1)


## Circuit Diagram

![nodered-esp32](https://github.com/KasunThushara/CatFeeder/assets/73091563/a1701b72-9072-4507-8985-667114172464)


- Servo pin ------ >> 12
- Trig pin of ultrasonic sensor----- >> 5
- Echo pin of ultrasonic sensor----- >> 18
- Signal pin of PIR sensor ------ >> 4

## Steps

### Prepare your Raspberry Pi
If you are using the Raspbian operating system on your Raspberry Pi, Node-RED comes pre-installed, so you don't need to manually install it. However, if Node-RED is not pre-installed, you can follow these steps to install it:
[Node-RED Installation Guide for Raspberry Pi](https://nodered.org/docs/getting-started/raspberrypi)

### Install Mosquitto MQTT Broker
Mosquitto is a popular MQTT broker that enables MQTT communication on your Raspberry Pi. Follow these steps to install Mosquitto:

```bash
sudo apt update
sudo apt upgrade
sudo apt install mosquitto
sudo systemctl start mosquitto
sudo systemctl enable mosquitto
sudo systemctl status mosquito
```
### Install Dependencies
Before running your IoT project, you need to install the following Node-RED nodes using the Manage Palette feature:
```
node-red-contrib-tfjs-coco-ssd
node-red-contrib-image-tools
node-red-contrib-camerapi
node-red-dashboard
node-red-contrib-mqtt-broker
```

Here's a brief step-by-step guide on how to install nodes in Node-RED using the Manage Palette feature:

- Click the hamburger menu (three horizontal lines) at the top right corner and select "Manage Palette."

- In the Manage Palette window, go to the "Install" tab.

- Search for the desired node, e.g., "node-red-contrib-mqtt-broker" for MQTT communication.

- Click "Install" next to the node you want to add.

- Once you have installed all the dependencies and set up your circuit, you are ready to run your Raspberry Pi IoT project!

#### Upload esp_pir_distance_mqqt.ino to Esp 32

#### import flows.json file to node red
   


