import App from "./components/App"
import VueDevtools from "nativescript-vue-devtools"
import Vue from "nativescript-vue"
import store from "./store/index"

// Vue.registerElement("ColorPicker", () => {
//     return require("nativescript-color-picker").ColorPicker
// })
Vue.registerElement(
    "StarRating",
    () => require("nativescript-star-ratings").StarRating
)

if (TNS_ENV !== "production") {
    Vue.use(VueDevtools, { host: "10.10.11.185" })
}
// Prints Vue logs when --env.production is *NOT* set while building
Vue.config.silent = TNS_ENV === "production"

new Vue({
    render: h => h("frame", [h(App)]),
    store
}).$start()
