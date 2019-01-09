<template>
  <Page>
    <ActionBar title="MQTT Controller" android:flat="true"/>
    <TabView
      android:tabBackgroundColor="#53ba82"
      android:tabTextColor="#c4ffdf"
      android:selectedTabTextColor="#ffffff"
      androidSelectedTabHighlightColor="#ffffff"
      :selectedIndex="activeTabIndex"
      @selectedIndexChange="handleTabChange"
    >
      <TabViewItem title="Color Picker">
        <Picker/>
      </TabViewItem>
      <TabViewItem title="Settings">
        <Settings/>
      </TabViewItem>
      <!-- <TabViewItem title="Hello">
        <Control/>
      </TabViewItem>-->
    </TabView>
  </Page>
</template>

<script>
import Control from "./Control";
import Settings from "./Settings";
import Picker from "./Picker";
import { mapState, mapMutations, mapGetters, mapActions } from "vuex";
import { SOCKET_CLIENT_STATES } from "../store/index";

import * as ApplicationSettings from "application-settings";

export default {
  components: {
    Control,
    Settings,
    Picker
  },
  data() {
    return {};
  },
  computed: {
    ...mapState(["activeTabIndex"]),
    ...mapGetters(["socketioClientState"])
  },
  methods: {
    ...mapMutations(["setActiveTabIndex", "load"]),
    ...mapActions(["initalizeSocketioClient"]),
    handleTabChange(newIndex) {
      this.setActiveTabIndex(newIndex);
    }
  },
  created() {
    this.load();

    if (this.socketioClientState === SOCKET_CLIENT_STATES.CONFIGURED) {
      this.initalizeSocketioClient();
    }
  },
  mounted() {}
};
</script>

<style lang="scss" scoped>
@import "../../style/variables.scss";

ActionBar {
  background-color: $green;
  color: $white;
}
</style>
