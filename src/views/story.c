#include <string.h>
#include <pebble.h>
#include "story.h"
#include "constants.h"

static Layer* story_canvas_layer;
static struct Story story;

static void story_canvas_update_proc(Layer* layer, GContext* ctx) {
  draw_story_icon(layer, ctx, story);
  draw_story_title(layer, ctx, story);
}

// void story_window_update(Layer * layer, GContext * context) {
//   // GRect bounds = layer_get_bounds(layer);
//
//   // render thestory yo
// }
//
static void story_window_load(Window *window) {

//   APP_LOG(APP_LOG_LEVEL_DEBUG, "Drawing story screen");
//   window_set_background_color(story_window, GColorBlack);
//   Layer *window_layer = window_get_root_layer(window);
//   layer_set_update_proc(window_layer, story_window_update);
}
//
static void story_window_unload(Window *window) {
}

void story_show(struct Story sty) {
  story = sty;
  story_window = window_create();
  GColor backgroundColor = window_color_for_story(story);
  window_set_background_color(story_window, backgroundColor);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "story_show");
  // window_set_click_config_provider(story_window, click_config_provider);
  window_set_window_handlers(story_window, (WindowHandlers) {
    .load = story_window_load,
    .unload = story_window_unload,
  });

  GRect bounds = layer_get_bounds(window_get_root_layer(story_window));
  story_canvas_layer = layer_create(bounds);
  layer_set_update_proc(story_canvas_layer, story_canvas_update_proc);
  layer_add_child(window_get_root_layer(story_window), story_canvas_layer);

  const bool animated = true;
  window_stack_push(story_window, animated);
}

GColor window_color_for_story(struct Story story) {
  switch (story.type) {
    case bug_type:
      return PBL_IF_COLOR_ELSE(BUG_RED, GColorLightGray);
    case chore_type:
      return PBL_IF_COLOR_ELSE(CHORE_GRAY, GColorDarkGray);
    case feature_type:
      return PBL_IF_COLOR_ELSE(FEATURE_YELLOW, GColorLightGray);
    case release_type:
      return PBL_IF_COLOR_ELSE(RELEASE_BLUE, GColorDarkGray);
    default:
      return GColorWhite;
  }
}

void draw_story_icon(Layer* layer, GContext* ctx, struct Story story) {
  GRect bounds = layer_get_bounds(layer);
  int radius = 20;
  int xLocation = bounds.size.w/2;
  int yLocation = PBL_IF_ROUND_ELSE(radius * 2, radius + 4);

  switch (story.type) {
    case bug_type:
      story_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_BUG);
      break;
    case chore_type:
      story_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_CHORE);
      break;
    case feature_type:
      story_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_FEATURE);
      break;
    case release_type:
      story_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_RELEASE);
      break;
    default:
      break;
  }

  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, GPoint(xLocation, yLocation), 15);

  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_circle(ctx, GPoint(xLocation, yLocation), 15);

  GRect iconBounds = gbitmap_get_bounds(story_icon_bitmap);
  iconBounds.origin.x = xLocation - iconBounds.size.w / 2;
  iconBounds.origin.y = yLocation - iconBounds.size.h / 2;
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  graphics_draw_bitmap_in_rect(ctx, story_icon_bitmap, iconBounds);
}

void draw_story_title(Layer* layer, GContext* ctx, struct Story story) {
  GRect bounds = layer_get_bounds(layer);
  int xLocation = 0;
  int yLocation = PBL_IF_ROUND_ELSE(bounds.size.h/2 - 20, 40 + 10);

  GRect nameLayerBounds = GRect(xLocation+5, yLocation, bounds.size.w - 10, bounds.size.h - yLocation);
  GRect nameLayerShadowBounds = GRect(xLocation+6, yLocation - 2, bounds.size.w - 10, bounds.size.h - yLocation);

  GColor textColor = GColorBlack;
  story__nameLayer = text_layer_create(nameLayerBounds);

  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, nameLayerShadowBounds, 0, GCornerNone);

  text_layer_set_text_color(story__nameLayer, textColor);
  text_layer_set_background_color(story__nameLayer, GColorWhite);
  text_layer_set_overflow_mode(story__nameLayer, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(story__nameLayer, GTextAlignmentCenter);
  text_layer_enable_screen_text_flow_and_paging(story__nameLayer, 5);
  text_layer_set_text(story__nameLayer, story.title);

  layer_add_child(layer, text_layer_get_layer(story__nameLayer));
}
