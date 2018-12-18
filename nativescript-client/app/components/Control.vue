<template>
    <GridLayout columns="*" rows="50,50,50,*">
        <Label class="label" text="Say hello:" col="0" row="0"/>
        <Button v-if="socketConfigurationNeeded" class="button config-needed" text="Config Needed" @tap="goToSeettingsTab" col="0" row="1"/>
        <Button v-if="!socketConfigurationNeeded" class="button" text="Send Greetings" @tap="sendGreetings" col="0" row="1"/>
        <Label class="label" text="Response:" col="0" row="2"/>
        <Label class="output" :text="response" col="0" row="3" colSpan="2"/>
    </GridLayout>
</template>

<script>
    import {mapState, mapGetters, mapMutations} from 'vuex'
    import {SOCKET_CLIENT_STATES} from '../store/index'

    export default {
        data () {
            return {
            }
        },
        computed: {
            ...mapState({storedActiveTabIndex: 'activeTabIndex', response: 'serverResponse',client: 'socketioClient'}),
            ...mapGetters(['socketioClientState']),
            activeTabIndex: {
                get() {
                    return this.storedActiveTabIndex
                },
                set(index) {
                    this.setActiveTabIndex(index)
                }
            },
            socketConfigurationNeeded() {
                return this.socketioClientState === SOCKET_CLIENT_STATES.UNCONFIGURED ? true : false
            }
        },
        methods: {
            ...mapMutations(['setActiveTabIndex']),
            sendGreetings () {
                this.client.emit('blink-led')
            },
            goToSeettingsTab() {
                this.setActiveTabIndex(1)
            }
        }
    }
</script>

<style lang='scss' scoped>
@import "../../style/variables.scss";

.button {
  width: 500px;
  background-color: $green;

    &.config-needed {
        background-color: $gray;
    }
}

.label {
  margin-top: 50px;
  vertical-align: center;
  text-align: center;
  font-size: 20;
  color: $darkgray;
  font-weight:bold;
}

.output {
  background: $lightgray;
  border: 1px solid $gray;
  color: $black;
  margin: 50px;
  padding: 20px;
}
</style>
