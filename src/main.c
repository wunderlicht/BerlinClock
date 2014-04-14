#include <pebble.h>

Window *my_window;
Layer *root_layer;
Layer *hours_layer;
Layer *minutes_layer;
Layer *seconds_layer;
struct tm *now; //obj-c is so beautiful... this is the only chance to retain time throughout all draws.

//forward declaration as learned in university :-)
void handle_init(void);
void handle_deinint(void);
void handle_ticks(struct tm *tick_time, TimeUnits units_changed);
void draw_seconds();
void draw_minutes();
void draw_hours();

void handle_init(void) {
  my_window = window_create();
  window_stack_push(my_window, true);
  Layer *root_layer = window_get_root_layer(my_window);
  //get geometry
  GRect bounds = layer_get_frame(root_layer);
  int16_t hight = bounds.size.h;
  int16_t width = bounds.size.w;
  //stack all three layers. every covering a third of the screen
  //hours_layer   = layer_create((GRect){ .origin = { 0, 0 }, .size = bounds.size });
  hours_layer   = layer_create(GRect(0, 0, 0, width));
  minutes_layer = layer_create((GRect){ .origin = { 0, hight/3 }, .size = bounds.size });
  seconds_layer = layer_create((GRect){ .origin = { 0, (hight/3)*2 }, .size = bounds.size });
  
  //put layers into root layer
  layer_add_child(root_layer, hours_layer);
  layer_add_child(root_layer, minutes_layer);
  layer_add_child(root_layer, seconds_layer);
  
  //setup the handlers
  tick_timer_service_subscribe(SECOND_UNIT, handle_ticks);
}

void handle_deinit(void) {
  layer_destroy(hours_layer);
  layer_destroy(minutes_layer);
  layer_destroy(seconds_layer);
  window_destroy(my_window);
}

void handle_ticks(struct tm *tick_time, TimeUnits units_changed) {
  //I think there is more in the unit_changed parameter. Maybe I don't need to invalidate all layers.
  now = tick_time;
  layer_mark_dirty(seconds_layer);
  layer_mark_dirty(minutes_layer);
  layer_mark_dirty(hours_layer);  
}

void draw_seconds() {
  static bool ticktock = false;
  
}

void draw_minutes() {
}

void draw_hours() {
}


int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
