#include <pebble.h>

#define DATE 1

Window* window;
GBitmap* bs[12];
GBitmap* bcolon;
GBitmap* bdash;
BitmapLayer* bl[10];
uint32_t bsh[12] = {RESOURCE_ID_IMAGE_0, RESOURCE_ID_IMAGE_1, RESOURCE_ID_IMAGE_2, RESOURCE_ID_IMAGE_3, RESOURCE_ID_IMAGE_4, RESOURCE_ID_IMAGE_5, RESOURCE_ID_IMAGE_6, RESOURCE_ID_IMAGE_7, RESOURCE_ID_IMAGE_8, RESOURCE_ID_IMAGE_9, RESOURCE_ID_IMAGE_colon, RESOURCE_ID_IMAGE_dash};

void setSegs(BitmapLayer* high, BitmapLayer* low, int val)
{
	bitmap_layer_set_bitmap(high, bs[val/10]);
	bitmap_layer_set_bitmap(low, bs[val%10]);
}

void handle_minute_tick(struct tm *time, TimeUnits units_changed)
{
	if(!clock_is_24h_style() && time->tm_hour > 12)
	{
		time->tm_hour -= 12;
	}
	if(!clock_is_24h_style() && time->tm_hour == 0)
	{
		time->tm_hour = 12;
	}

	setSegs(bl[0], bl[1], time->tm_hour);
	setSegs(bl[3], bl[4], time->tm_min);

	#if(DATE)
		setSegs(bl[5], bl[6], time->tm_mon+1);
		setSegs(bl[8], bl[9], time->tm_mday+1);
	#endif
}

void init(void)
{
	int i;
	static struct tm* now;
	time_t unix_now;

	window = window_create();

	for(i = 0; i < 12; i++)
	{
		bs[i] = gbitmap_create_with_resource(bsh[i]);
	}
	bcolon = bs[10];
	bdash = bs[11];

	#if(DATE)
		bl[0] = bitmap_layer_create(GRect(0,(168/2)-56,30,51));
		bl[1] = bitmap_layer_create(GRect(35,(168/2)-56,30,51));
		bl[2] = bitmap_layer_create(GRect(67,(168/2)-56,9,51));
		bl[3] = bitmap_layer_create(GRect(79,(168/2)-56,30,51));
		bl[4] = bitmap_layer_create(GRect(114,(168/2)-56,30,51));

		bl[5] = bitmap_layer_create(GRect(0,(168/2)+5,30,51));
		bl[6] = bitmap_layer_create(GRect(35,(168/2)+5,30,51));
		bl[7] = bitmap_layer_create(GRect(67,(168/2)+5,9,51));
		bl[8] = bitmap_layer_create(GRect(79,(168/2)+5,30,51));
		bl[9] = bitmap_layer_create(GRect(114,(168/2)+5,30,51));
		for(i = 0; i < 10; i++)
			layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bl[i]));
	#else
		bl[0] = bitmap_layer_create(GRect(0,(168/2)-25,30,51));
		bl[1] = bitmap_layer_create(GRect(35,(168/2)-25,30,51));
		bl[2] = bitmap_layer_create(GRect(67,(168/2)-25,9,51));
		bl[3] = bitmap_layer_create(GRect(79,(168/2)-25,30,51));
		bl[4] = bitmap_layer_create(GRect(114,(168/2)-25,30,51));
		for(i = 0; i < 5; i++)
			layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bl[i]));
	#endif

	window_set_background_color(window, GColorBlack);

	bitmap_layer_set_bitmap(bl[2], bcolon);

	#if(DATE)
		bitmap_layer_set_bitmap(bl[7], bdash);
	#endif

	tick_timer_service_subscribe(SECOND_UNIT, &handle_minute_tick);	

	unix_now = time(NULL);
	now = localtime(&unix_now);
	handle_minute_tick(now, SECOND_UNIT);

	window_stack_push(window, true);
}

void deinit(void)
{
	int i;

	for (i = 0; i < 5; i++)
		bitmap_layer_destroy(bl[i]);

	#if (DATE)
		for (i = 5; i < 10; i++)
			bitmap_layer_destroy(bl[i]);
	#endif

	window_destroy(window);

	for (i = 0; i < 12; i++)
		gbitmap_destroy(bs[i]);
}

int main(void)
{
	init();

	app_event_loop();

	deinit();
}
