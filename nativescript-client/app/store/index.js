import Vue from "nativescript-vue"
import Vuex from "vuex"
import moment from "moment"
import * as ApplicationSettings from "application-settings"
import stringify from "json-stringify-safe"
import SocketClient from "../tools/socket-client"
import { devOverrides } from "../../config"

let client = null

Vue.use(Vuex)

const SOCKET_CLIENT_STATES = {
    UNCONFIGURED: "unconfigured",
    CONFIGURED: "configured",
    CONNECTED: "connected"
}

export { SOCKET_CLIENT_STATES }

const state = {
    host: null,
    // host: "10.10.11.185",
    port: null,
    // port: 3001,
    activeTabIndex: 0,
    socketioClient: null,
    serverResponse: null
}

const mutations = {
    load(state) {
        if (ApplicationSettings.getString("host")) {
            state.host = ApplicationSettings.getString("host")
        }
        if (ApplicationSettings.getString("port")) {
            state.port = ApplicationSettings.getString("port")
        }
    },
    setHost(state, value) {
        state.host = value
        if (!devOverrides.enabled) {
            ApplicationSettings.setString("host", value)
        }
    },
    setPort(state, value) {
        state.port = value
        if (!devOverrides.enabled) {
            ApplicationSettings.setString("port", value)
        }
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

        client = new SocketClient(state.host, state.port)

        client.on("connect", () => {
            console.log("connected to socket server")
        })

        client.on("test", data => {
            state.serverResponse = data
        })

        client.on("response", data => {
            const stamped = moment().format("hh:mm:ss") + ": " + data
            const newResponse = state.serverResponse + "\n" + stamped
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

if (devOverrides.enabled === true) {
    console.log("using dev defaults")
    store.commit("setHost", devOverrides.socketServer.host)
    store.commit("setPort", devOverrides.socketServer.port)
}

export default store
