#include <string.h>
#include <pebble.h>
#include "loadingScreen.h"
#include "paths.h"
#include "constants.h"

static void loadingScreen_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Drawing splash screen");
  Layer *window_layer = window_get_root_layer(window);
  layer_set_update_proc(window_layer, loadingScreen_window_update);
}

static void loadingScreen_window_unload(Window *window) {
  text_layer_destroy(loadingScreen__textLayer);
  text_layer_destroy(loadingScreen__textLayer_shadow);
}

void drawLogo(Layer * layer, GContext * context) {
  GRect bounds = layer_get_bounds(layer);
  int xLocation = bounds.size.w/2;
  int yLocation = bounds.size.h/2;

  graphics_context_set_fill_color(context, PBL_IF_COLOR_ELSE(BRAND_BLUE, GColorBlack));
  graphics_fill_circle(context, (GPoint) {.x = xLocation, .y = yLocation}, 55);

  graphics_context_set_fill_color(context, GColorWhite);
  graphics_fill_circle(context, (GPoint) {.x = xLocation, .y = yLocation}, 47);

  graphics_context_set_fill_color(context, PBL_IF_COLOR_ELSE(BRAND_ORANGE, GColorBlack));
  GPath *cardPath = gpath_create(&CARD_PATH_INFO);

  for(int i = 0; i < 6; i++) {
    gpath_move_to(cardPath, (GPoint){.x=xLocation, .y=yLocation});
    gpath_rotate_to(cardPath, i*(TRIG_MAX_ANGLE/6));
    gpath_draw_filled(context, cardPath);
  }

  gpath_destroy(cardPath);
}

void loadingScreen_window_update(Layer * layer, GContext * context) {
  GRect bounds = layer_get_bounds(layer);
  int xLocation = bounds.size.w/2;
  int yLocation = bounds.size.h/2;

  drawLogo(layer, context);

  #if defined(PBL_COLOR)
    loadingScreen__textLayer_shadow = text_layer_create((GRect) { .origin = { 1, yLocation-12 }, .size = { bounds.size.w, 28 } });
    text_layer_set_text_color(loadingScreen__textLayer_shadow, GColorWhite);
    text_layer_set_background_color(loadingScreen__textLayer_shadow, GColorClear);
    text_layer_set_font(loadingScreen__textLayer_shadow, fonts_get_system_font("RESOURCE_ID_GOTHIC_28_BOLD"));
    text_layer_set_text(loadingScreen__textLayer_shadow, "STANDUP");
    text_layer_set_text_alignment(loadingScreen__textLayer_shadow, GTextAlignmentCenter);
    layer_add_child(layer, text_layer_get_layer(loadingScreen__textLayer_shadow));
  #endif

  yLocation = PBL_IF_COLOR_ELSE((bounds.size.h/2)-14, bounds.size.h - 32);
  loadingScreen__textLayer = text_layer_create((GRect) { .origin = { 0, yLocation }, .size = { bounds.size.w, 28 } });
  text_layer_set_background_color(loadingScreen__textLayer, GColorClear);
  text_layer_set_font(loadingScreen__textLayer, fonts_get_system_font("RESOURCE_ID_GOTHIC_28_BOLD"));
  text_layer_set_text(loadingScreen__textLayer, "STANDUP");
  text_layer_set_text_alignment(loadingScreen__textLayer, GTextAlignmentCenter);
  layer_add_child(layer, text_layer_get_layer(loadingScreen__textLayer));


  // if not configured
  yLocation = PBL_IF_ROUND_ELSE((bounds.size.h/2)-28, 4);
  loadingScreen__textLayer_configure = text_layer_create((GRect) { .origin = { 0, yLocation }, .size = { bounds.size.w, 20 } });
  text_layer_set_background_color(loadingScreen__textLayer_configure, GColorYellow);
  text_layer_set_font(loadingScreen__textLayer_configure, fonts_get_system_font("RESOURCE_ID_GOTHIC_14_BOLD"));
  text_layer_set_text(loadingScreen__textLayer_configure, "Please sign in on phone.");
  text_layer_set_text_alignment(loadingScreen__textLayer_configure, GTextAlignmentCenter);
  layer_add_child(layer, text_layer_get_layer(loadingScreen__textLayer_configure));
}

// static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
//   APP_LOG(APP_LOG_LEVEL_DEBUG, "SELECT");
// }
//
// static void click_config_provider(void *context) {
//   window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
// }

void loadingScreen_show() {
  loadingScreen_window = window_create();
  // window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(loadingScreen_window, (WindowHandlers) {
    .load = loadingScreen_window_load,
    .unload = loadingScreen_window_unload,
  });
  const bool animated = true;
  window_stack_push(loadingScreen_window, animated);

  return;
}
