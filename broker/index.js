const mosca = require('mosca')

const pubSubSettings = {
    type: 'mongo',
    url: 'mongodb://localhost:27017/mqtt',
    pubsubCollection: 'ascoltatori',
    mongo: {}
}

const moscaSettings = {
    port: 1883,
    backend: pubSubSettings
}

const server = new mosca.Server(moscaSettings)

server.on('ready', setup)

function setup() {
    console.log("Mosca server is up and running")
    server.on('published', logPublishingOfAMessage)
    server.on('clientConnected', handleClientConnection)
    server.on('clientDisconnected', handleClientDisconnect)
}

function handleClientConnection(client) {
    console.log("client connected:", client.id)

    spamClient()

}

function handleClientDisconnect(client) {
    console.log("client disconnected", client.id)
}

function logPublishingOfAMessage(packet, client) {
    console.log("published a message:")
    console.log("published", packet)
    console.log("client", client)

}

function spamClient() {
    setInterval(sendLedBlinkMessage, 5000)
}

function sendLedBlinkMessage() {
    console.log('sending blink message')
    const ledBlinkMessage = {
        topic: '/blink/led',
        payload: 'blink that led!',
        qos: 0,
        retain: false
    }
    server.publish(ledBlinkMessage, () => {
        console.log("Published LED blink message. Cross your fingers!")
    })

}