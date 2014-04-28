#include "berlinclock.h"
  
//static Layer *root_layer;
static Layer *seconds_layer;
static Layer *hours_layer_top;
static Layer *hours_layer_bottom;
static Layer *minutes_layer_top;
static Layer *minutes_layer_bottom;
static struct tm now; //obj-c is so beautiful... this is the only chance to retain time throughout all draws.
  
static void draw_seconds(Layer *layer, GContext *ctx);
static void draw_hours_top(Layer *layer, GContext *ctx);
static void draw_hours_bottom(Layer *layer, GContext *ctx);
static void draw_minutes_top(Layer *layer, GContext *ctx);
static void draw_minutes_bottom(Layer *layer, GContext *ctx);
static void draw_bar(Layer *layer, GContext *ctx, int16_t on, int16_t max); //convenience function
static void draw_bar_blank(Layer *layer, GContext *ctx, int16_t on, int16_t max, int16_t fullevery);
static Layer *create_and_add_layer(Layer *parent, GRect rect);

void create_berlin_clock_layer(Window *window) {    
  Layer *root_layer = window_get_root_layer(window);
  //get geometry
  GRect bounds = layer_get_frame(root_layer);
  int16_t layer_width = bounds.size.w;
  int16_t layer_hight = bounds.size.h/5; //5 rows, 1 for each sec, 5h, 1h, 5m, 1m
  
  seconds_layer        = create_and_add_layer(root_layer, GRect(0, layer_hight*0, layer_width, layer_hight)); //0,0,lw,32
  hours_layer_top      = create_and_add_layer(root_layer, GRect(0, layer_hight*1, layer_width, layer_hight)); //0,33,lw,32
  hours_layer_bottom   = create_and_add_layer(root_layer, GRect(0, layer_hight*2, layer_width, layer_hight)); //0,66,lw,32
  minutes_layer_top    = create_and_add_layer(root_layer, GRect(0, layer_hight*3, layer_width, layer_hight)); //0,99,lw,32
  minutes_layer_bottom = create_and_add_layer(root_layer, GRect(0, layer_hight*4, layer_width, layer_hight)); //0,132,lw,32
  //add redraw handlers
  layer_set_update_proc(seconds_layer, draw_seconds);
  layer_set_update_proc(hours_layer_top, draw_hours_top);
  layer_set_update_proc(hours_layer_bottom, draw_hours_bottom);
  layer_set_update_proc(minutes_layer_top, draw_minutes_top);
  layer_set_update_proc(minutes_layer_bottom, draw_minutes_bottom);
}

void destroy_berlin_clock_layer() {
  layer_destroy(minutes_layer_bottom);
  layer_destroy(minutes_layer_top);
  layer_destroy(hours_layer_bottom);
  layer_destroy(hours_layer_top);
  layer_destroy(seconds_layer);
}

void handle_ticks(struct tm *tick_time, TimeUnits units_changed) {
  now = *tick_time;
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "tick");
  layer_mark_dirty(seconds_layer); //subscribed to seconds, so update them always
  if (units_changed & MINUTE_UNIT) { //only update if necessary
    layer_mark_dirty(minutes_layer_top);
    layer_mark_dirty(minutes_layer_bottom);
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "minute changed");
  }
  if (units_changed & HOUR_UNIT) { //only update if necessary
    layer_mark_dirty(hours_layer_top);
    layer_mark_dirty(hours_layer_bottom);
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "hour changed");
  }
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
  //full hight every bar
  draw_bar_blank(layer, ctx, on, max, 1);
}

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