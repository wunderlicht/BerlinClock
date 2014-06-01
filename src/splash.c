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

#include "splash.h"

static Layer *splash_root;
static BitmapLayer *splash_layer;
static GBitmap *splash_bitmap;

void display_splash_layer(Window *window) {
  Layer *root_layer = window_get_root_layer(window);
  splash_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BERLIN_BEAR);
  splash_layer = bitmap_layer_create(layer_get_frame(root_layer));
  bitmap_layer_set_bitmap(splash_layer, splash_bitmap);
  splash_root = bitmap_layer_get_layer(splash_layer);
  layer_add_child(root_layer, splash_root);
}

void destroy_splash_layer() {
  layer_remove_from_parent(splash_root);
  bitmap_layer_destroy(splash_layer);
  gbitmap_destroy(splash_bitmap);
}