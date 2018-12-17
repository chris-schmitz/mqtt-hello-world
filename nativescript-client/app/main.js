import App from './components/App'
import VueDevtools from 'nativescript-vue-devtools'
import Vue from 'nativescript-vue'

import store from './store/index'

if(TNS_ENV !== 'production') {
  Vue.use(VueDevtools, {host: '192.168.1.8'})
}
// Prints Vue logs when --env.production is *NOT* set while building
Vue.config.silent = (TNS_ENV === 'production')


new Vue({
  render: h => h('frame', [h(App)]),
  store
}).$start()
