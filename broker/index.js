const brokerSetup = require('./server-setups/setupBroker')
const socketioSetup = require('./server-setups/setupSocketio')

const io = socketioSetup()
const server = brokerSetup()

// * ============================================== * //
// * Socket.io connections for nativescript app

io.on("connection", function(socket) {
    console.log('got a connection')
    console.log("a user connected")

    io.emit("test", "Connected to socket server.")

    socket.on('disconnect', () => {
        console.log('user disconnected')
    })

    socket.on('blink-led', () => {
        console.log('sending blink')
        sendLedBlinkMessage()
        io.to(socket.id).emit('response', 'blink sent to feather')
    })
})
// * ============================================== * //


// ? ============================================== ? //
// ? MQTT connections for everything else
server.on("ready", setup)


function setup() {
    console.log("server running")
    server.on("clientConnected", client => {
        console.log("client connected")
        // spamClient()
    })
    server.on("clientDisconnected", clientDisconnected)
    server.on("published", handlePublishedMessage)
}

function clientDisconnected(client) {
    console.log("client disconnected")
}

function handlePublishedMessage(packet, client) {
    if (client) {
        console.log(`the client ${client.id} published the message: `)
    } else {
        console.log("the following packet was published without a client:")
    }
    console.log(packet.topic)

    if (packet.topic === "/relayto/feather") {
        relayToClient(client)
        io.emit('relay', packet)
    }
}

function relayToClient(client) {
    console.log("relaying to feather!")
    const message = {
        topic: "/message",
        payload: `hi from client: ${client.id}`,
        qos: 2,
        retain: true
    }

    server.publish(message, () => {
        console.log("relayed message to feather")
    })
}

function spamClient() {
    setInterval(sendLedBlinkMessage, 5000)
    setInterval(demoMessage, 500)
}

function demoMessage() {
    const message = {
        topic: "/mqtt/demo",
        payload: "abcde", // or a Buffer
        qos: 2, // 0, 1, or 2
        retain: true // or true
    }

    server.publish(message, () => {
        console.log("message published to client")
    })
}

function sendLedBlinkMessage() {
    console.log("sending blink message")
    const ledBlinkMessage = {
        topic: "/blink/led",
        payload: "blink that led!",
        qos: 0,
        retain: false
    }
    server.publish(ledBlinkMessage, () => {
        console.log("Published LED blink message. Cross your fingers!")
    })
}
// ? ============================================== ? //