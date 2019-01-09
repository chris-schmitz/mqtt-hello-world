var app = require("express")()
var http = require("http").Server(app)
var io = require("socket.io")(http)

function setup() {
    console.log("setting up socket.io")
    http.listen(3001, function() {
        console.log("listening on *:3001")
    })

    return { io, app }
}

module.exports = setup
