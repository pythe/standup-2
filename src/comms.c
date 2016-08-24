#include <pebble.h>
#include "comms.h"
#include "views/story.h"

struct Story *stories = NULL;
int storiesCount = 0;

static void appMessageInboxReceived(DictionaryIterator *iterator, void *context) {
  Tuple *completeTuple = dict_find(iterator, MESSAGE_KEY_complete);
  Tuple *countTuple = dict_find(iterator, MESSAGE_KEY_count);

  if (completeTuple){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Completed receiving stories, time to display them.");
  } else if (countTuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Allocating stories array[%d]", countTuple->value->uint8);
    stories = malloc(countTuple->value->uint8 * sizeof(struct Story *));
  } else {
    if (stories == NULL) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Received a story without first receiving the count!");
      return;
    }
    Tuple *tuple;
    // int id;
    // int state;
    // int type;
    // char* name;

    while((tuple = dict_read_next(iterator))) {
      if (tuple->key == MESSAGE_KEY_id) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Received an ID %d", tuple->value->uint8);
        // id = tuple->value->uint8;
      } else if (tuple->key == MESSAGE_KEY_name) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Received a name %s", tuple->value->cstring);
      } else if (tuple->key == MESSAGE_KEY_state) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Received a state %d", tuple->value->uint8);
      } else if (tuple->key == MESSAGE_KEY_type) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Received a type %d", tuple->value->uint8);
      }
    }
  }
}

static void appMessageInboxDropped(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Inbox dropped with reason %d", reason);
}

void comms_init() {
  app_message_register_inbox_received(appMessageInboxReceived);
  app_message_register_inbox_dropped(appMessageInboxDropped);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}
