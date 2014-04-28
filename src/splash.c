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