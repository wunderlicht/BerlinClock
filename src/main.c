#include <pebble.h>

Window *my_window;
Layer *root_layer;
Layer *hours_layer;
Layer *minutes_layer;
Layer *seconds_layer;
struct tm now; //obj-c is so beautiful... this is the only chance to retain time throughout all draws.

//forward declaration as learned in university :-)
void handle_init(void);
void init_window(void);
void handle_deinint(void);
void handle_ticks(struct tm *tick_time, TimeUnits units_changed);
void draw_seconds(Layer *layer, GContext *ctx);
void draw_minutes(Layer *layer, GContext *ctx);
void draw_hours(Layer *layer, GContext *ctx);

void handle_init(void) {
  init_window();
  //setup the handlers
  tick_timer_service_subscribe(SECOND_UNIT, handle_ticks);
}

void init_window(void) {
  my_window = window_create();
  window_stack_push(my_window, true);
  Layer *root_layer = window_get_root_layer(my_window);
  //get geometry
  GRect bounds = layer_get_frame(root_layer);
  int16_t layer_hight = bounds.size.h/3; //devide the screen in thirds
  int16_t layer_width = bounds.size.w;
  //stack all three layers. every covering a third of the screen, seconds on top, hours at the bottom
  seconds_layer = layer_create(GRect(0, 0, layer_width, layer_hight));
  minutes_layer = layer_create(GRect(0, layer_hight, layer_width, layer_hight));
  hours_layer   = layer_create(GRect(0, layer_hight*2, layer_width, layer_hight)); 
  //put layers into root layer
  layer_add_child(root_layer, seconds_layer);
  layer_add_child(root_layer, minutes_layer);
  layer_add_child(root_layer, hours_layer);
  //add redraw handlers
  layer_set_update_proc(seconds_layer, draw_seconds);
  layer_set_update_proc(minutes_layer, draw_minutes);
  layer_set_update_proc(hours_layer, draw_hours);  
}

void handle_deinit(void) {
  layer_destroy(hours_layer);
  layer_destroy(minutes_layer);
  layer_destroy(seconds_layer);
  window_destroy(my_window);
}

void handle_ticks(struct tm *tick_time, TimeUnits units_changed) {
  //I think there is more in the unit_changed parameter. Maybe I don't need to invalidate all layers.
  now = *tick_time;
  layer_mark_dirty(seconds_layer);
  layer_mark_dirty(minutes_layer);
  layer_mark_dirty(hours_layer);  
}

void draw_seconds(Layer *layer, GContext *ctx) {
  int16_t ticktock = now.tm_sec % 2;
  GColor color = ticktock ? GColorBlack : GColorWhite;
  GRect bounds = layer_get_bounds(layer);
  // Draw a black filled rectangle
  graphics_context_set_fill_color(ctx, color);
  graphics_fill_rect(ctx, bounds, 3, GCornersAll);  
}

void draw_minutes(Layer *layer, GContext *ctx) {
}

void draw_hours(Layer *layer, GContext *ctx) {
}


int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
