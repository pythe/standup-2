#include <pebble.h>
#include "comms.h"
#include "views/loadingScreen.h"

static void appMessageInboxReceived(DictionaryIterator *iterator, void *context) {

}

static void appMessageOutboxFailed(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox failed with reason %d", reason);
}

static void appMessageInboxDropped(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Inbox dropped with reason %d", reason);
}

static void init(void) {
  // show the loading screen
  loadingScreen_show();

  app_message_register_inbox_received(appMessageInboxReceived);
  app_message_register_inbox_dropped(appMessageInboxDropped);
  // app_message_register_outbox_sent(appMessageOutboxSent);
  app_message_register_outbox_failed(appMessageOutboxFailed);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  // make web request
  //IF CONFIGURED
  requestMyStories();
  // when request returns, create a stories view

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