#include <string.h>
#include <pebble.h>

#pragma once

Window *loadingScreen_window;
TextLayer *loadingScreen__textLayer;
TextLayer *loadingScreen__textLayer_shadow;
TextLayer *loadingScreen__textLayer_configure;
// loadingScreen_logoLayer;

void loadingScreen_show();

void loadingScreen_window_update(Layer* layer, GContext* context);
