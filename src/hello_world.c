#include <pebble.h>

Window *window;
TextLayer *text_layer;
TextLayer *date_layer;
InverterLayer *inverter_layer;

void handle_timechanges(struct tm *tick_time, TimeUnits units_changed) {
  static char time_buffer[25];
  static char date_buffer[25];
  
  strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S\nAlex's Watch", tick_time);
  text_layer_set_text(text_layer, time_buffer);
  
  strftime(date_buffer, sizeof(date_buffer), "%a %b %e", tick_time);
  text_layer_set_text(date_layer, date_buffer);
}

void handle_init(void) {
	// Create a window and text layer
	window = window_create();
	text_layer = text_layer_create(GRect(0, 0, 144, 56));
	
	// Set the text, font, and text alignment
	text_layer_set_text(text_layer, "Alex's watch");
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));

  date_layer = text_layer_create(GRect(0, 62, 144, 106));
  //date_layer = text_layer_create(GRect(0, 56, 144, 112));
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));

  inverter_layer = inverter_layer_create(GRect(0, 62, 144, 106));
  //inverter_layer = inverter_layer_create(GRect(0, 56, 144, 112));
  layer_add_child(window_get_root_layer(window), inverter_layer_get_layer(inverter_layer));
  
  time_t now = time(NULL);
  handle_timechanges(localtime(&now), SECOND_UNIT);
  tick_timer_service_subscribe(SECOND_UNIT, handle_timechanges);
  
	// Push the window
	window_stack_push(window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(text_layer);
  text_layer_destroy(date_layer);
  inverter_layer_destroy(inverter_layer);
	
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
