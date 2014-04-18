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
void draw_bar(Layer *layer, GContext *ctx, int16_t on, int16_t max); //convenience function
void draw_bar_blank(Layer *layer, GContext *ctx, int16_t on, int16_t max, int16_t fullevery);
Layer *create_and_add_layer(Layer *parent, GRect rect);

void handle_init(void) {
  init_window();
  tick_timer_service_subscribe(SECOND_UNIT, handle_ticks);
}

#define BLACKONWHITE
#ifdef BLACKONWHITE
  #define FOREGROUND GColorBlack
  #define BACKGROUND GColorWhite
#else
  #define FOREGROUND GColorWhite
  #define BACKGROUND GColorBlack  
#endif
void init_window(void) {
  my_window = window_create();
  window_set_background_color(my_window, BACKGROUND);
  window_stack_push(my_window, true);
  Layer *root_layer = window_get_root_layer(my_window);
  //get geometry
  GRect bounds = layer_get_frame(root_layer);
  int16_t layer_width = bounds.size.w;  
  //create_and_add_layer(root_layer, seconds_layer, GRect(0, 33, layer_width, 32));
  seconds_layer        = create_and_add_layer(root_layer, GRect(0,   0, layer_width, 32));
  hours_layer_top      = create_and_add_layer(root_layer, GRect(0,  33, layer_width, 32));
  hours_layer_bottom   = create_and_add_layer(root_layer, GRect(0,  66, layer_width, 32));
  minutes_layer_top    = create_and_add_layer(root_layer, GRect(0,  99, layer_width, 32));
  minutes_layer_bottom = create_and_add_layer(root_layer, GRect(0, 132, layer_width, 32));
  //add redraw handlers
  layer_set_update_proc(seconds_layer, draw_seconds);
  layer_set_update_proc(hours_layer_top, draw_hours_top);
  layer_set_update_proc(hours_layer_bottom, draw_hours_bottom);
  layer_set_update_proc(minutes_layer_top, draw_minutes_top);
  layer_set_update_proc(minutes_layer_bottom, draw_minutes_bottom);
}

void handle_deinit(void) {
  tick_timer_service_unsubscribe();
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

Layer *create_and_add_layer(Layer *parent, GRect rect) {
  Layer *new_layer = layer_create(rect); //allocs memory
  layer_add_child(parent, new_layer);
  return new_layer;
}

void draw_seconds(Layer *layer, GContext *ctx) {
  int16_t ticktock = now.tm_sec % 2;
  GColor color = ticktock ? GColorBlack : GColorWhite;
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_fill_color(ctx, color);
  graphics_fill_circle(ctx, GPoint(bounds.size.w/2, bounds.size.h/2), (bounds.size.h/2)-2);
}

void draw_minutes_top(Layer *layer, GContext *ctx) {
  int16_t toprow    = now.tm_min/5;
  draw_bar_blank(layer, ctx, toprow, 11, 3);
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
  //no blanks please
  draw_bar_blank(layer, ctx, on, max, 1);
}

#define PADDING 2
#define SHORTBAR 8
void draw_bar_blank(Layer *layer, GContext *ctx, int16_t on, int16_t max, int16_t fullevery) {
  GRect bounds = layer_get_bounds(layer);
  //calculations outside the loop (conserves computing "power")
  const int16_t width = bounds.size.w/max;
  const int16_t paddedwidth = width-PADDING;
  const int16_t paddedfullhight = bounds.size.h-PADDING;
  const int16_t paddedhight = bounds.size.h-SHORTBAR;
  //delete the layer
  graphics_context_set_fill_color(ctx, BACKGROUND);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);
  //and draw the shit
  graphics_context_set_fill_color(ctx, FOREGROUND);
  for(int i=0; i<on; i++) {
    if (!((i+1)%fullevery)) {
      //fullhight
      graphics_fill_rect(ctx, GRect((width*i)+PADDING,PADDING,paddedwidth,paddedfullhight), 5, GCornersAll);    
    } else {
      //short hight
      graphics_fill_rect(ctx, GRect((width*i)+PADDING,SHORTBAR,paddedwidth,paddedhight), 5, GCornersAll);    
    }
  }
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}