#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

Servo myservo;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32

int pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
int servoPin = 12;

const char* ssid = "Dialog 4G";
const char* password = "kasun16855";
const char* mqtt_server = "192.168.8.104"; // Replace with your MQTT broker IP or hostname
const char* mqtt_topic = "motion_distance"; // MQTT topic to publish data

const int inputPin = 4;
const int trigPin = 5;
const int echoPin = 18;
const float SOUND_SPEED = 0.034;
const float CM_TO_INCH = 0.393701;

int pirState = LOW;
int val = 0;
long duration;
float distanceCm;
float distanceInch;

WiFiClient espClient;
PubSubClient client(espClient);



void connectToMQTT() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ArduinoClient")) {
      Serial.println("Connected to MQTT!");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  // Convert the payload to a string
  char msg[length + 1];
  for (int i = 0; i < length; i++) {
    msg[i] = (char)payload[i];
  }
  msg[length] = '\0'; // Null-terminate the string

  Serial.print("Message received: ");
  Serial.println(msg);
  if (strcmp(msg, "true") == 0) {
    servoRotation();
  }

  // Add your custom logic here to process the received message if needed
}

void setup() {
  pinMode(inputPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 500, 2400);
  myservo.write(80);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  // Set the MQTT server
  client.setServer(mqtt_server, 1883); // Port 1883 is the default for MQTT

  // Set the MQTT callback function
  client.setCallback(callback);

  // Connect to MQTT
  connectToMQTT();

  // Subscribe to the "ABC_msg" topic
  client.subscribe("ABC_msg");

}

void loop() {
  val = digitalRead(inputPin);

  if (val == HIGH)
  {
    if (pirState == LOW)
    {
      Serial.println("Motion detected!");
      float dist = distanceMeasure();
      Serial.println(dist);
      pirState = HIGH;

      if (dist < 30) {
        // Connect to MQTT if not already connected
        if (!client.connected()) {
          connectToMQTT();
        }

        // Publish the distance data to the MQTT topic
        char distStr[10];
        snprintf(distStr, sizeof(distStr), "%.2f", dist);
        client.publish(mqtt_topic, distStr);
      }
    }
  }
  else
  {
    if (pirState == HIGH)
    {
      Serial.println("Motion ended!");
      pirState = LOW;
    }
  }

  // Handle incoming MQTT messages
  client.loop();
}

float distanceMeasure() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;
  distanceInch = distanceCm * CM_TO_INCH;

  return distanceCm;
}
void servoRotation() {
  for (pos = 80; pos >= 25; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);    // tell servo to go to position in variable 'pos'
    delay(30);             // waits 15ms for the servo to reach the position
  }

  for (pos = 25; pos <= 80; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);    // tell servo to go to position in variable 'pos'
    delay(30);             // waits 15ms for the servo to reach the position
  }
}
