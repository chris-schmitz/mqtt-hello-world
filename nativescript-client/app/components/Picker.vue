<template>
  <FlexboxLayout
    class="picker-container"
    flexDirection="column"
    justifyContent="flex-start"
    alignItems="center"
  >
    <Label class="swatch" :style="backgroundColor" flexGrow="3"/>

    <StackLayout orientation="horizontal" flexGrow="1">
      <Label class="selected-color">
        <FormattedString>
          <Span class="label" text="The selected color is: "/>
          <Span class="value" :style="valueColor" :text="selectedColor.hex"/>
        </FormattedString>
      </Label>
    </StackLayout>
    <StackLayout orientation="horizontal" flexGrow="1">
      <Label class="selected-color">
        <FormattedString>
          <Span class="label" text="The active LED is: "/>
          <Span class="value" :style="valueColor" :text="activeLed"/>
        </FormattedString>
      </Label>
    </StackLayout>
    <Label flexGrow="1"/>
    <Button text="Show RGB Picker" @tap="showColorPicker" class="button"/>
    <Label text="Set color for LED:"/>
    <FlexboxLayout flexWrap="wrap">
      <Button
        v-for="(state, index) in ledStates"
        :key="index"
        :text="index + 1"
        :class="{'active-led-button': activeLed == index + 1}"
        :style="getLedButtonColor(index + 1)"
        @tap="setActiveLed(index + 1)"
      />
    </FlexboxLayout>
    <Button
      text="All"
      :class="{'active-led-button': activeLed == 'all'}"
      @tap="setActiveLed('all')"
    />
  </FlexboxLayout>
</template>

<script>
import { ColorPicker } from "nativescript-color-picker";
import { Color } from "tns-core-modules/Color";
import { mapState, mapMutations, mapGetters } from "vuex";

export default {
  data() {
    return {
      picker: null,
      selectedColor: new Color("black"),
      ledStates: [],
      activeLed: 1,
      swatchColor: new Color("purple")
    };
  },
  computed: {
    ...mapState({ client: "socketioClient" }),
    ...mapGetters(["fullServerAddress"]),
    valueColor() {
      return {
        color: this.selectedColor ? this.selectedColor.hex : ""
      };
    },
    backgroundColor() {
      return {
        background: this.selectedColor ? this.selectedColor.hex : ""
      };
    }
  },
  methods: {
    setActiveLed(identifier) {
      this.activeLed = identifier;
      if (identifier !== "all") {
        this.selectedColor = this.ledStates[identifier - 1];
      } else {
        this.selectedColor = this.ledStates[0];
      }
    },
    showColorPicker() {
      let color = "#000000";
      if ("hex" in this.selectedColor) {
        color = this.selectedColor.hex;
      }
      this.picker
        .show(color, "RGB")
        .then(result => {
          this.selectedColor = new Color(result);
          this.swatchColor = { background: this.selectedColor.hex };

          if (this.activeLed !== "all") {
            this.ledStates[this.activeLed - 1] = this.selectedColor;
          } else {
            this.ledStates = this.ledStates.map(ledColor => {
              return this.selectedColor;
            });
          }

          this.client.emit("set-strip-color", {
            color: {
              r: this.selectedColor.r,
              g: this.selectedColor.g,
              b: this.selectedColor.b
            }
          });
        })
        .catch(err => {
          console.log(err);
        });
    },
    getStripColors() {
      fetch(`${this.fullServerAddress}/strip/colors`)
        .then(response => {
          console.log("got strip color info");
          return response.json();
        })
        .then(json => {
          this.setLedStates(json.leds);
        })
        .catch(error => {
          console.error(error);
        });
    },
    setLedStates(states) {
      const convertedStates = states.map(rgbValues => {
        // ^ Note we could use the alpha channel to control brightness, though I think that's on a strip
        // ^ level instead of the individual LED so maybe not :/
        return new Color("100", rgbValues.r, rgbValues.g, rgbValues.b);
      });
      console.log(convertedStates);
      this.ledStates = convertedStates;
    },
    getLedButtonColor(buttonNumber) {
      if (this.ledStates.length === 0) return;

      return { background: this.ledStates[buttonNumber - 1].hex };
    }
  },
  created() {
    this.picker = new ColorPicker();
    this.getStripColors();
  }
};
</script>

<style lang='scss' scoped>
@import "../../style/variables.scss";

.picker-container {
  .button {
    margin-bottom: 30px;
  }
  .swatch {
    // height: 500px;
    margin: 30px;
    width: 90%;
  }
  .selected-color {
    .value {
      font-weight: bold;
      // font-size: 50px;
    }
  }

  .active-led-button {
    // background: $green !important;
    color: $white;
    font-weight: bold;
  }
}
</style>
