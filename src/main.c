#include <pebble.h>
#include "splash.h"
#include "berlinclock.h"

#define SPLASHDURATION 2000
  
Window *my_window;

//forward declaration
void handle_init();
void init_window();
void init_watch();
void handle_deinit();

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}

void handle_init() {
  init_window();
  display_splash_layer(my_window);
  app_timer_register(SPLASHDURATION, init_watch, NULL);
}

void init_window() {
  my_window = window_create();
  window_set_background_color(my_window, BACKGROUND); //BACKGROUND from berlinclock.h
  window_stack_push(my_window, true);
}

void init_watch() {
  destroy_splash_layer();
  create_berlin_clock_layer(my_window);
  tick_timer_service_subscribe(SECOND_UNIT, handle_ticks);
}

void handle_deinit() {
  tick_timer_service_unsubscribe();
  destroy_berlin_clock_layer();
  window_destroy(my_window);
}