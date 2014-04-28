#include <pebble.h>

#define BLACKONWHITE
#ifdef BLACKONWHITE
  #define FOREGROUND GColorBlack
  #define BACKGROUND GColorWhite
#else
  #define FOREGROUND GColorWhite
  #define BACKGROUND GColorBlack  
#endif

#define PADDING 2
#define SHORTBAR 8

void handle_ticks(struct tm *tick_time, TimeUnits units_changed);
void create_berlin_clock_layer(Window *window);
void destroy_berlin_clock_layer();