<template>
    <GridLayout columns="*" rows="75,auto,auto,auto,auto,auto,auto,auto,auto">

        <Label class="heading" text="Settings" col="0" row="0"/>
        <Button class="button" :class="stateClass" :text="buttonText" @tap="handleButtonClick" row="1" />
        <Label class="label" text="Host:" col="0" row="2" />
        <TextField class="field" :class="stateClass" :editable="!locked" v-model="host" hint="xx.xx.xx.xx or www.whatever.com" row="3"/>
        <Label class="label" text="Websocket Port:" col="0" row="4" />
        <TextField class="field" :class="stateClass" :editable="!locked" v-model="port" hint="xxxx" row="5"/>

        <Label class="label" text="Socket Client State:" col="0" row="6" />
        <Label class="field" :text="socketioClientState" col="0" row="7" />

    </GridLayout>
</template>

<script>
    import {mapState, mapMutations, mapGetters, mapActions} from 'vuex'
    export default {
        data () {
            return {
                locked: true
            }
        },
        computed: {
            ...mapState({sharedHost: 'host', sharedPort: 'port'}),
            ...mapGetters(['fullServerAddress', 'socketioClientState']),
            host: {
                get() {
                    return this.sharedHost
                },
                set(host) {
                    this.setHost(host)
                }
            },
            port: {
                get() {
                    return this.sharedPort
                },
                set(port) {

                    this.setPort(port)
                }
            },
            stateClass() {
                return this.locked ? 'edit' : 'save'
            },
            buttonText() {
                return this.locked ? "Edit" : "Save"
            },

        },
        methods: {
            ...mapMutations(['setHost', 'setPort']),
            ...mapActions(['initalizeSocketioClient']),
            handleButtonClick() {
                if (!this.locked && this.fullServerAddress) {
                    this.initalizeSocketioClient()
                }
                this.locked = !this.locked
            }
        },
    }
</script>

<style lang="scss" scoped>
@import "../../style/variables.scss";

.heading{
    margin: 20px;
    margin-top: 40px;
    font-size: 30;
    color: $black;
    text-align: center;
}
.label {
  margin-top: 50px;
  vertical-align: center;
  text-align: left;
  font-size: 20;
  font-weight: bold;
  color: $darkgray;
}

.button {
    width: 500px;
    margin:20px;
    color:$white;
}

.button.edit{
    background: $orange;
}
.button.save{
    background: $green;
}
.field.edit{
    background: rgba($lightgreen, 0.5);
}
</style>
