import Vue from 'nativescript-vue'
import Vuex from 'vuex'

Vue.use(Vuex)

const state = {
    host: null,
    port: null
}

const mutations = {
    setHost(state, value) {
        state.host = value
    },
    setPort(state, value) {
        state.port = value
    }
}

const actions = {

}

const getters = {

}

const store = new Vuex.Store({ state, mutations,getters,actions })

export default store