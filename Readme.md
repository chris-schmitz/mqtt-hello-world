# Hello MQTT

This is a hello-world project I built to explore internet of things communication via MQTT.

This is _def_ still a work in progress at the moment (as of 2018-12-04).

## Setup

To get up and running:

```bash
# From cloned repository folder

# create and fill in the credentials header for the client
cd client
cp example.credentials.h credentials.h
# Then open the new `credentials.h` file and fill in your wifi and server info


# install the broker dependencies
cd ../broker
npm install
node index.js # this will start the broker server
```
