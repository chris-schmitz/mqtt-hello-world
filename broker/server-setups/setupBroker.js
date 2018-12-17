var mosca = require("mosca")

function setup() {
    var server = new mosca.Server({
        http: {
            port: 3000,
            bundle: true,
            static: "./"
        }
    })
    return server
}

module.exports = setup
