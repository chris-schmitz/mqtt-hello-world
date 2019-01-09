import io from "socket.io-client/dist/socket.io.js"

class SocketClient {
    constructor(host, port) {
        this.url = `http://${host}:${port}`
        this.client = io(this.url)
        return this.client
    }
}

export default SocketClient
