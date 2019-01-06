<template>
  <StackLayout>
    <ActionBar title="Color Picker"/>
    <Button text="Show RGB Picker" @tap="showColorPicker"/>
    <StackLayout orientation="horizontal">
      <Label class="selected-color">
        <FormattedString>
          <Span class="label" text="The selected color is: "/>
          <Span class="value" :style="valueColor" :text="selectedColor.hex"/>
        </FormattedString>
      </Label>
    </StackLayout>
  </StackLayout>
</template>

<script>
import { ColorPicker } from "nativescript-color-picker";
import { Color } from "tns-core-modules/Color";
import { mapState } from "vuex";

export default {
  data() {
    return {
      picker: null,
      selectedColor: { hex: "#00FF00" }
    };
  },
  computed: {
    ...mapState({ client: "socketioClient" }),
    valueColor() {
      return {
        color: this.selectedColor ? this.selectedColor.hex : ""
      };
    }
  },
  methods: {
    showColorPicker() {
      this.picker
        .show(this.selectedColor.hex, "RGB")
        .then(result => {
          this.selectedColor = new Color(result);
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
    }
  },
  created() {
    this.picker = new ColorPicker();
  }
};
</script>

<style lang='scss' scoped>
.selected-color {
  .value {
    font-weight: bold;
    font-size: 50px;
  }
}
</style>
