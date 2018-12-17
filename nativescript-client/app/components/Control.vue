<template>
    <GridLayout columns="*" rows="50,50,50,*">
        <Label class="label" text="Say hello:" col="0" row="0"/>
        <Button class="button" text="Send Greetings" @tap="sendGreetings" col="0" row="1"/>
        <Label class="label" text="Response:" col="0" row="2"/>
        <Label class="output" :text="response" col="0" row="3" colSpan="2"/>
    </GridLayout>
</template>

<script>
    import {mapState} from 'vuex'
    import io from 'socket.io-client/dist/socket.io.js'


    export default {
        data () {
            return {
                client:null,
                response: null
            }
        },
        computed: {
            ...mapState(['host', 'port'])
        },
        methods: {
            sendGreetings () {

            }
        },
        created() {
            this.client = io('http://192.168.1.8:3001')

            this.client.on('connect', () => {
                })
            this.client.on('test', (data) => {
                // this.response = JSON.stringify(data)
            })

            this.client.on('relay', (data) => {
                this.response = JSON.stringify(data)
            })
        }
    }
</script>

<style lang='scss' scoped>
@import "../../style/variables.scss";

.button {
  width: 500px;
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
