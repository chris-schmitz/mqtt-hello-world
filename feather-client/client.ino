/*
 * This client is heavily cribbed from Keith Chester's Garden Relay client:
 *
 * https://github.com/hlfshell/garden-relay
 *
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// * Wifi creds and server information
#include "credentials.h"
#include "enumdefs.h"

#define PRINT_OUTPUT true

// * mqtt subscriptions
const char *demoTopic = "/mqtt/demo";
const char *testSubscriptionTopic = "/blink/led";
const char *messageFromBroker = "/message";

// * hardware pin defs
#define LED_PIN 0

#define RGB_R 14
#define RGB_G 12
#define RGB_B 13

// * getting classy
WiFiClient wifi;
PubSubClient client(wifi);

void blinkLED()
{
    Serial.println("blink led function fired");
    digitalWrite(LED_PIN, HIGH);
    delay(900);
    digitalWrite(LED_PIN, LOW);
    delay(300);
    digitalWrite(LED_PIN, HIGH);
    delay(900);
    digitalWrite(LED_PIN, LOW);
    delay(300);
}

void sayHi()
{
    Serial.println("Oh hey!!");
}

void connectToWifi()
{
    Serial.println();
    Serial.println();
    Serial.print("Connecting to: ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("Wifi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void connectToMqtt()
{
    Serial.println("Connecting to mqtt");
    delay(2000);

    // * Loop until we're connected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection ...");

        if (client.connect("FeatherHuzzahClient"))
        {
            Serial.println("Connected to MQTT :D");
            client.subscribe(testSubscriptionTopic);
            client.subscribe(demoTopic);
            client.subscribe(messageFromBroker);
        }
        else
        {
            Serial.print("Failed. State=");
            Serial.print(client.state());
            Serial.println(". Trying again in 5 seconds...");
            delay(5000);
            Serial.println("Alright, let's try that again.");
            delay(2000);
        }
    }
}

void handleMQTTMessage(char *topic, byte *payload, unsigned int length)
{
    Serial.println("We got a message from MQTT broker!!");
    Serial.print(">> [");
    Serial.print(topic);
    Serial.print("] - ");

    // * parse command from payload
    char command[length];

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
        command[i] = (char)payload[i];
    }
    Serial.print(", Command: ");
    Serial.println(command);
    Serial.println();

    if (strcmp(topic, testSubscriptionTopic) == 0)
    {
        // * parse payload if you need to
        blinkLED();
    }

    if (strcmp(topic, demoTopic) == 0)
    {
        Serial.println("Got the demo message");
        // * parse payload if you need to
        sayHi();
    }

    if (strcmp(topic, messageFromBroker) == 0)
    {
        Serial.println("Got a message from the broker!!!");
        Serial.println(command);
        blinkLED();
    }
}

void setup()
{
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);

    digitalWrite(LED_PIN, LOW);

    delay(100);
    connectToWifi();
    client.setServer(SERVER_HOST, MQTT_PORT);
    Serial.println("created client server. setting callback");
    client.setCallback(handleMQTTMessage);

    // * RGB LED
    pinMode(RGB_R, OUTPUT);
    pinMode(RGB_G, OUTPUT);
    pinMode(RGB_B, OUTPUT);
    digitalWrite(RGB_R, HIGH);
    digitalWrite(RGB_G, HIGH);
    digitalWrite(RGB_B, HIGH);
}

int value = 0;

void loop()
{
    // setRGBLED(255, 255, 255);
    // delay(2000);
    setRGBLED(red);
    delay(2000);
    setRGBLED(green);
    delay(2000);
    setRGBLED(blue);
    delay(2000);
    setRGBLED();
    delay(2000);
    return;

    // if (!client.connected())
    // {
    //     connectToMqtt();
    // }
    // client.loop();
}

void setRGBLED(Color color)
{
    switch (color)
    {
    case red:
        digitalWrite(RGB_R, LOW);
        digitalWrite(RGB_G, HIGH);
        digitalWrite(RGB_B, HIGH);
        break;
    case green:
        digitalWrite(RGB_R, HIGH);
        digitalWrite(RGB_G, LOW);
        digitalWrite(RGB_B, HIGH);
        break;
    case blue:
        digitalWrite(RGB_R, HIGH);
        digitalWrite(RGB_G, HIGH);
        digitalWrite(RGB_B, LOW);
        break;
    }
}

void setRGBLED(uint8_t r, uint8_t g, uint8_t b)
{
    digitalWrite(RGB_R, LOW);
    digitalWrite(RGB_G, LOW);
    digitalWrite(RGB_B, LOW);
}

void setRGBLED()
{
    digitalWrite(RGB_R, HIGH);
    digitalWrite(RGB_G, HIGH);
    digitalWrite(RGB_B, HIGH);
}
