<template>
  <StackLayout>
    <ActionBar title="Color Picker"/>
    <Button text="Show RGB Picker" @tap="showColorPicker"/>
    <StackLayout orientation="horizontal">
      <Label class="selected-color">
        <FormattedString>
          <Span class="label" text="The selected color is: "/>
          <Span class="value" :style="valueColor" :text="selectedColor"/>
        </FormattedString>
      </Label>
    </StackLayout>
  </StackLayout>
</template>

<script>
import { ColorPicker } from "nativescript-color-picker";
import { Color } from "tns-core-modules/Color";

export default {
  data() {
    return {
      picker: null,
      selectedColor: null
    };
  },
  computed: {
    valueColor() {
      return {
        color: this.selectedColor ? this.selectedColor.hex : ""
      };
    }
  },
  methods: {
    showColorPicker() {
      this.picker
        .show("#3489db", "RGB")
        .then(result => {
          this.selectedColor = new Color(result);
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
