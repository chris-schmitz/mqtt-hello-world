#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// * Wifi creds and server information
#include "credentials.h"

#define PRINT_OUTPUT true

// * mqtt subscriptions
const char *testSubscriptionTopic = "/blink/led";

// * hardware pin defs
#define LED_PIN 0

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
}

int value = 0;

void loop()
{
    if (!client.connected())
    {
        connectToMqtt();
    }
    client.loop();
}
