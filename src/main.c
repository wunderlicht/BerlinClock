#include <pebble.h>

Window *my_window;
Layer *root_layer;
Layer *seconds_layer;
Layer *hours_layer_top;
Layer *hours_layer_bottom;
Layer *minutes_layer_top;
Layer *minutes_layer_bottom;
struct tm now; //obj-c is so beautiful... this is the only chance to retain time throughout all draws.

//forward declaration as learned in university :-)
void handle_init(void);
void init_window(void);
void handle_deinint(void);
void handle_ticks(struct tm *tick_time, TimeUnits units_changed);
void draw_seconds(Layer *layer, GContext *ctx);
void draw_hours_top(Layer *layer, GContext *ctx);
void draw_hours_bottom(Layer *layer, GContext *ctx);
void draw_minutes_top(Layer *layer, GContext *ctx);
void draw_minutes_bottom(Layer *layer, GContext *ctx);
void draw_bar(Layer *layer, GContext *ctx, int16_t on, int16_t max);

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
  
  seconds_layer        = layer_create(GRect(0, 0, layer_width, 32));
  hours_layer_top      = layer_create(GRect(0, 33, layer_width, 32));
  hours_layer_bottom   = layer_create(GRect(0, 66, layer_width, 32));
  minutes_layer_top    = layer_create(GRect(0, 99, layer_width, 32));
  minutes_layer_bottom = layer_create(GRect(0, 132, layer_width, 32));
  //put layers into root layer
  layer_add_child(root_layer, seconds_layer);
  layer_add_child(root_layer, hours_layer_top);
  layer_add_child(root_layer, hours_layer_bottom);
  layer_add_child(root_layer, minutes_layer_top);
  layer_add_child(root_layer, minutes_layer_bottom);
  //add redraw handlers
  layer_set_update_proc(seconds_layer, draw_seconds);
  layer_set_update_proc(hours_layer_top, draw_hours_top);
  layer_set_update_proc(hours_layer_bottom, draw_hours_bottom);
  layer_set_update_proc(minutes_layer_top, draw_minutes_top);
  layer_set_update_proc(minutes_layer_bottom, draw_minutes_bottom);
}

void handle_deinit(void) {
  layer_destroy(minutes_layer_bottom);
  layer_destroy(minutes_layer_top);
  layer_destroy(hours_layer_bottom);
  layer_destroy(hours_layer_top);
  layer_destroy(seconds_layer);
  window_destroy(my_window);
}

void handle_ticks(struct tm *tick_time, TimeUnits units_changed) {
  //I think there is more in the unit_changed parameter. Maybe I don't need to invalidate all layers.
  now = *tick_time;
  layer_mark_dirty(seconds_layer);
  layer_mark_dirty(hours_layer_top);  
  layer_mark_dirty(hours_layer_bottom);  
  layer_mark_dirty(minutes_layer_top);
  layer_mark_dirty(minutes_layer_bottom);
}

void draw_seconds(Layer *layer, GContext *ctx) {
  int16_t ticktock = now.tm_sec % 2;
  GColor color = ticktock ? GColorBlack : GColorWhite;
  GRect bounds = layer_get_bounds(layer);
  // Draw a black filled rectangle
  graphics_context_set_fill_color(ctx, color);
  graphics_fill_circle(ctx, GPoint(bounds.size.w/2, bounds.size.h/2), (bounds.size.h/2)-2);
}

void draw_minutes_top(Layer *layer, GContext *ctx) {
  int16_t toprow    = now.tm_min/5;
  draw_bar(layer, ctx, toprow, 11);
}

void draw_minutes_bottom(Layer *layer, GContext *ctx) {
  int16_t bottomrow = now.tm_min%5;
  draw_bar(layer, ctx, bottomrow, 4);
}

void draw_hours_top(Layer *layer, GContext *ctx) {
  int16_t toprow    = now.tm_hour/5;
  draw_bar(layer, ctx, toprow, 4);
}

void draw_hours_bottom(Layer *layer, GContext *ctx) {
  int16_t bottomrow = now.tm_hour%5;
  draw_bar(layer, ctx, bottomrow, 4);
}

void draw_bar(Layer *layer, GContext *ctx, int16_t on, int16_t max) {
  GRect bounds = layer_get_bounds(layer);
  const int16_t width = bounds.size.w/max;
  //delete the layer
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);
  //and draw the shit
  graphics_context_set_fill_color(ctx, GColorBlack);
  for(int i=0; i<on; i++) {
    graphics_fill_rect(ctx, GRect((width*i)+2,2,width-2,bounds.size.h), 5, GCornersAll);
  }
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}