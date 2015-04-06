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
#ifdef PBL_BW
  #define BLACKONWHITE
  #ifdef BLACKONWHITE
    #define FOREGROUND GColorBlack
    #define BACKGROUND GColorWhite
  #else
    #define FOREGROUND GColorWhite
    #define BACKGROUND GColorBlack  
  #endif
  #define SECONDSFILL FOREGROUND
  #define MINUTESFILL FOREGROUND
  #define HOURSFILL FOREGROUND
#endif
  
#ifdef PBL_COLOR  
  #define BACKGROUND GColorLightGray
  #define SECONDSFILL GColorYellow
  #define HOURSFILL GColorRed
  #define MINUTESFILL GColorYellow
  #define QUARTERFILL GColorRed
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