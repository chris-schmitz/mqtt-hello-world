/*
 * This client is heavily cribbed from Keith Chester's Garden Relay client:
 *
 * https://github.com/hlfshell/garden-relay
 *
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

// * Wifi creds and server information
#include "credentials.h"
#include "enumdefs.h"

#define PRINT_OUTPUT true

// * mqtt subscriptions
const char *demoTopic = "/mqtt/demo";
const char *testSubscriptionTopic = "/blink/led";
const char *messageFromBroker = "/message";
const char *stripColorSetTopic = "/strip/color/set/rgb";
const char *stripColorGetTopic = "/strip/color/get";
const char *stripColorPublishTopic = "/strip/color";

// * hardware pin defs
#define LED_PIN 0

#define LED_STRIP_PIN 14
#define TOTAL_PIXELS 8

// * getting classy
WiFiClient wifi;
PubSubClient client(wifi);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTAL_PIXELS, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

// * And define any remaining tools
uint32_t pixelColors[TOTAL_PIXELS];

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
            client.subscribe(stripColorSetTopic);
            client.subscribe(stripColorGetTopic);
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

    if (strcmp(topic, stripColorSetTopic) == 0)
    {
        Serial.println("got set strip color message");

        char *strtokIndex;
        uint8_t r, g, b;
        int8_t ledIndex;

        strtokIndex = strtok(command, ",");
        r = atoi(strtokIndex);

        strtokIndex = strtok(NULL, ",");
        g = atoi(strtokIndex);

        strtokIndex = strtok(NULL, ",");
        b = atoi(strtokIndex);

        strtokIndex = strtok(NULL, ",");
        ledIndex = atoi(strtokIndex);

        if (ledIndex == -1)
        {
            setStripColor(r, g, b);
        }
        else
        {
            setPixelColor(r, g, b, ledIndex);
        }
    }

    if (strcmp(topic, stripColorGetTopic) == 0)
    {
        char *payload = "";
        payload = intArrayToString(pixelColors, TOTAL_PIXELS, &payload);

        client.publish(stripColorPublishTopic, payload);
    }
}

char intArrayToString(uint32_t intArray[], int arraySize, *payload)
{
    for (int i = 0; i < arraySize; i++)
    {
        payload += itoa(intArray[i]);
    }
    return payload;
}

void signalReady()
{
    for (uint8_t i = 0; i <= TOTAL_PIXELS; i++)
    {
        strip.setPixelColor(i, 0, 255, 0);
        strip.show();
        delay(100);
    }

    for (uint8_t i = 0; i <= TOTAL_PIXELS; i++)
    {
        strip.setPixelColor(i, 0, 0, 0);
        strip.show();
    }
    delay(100);
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

    strip.begin();
    signalReady();
}

int value = 0;

void loop()
{
    // Serial.println("top of loop");

    if (!client.connected())
    {
        Serial.println("try MQTT connect");
        connectToMqtt();
    }
    client.loop();
}

void setStripColor(uint8_t r, uint8_t g, uint8_t b)
{
    for (uint8_t i = 0; i <= TOTAL_PIXELS; i++)
    {
        setPixelColor(i, r, g, b);
    }
    strip.show();
}

void setPixelColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
    strip.setPixelColor(index, r, g, b);
    pixelColors[index] = strip.getPixelColor(index); //? should we do it this way or just set the passed in rgb??
}
