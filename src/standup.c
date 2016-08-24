#include <pebble.h>
#include "comms.h"
#include "views/loadingScreen.h"
#include "views/story.h"

static void init(void) {
  // show the loading screen
  loadingScreen_show();

  comms_init();
  //IF CONFIGURED
  // requestMyStories();
  // when request returns, create a stories view

  // testing for story view
  // struct Story story = {
  //   .title = "Main sheet jib topmast Brethren of the Coast knave boom holystone Jack Ketch Corsair execution dock. Mutiny take a caulk bilge rat ballast schooner coffer jib rutters heave to squiffy. Stern cable dance the hempen jig Admiral of the Black tender heave down chase guns keel gunwalls line.",
  //   .type = bug_type
  // };

  // struct Story stories[5];
  // stories[0] = story;
  //
  // stories_view(stories);
}

static void deinit(void) {
  app_message_deregister_callbacks();
  // window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing.");

  app_event_loop();
  deinit();
}
