/*********************************************************************
  Berlin Mengenlehreuhr Watchface for Pebble
  Copyright (C) 2014 Torsten Wunderlich

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

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

/* For later funny use :-) 
void tap_handler(AccelAxisType axis, int32_t direction) {
  if (axis==ACCEL_AXIS_X) APP_LOG(APP_LOG_LEVEL_DEBUG, "X");
  if (axis==ACCEL_AXIS_Y) APP_LOG(APP_LOG_LEVEL_DEBUG, "Y");
  if (axis==ACCEL_AXIS_Z) APP_LOG(APP_LOG_LEVEL_DEBUG, "Z");  
}
*/

void init_window() {
  my_window = window_create();
  window_set_background_color(my_window, BACKGROUND); //BACKGROUND from berlinclock.h
  window_stack_push(my_window, true);
  //accel_tap_service_subscribe(tap_handler);
}

void init_watch() {
  destroy_splash_layer();
  create_berlin_clock_layer(my_window);
  tick_timer_service_subscribe(SECOND_UNIT, handle_ticks);
}

void handle_deinit() {
  accel_tap_service_unsubscribe();
  tick_timer_service_unsubscribe();
  destroy_berlin_clock_layer();
  window_destroy(my_window);
}