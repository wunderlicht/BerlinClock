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

//uncomment following line to set a time (handy for screenshots)  
//#define FORCEDTIME  
#define FORCED_HOURS 10
#define FORCED_MINUTES 31

void handle_ticks(struct tm *tick_time, TimeUnits units_changed);
void create_berlin_clock_layer(Window *window);
void destroy_berlin_clock_layer();