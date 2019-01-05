<template>
  <StackLayout>
    <ActionBar title="Color Picker"/>
    <Button text="Show RGB Picker" @tap="showColorPicker"/>
    <StackLayout orientation="horizontal">
      <Label text="Selected Color: " class="message" textWrap="true"/>
      <Label :text="selectedColor" class="message" textWrap="true"/>
    </StackLayout>
  </StackLayout>
</template>

<script>
import { ColorPicker } from "nativescript-color-picker";

export default {
  data() {
    return {
      picker: null,
      selectedColor: null
    };
  },
  methods: {
    showColorPicker() {
      console.log(this.picker.show);
      this.picker
        .show("#3489db", "RGB")
        .then(result => {
          console.log("color int: " + result);
          this.selectedColor = result;
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
.picker-container {
}
</style>
