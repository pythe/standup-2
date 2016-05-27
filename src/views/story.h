#include <string.h>
#include <pebble.h>

#ifndef STORY_H
#define STORY_H

Window *story_window;
TextLayer *story__nameLayer;
GBitmap* story_icon_bitmap;
// loadingScreen_logoLayer;

enum Story_Type {
  bug_type,
  chore_type,
  feature_type,
  release_type
};

struct Story {
  char * title;
  enum Story_Type type;
};

GColor window_color_for_story(struct Story);
void draw_story_icon(Layer *, GContext *, struct Story);
void draw_story_title(Layer *, GContext *, struct Story);

void story_show(struct Story);

// void story_window_update(Layer* layer, GContext* context);

#endif // STORY_H
