import Vue from "nativescript-vue"
import Vuex from "vuex"
import moment from "moment"

import io from "socket.io-client/dist/socket.io.js"

Vue.use(Vuex)

const SOCKET_CLIENT_STATES = {
    UNCONFIGURED: "unconfigured",
    CONFIGURED: "configured",
    CONNECTED: "connected"
}

export { SOCKET_CLIENT_STATES }

const state = {
    host: "192.168.1.8",
    // host: "10.10.11.185",
    port: 3001,
    activeTabIndex: 2,
    socketioClient: null,
    serverResponse: null
}

const mutations = {
    setHost(state, value) {
        state.host = value
    },
    setPort(state, value) {
        state.port = value
    },
    setActiveTabIndex(state, value) {
        state.activeTabIndex = value
    },
    setSocketioClient(state, value) {
        state.socketioClient = value
    },
    setServerResponse(state, value) {
        state.serverResponse = value
    }
}

const actions = {
    initalizeSocketioClient({ commit, state }) {
        if (!state.host && !state.port) {
            console.log("Cannot initialize socket io yet.")
            console.log("We don't have all of the configurations:")
            console.log({ host: state.host, port: state.port })
            return
        }

        const url = `http://${state.host}:${state.port}`
        const client = io(url)

        client.on("connect", () => {
            console.log("connected to socket server")
        })

        client.on("test", data => {
            state.serverResponse = data
        })

        client.on("response", data => {
            const stamped = moment().format("hh:mm:ss") + ": " + data
            const newResponse = state.serverResponse + "\n" + stamped
            console.log(newResponse)
            commit("setServerResponse", newResponse)
        })

        commit("setSocketioClient", client)
    }
}

const getters = {
    fullServerAddress(state) {
        if (state.host && state.port) {
            return `http://${state.host}:${state.port}`
        }
        return null
    },

    socketioClientState(state, getters) {
        if (getters.fullServerAddress && state.socketioClient) {
            return SOCKET_CLIENT_STATES.CONNECTED
        }

        if (getters.fullServerAddress && !state.socketioClient) {
            return SOCKET_CLIENT_STATES.CONFIGURED
        }

        return SOCKET_CLIENT_STATES.UNCONFIGURED
    }
}

const store = new Vuex.Store({ state, mutations, getters, actions })

export default store
