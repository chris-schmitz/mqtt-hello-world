<template>
  <Page>
    <ActionBar title="MQTT Controller" android:flat="true"/>
    <TabView
      android:tabBackgroundColor="#53ba82"
      android:tabTextColor="#c4ffdf"
      android:selectedTabTextColor="#ffffff"
      androidSelectedTabHighlightColor="#ffffff"
      selectedIndex="0"
    >
      <TabViewItem title="Control">
        <Control/>
      </TabViewItem>
      <TabViewItem title="Settings">
        <Settings/>
      </TabViewItem>
    </TabView>
  </Page>
</template>

<script>
import Control from './Control'
import Settings from './Settings'

export default {
  components: {
    Control,
    Settings
  },
  data() {
    return {
      response: 'test'
    };
  }
};
</script>

<style lang="scss" scoped>
@import "../../style/variables.scss";

ActionBar {
  background-color: $green;
  color: $white;
}

</style>
