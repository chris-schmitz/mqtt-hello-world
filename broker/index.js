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

    // const testMessage = {
    //     topic: '/hello/world',
    //     payload: 'hey there',
    //     qos: 0,
    //     retain: false
    // }

    // server.publish(testMessage, () => {
    //     console.log('done')
    // })

    server.on('published', (packet, client) => {
        console.log("published a message:")
        console.log("published", packet)
        console.log("client", client)
    })

    server.on('clientConnected', client => {
        console.log("client connected:", client.id)

        const ledBlinkMessage = {
            topic: '/blink/led',
            payload: 'blink that led!',
            qos: 0,
            retain: false
        }
        server.publish(ledBlinkMessage, () => {
            console.log("Published LED blink message. Cross your fingers!")
        })
    })

    server.on('clientDisconnected', client => {
        console.log("client disconnected", client.id)
    })
}

/*
const express = require('express')
const app = express()
const port = 3000
const path = require('path')

app.use(express.static(path.join(__dirname, 'public')))

app.get('/', (req, res) => res.send('Hello World!'))

app.listen(port, () => console.log(`Example app listening on port ${port}!`))
*/