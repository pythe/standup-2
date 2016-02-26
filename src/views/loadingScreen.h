#include <string.h>
#include <pebble.h>

#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

Window *loadingScreen_window;
TextLayer *loadingScreen__textLayer;
TextLayer *loadingScreen__textLayer_shadow;
TextLayer *loadingScreen__textLayer_configure;
// loadingScreen_logoLayer;

void loadingScreen_show();

void loadingScreen_window_update(Layer* layer, GContext* context);

#endif // LOADING_SCREEN_H
