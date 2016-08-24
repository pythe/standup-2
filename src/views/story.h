#include <string.h>
#include <pebble.h>

#pragma once

Window *story_window;
TextLayer *story__nameLayer;
GBitmap* story_icon_bitmap;

enum Story_Type {
  bug_type,
  chore_type,
  feature_type,
  release_type
};

enum Story_Status {
  unstarted,
  started,
  finished,
  delivered
};

struct Story {
  int id;
  char * name;
  enum Story_Type type;
  enum Story_Status status;
};

GColor window_color_for_story(struct Story);
void draw_story_icon(Layer *, GContext *, struct Story);
void draw_story_title(Layer *, GContext *, struct Story);

void story_show(struct Story);

// void story_window_update(Layer* layer, GContext* context);
