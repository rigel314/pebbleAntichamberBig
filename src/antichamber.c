#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define DATE 1

#define MY_UUID { 0x17, 0x90, 0xF9, 0xFE, 0x61, 0x71, 0x4C, 0xE7, 0x8F, 0xCB, 0xC8, 0x30, 0x32, 0x6A, 0xCF, 0xA4 }
PBL_APP_INFO(MY_UUID,
			"Antichamber", "Computing Eureka",
			1, 0, /* App version */
			DEFAULT_MENU_ICON,
			APP_INFO_WATCH_FACE);

Window window;
HeapBitmap bs[12];
HeapBitmap* bcolon;
HeapBitmap* bdash;
BitmapLayer bl[10];
int bsh[12] = {RESOURCE_ID_IMAGE_0, RESOURCE_ID_IMAGE_1, RESOURCE_ID_IMAGE_2, RESOURCE_ID_IMAGE_3, RESOURCE_ID_IMAGE_4, RESOURCE_ID_IMAGE_5, RESOURCE_ID_IMAGE_6, RESOURCE_ID_IMAGE_7, RESOURCE_ID_IMAGE_8, RESOURCE_ID_IMAGE_9, RESOURCE_ID_IMAGE_colon, RESOURCE_ID_IMAGE_dash};

void setSegs(BitmapLayer* ptr, int val)
{
	bitmap_layer_set_bitmap(&ptr[0], &bs[val/10].bmp);
	bitmap_layer_set_bitmap(&ptr[1], &bs[val%10].bmp);
}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent* t)
{
	PblTm time;
	get_time(&time);

	if(!clock_is_24h_style() && time.tm_hour > 12)
	{
		time.tm_hour -= 12;
	}
	if(!clock_is_24h_style() && time.tm_hour == 0)
	{
		time.tm_hour = 12;
	}

	setSegs(&bl[0], time.tm_hour);
	setSegs(&bl[3], time.tm_min);

	#if(DATE)
		setSegs(&bl[5], time.tm_mon+1);
		setSegs(&bl[8], time.tm_mday+1);
	#endif
}

void handle_init(AppContextRef ctx)
{
	int i;

	resource_init_current_app(&RES_FOR_ANTICHAMBER);

	window_init(&window, "Antichamber");

	for(i = 0; i < 12; i++)
	{
		heap_bitmap_init(&bs[i], bsh[i]);
	}
	bcolon = &bs[10];
	bdash = &bs[11];

	#if(DATE)
		bitmap_layer_init(&bl[0], GRect(0,(168/2)-56,30,51));
		bitmap_layer_init(&bl[1], GRect(35,(168/2)-56,30,51));
		bitmap_layer_init(&bl[2], GRect(67,(168/2)-56,9,51));
		bitmap_layer_init(&bl[3], GRect(79,(168/2)-56,30,51));
		bitmap_layer_init(&bl[4], GRect(114,(168/2)-56,30,51));

		bitmap_layer_init(&bl[5], GRect(0,(168/2)+5,30,51));
		bitmap_layer_init(&bl[6], GRect(35,(168/2)+5,30,51));
		bitmap_layer_init(&bl[7], GRect(67,(168/2)+5,9,51));
		bitmap_layer_init(&bl[8], GRect(79,(168/2)+5,30,51));
		bitmap_layer_init(&bl[9], GRect(114,(168/2)+5,30,51));
		for(i = 0; i < 10; i++)
			layer_add_child(&window.layer, &bl[i].layer);
	#else
		bitmap_layer_init(&bl[0], GRect(0,(168/2)-25,30,51));
		bitmap_layer_init(&bl[1], GRect(35,(168/2)-25,30,51));
		bitmap_layer_init(&bl[2], GRect(67,(168/2)-25,9,51));
		bitmap_layer_init(&bl[3], GRect(79,(168/2)-25,30,51));
		bitmap_layer_init(&bl[4], GRect(114,(168/2)-25,30,51));
		for(i = 0; i < 5; i++)
			layer_add_child(&window.layer, &bl[i].layer);
	#endif

	window_set_background_color(&window, GColorBlack);

	bitmap_layer_set_bitmap(&bl[2], &bcolon->bmp);

	#if(DATE)
		bitmap_layer_set_bitmap(&bl[7], &bdash->bmp);
	#endif

	handle_minute_tick(ctx, NULL);

	window_stack_push(&window, true);
}

void handle_deinit(AppContextRef ctx)
{
	int i;

	for (i = 0; i < 12; i++)
	{
		heap_bitmap_deinit(&bs[i]);
	}
}

void pbl_main(void *params)
{
	PebbleAppHandlers handlers = {
		.init_handler = &handle_init,
		.deinit_handler = &handle_deinit,
		.tick_info = {
			.tick_handler = &handle_minute_tick,
			.tick_units = MINUTE_UNIT
		}
	};
	app_event_loop(params, &handlers);
}
