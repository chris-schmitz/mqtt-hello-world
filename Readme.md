# Hello MQTT

This is a hello-world project I built to explore internet of things communication via MQTT.

This is _def_ still a work in progress at the moment (as of 2018-12-04).

![](readme_attachments/demo.gif)

## What is this?!
I've been working on learning a couple of concepts and tools; MQTT communication, using the adafruit huzzah, building native mobile apps with nativescript.

This is my hello-world app for all of those slapped together :)

The node server on my laptop is running the Mosca MQTT broker and Socket.io. The mqtt.js client won't run in nativescript so I had to use Socket.io to handle the nativescript client communication. Most of the client communication is handled by Mosca and any requests coming in from Socket.io call the Mosca functions.

The nativescript mobile client is built with nativescript-vue and has a socket.io client added in.

The Adafruit feather huzzah is using a custom pubsub setup that closely follows a project by my friend Keith Chester.

The main magic bullet through the entire setup is for the mobile app to message to the feather to run a function to blink the onboard LED.

Now that I have this down I'll be building out more of the functionality and after the holidays I'll be starting on my next personal project that will heavily leverage this tech. I'm _very_ excited :D


## Setup

To get up and running:

```bash
# From cloned repository folder

# 1. Set up your broker server
# You can run this from your laptop or something like a raspberry pi connected to the same wifi as the clients
# Note that the IP address of this server is what you'll need to add to the clients as the host address
cd ../broker
npm install
npm start # this will start the broker server

# 2. Set up the feather client
# create and fill in the credentials header for the client
cd feather-client
cp example.credentials.h credentials.h
# Then open the new `credentials.h` file and fill in your wifi and server info
# Once your credentials have been added, upload the `feather-client/client.ino` to your adafruit feather huzzah

# 3. Setup the nativescript client
# If you want to load this on your physical phone you'll need to have that phone plugged into your computer
# and have the developer tools enabled on the phone
cd nativescript-client
npm install
npm run dev:android
# or
npm run dev:android
# to open the vue dev tools:
npm run open:devtools
```
