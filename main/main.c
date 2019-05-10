#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "driver/ledc.h"
#include "driver/i2s.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_task_wdt.h"
#include "esp_spiffs.h"
#include "driver/rtc_io.h"
#include "esp_partition.h"
#include "esp_ota_ops.h"
/*
#include "../components/gnuboy/loader.h"
#include "../components/gnuboy/hw.h"
#include "../components/gnuboy/lcd.h"
#include "../components/gnuboy/fb.h"
#include "../components/gnuboy/cpu.h"
#include "../components/gnuboy/mem.h"
#include "../components/gnuboy/sound.h"
#include "../components/gnuboy/pcm.h"
#include "../components/gnuboy/regs.h"
#include "../components/gnuboy/rtc.h"
#include "../components/gnuboy/gnuboy.h"
*/
#include "pcm.h"
#include <string.h>

//#include "hourglass_empty_black_48dp.h"
//#include "../components/odroid/image_splash.h"

#include "../components/odroid/odroid_settings.h"
#include "../components/odroid/odroid_input.h"
#include "../components/odroid/odroid_display.h"
#include "../components/odroid/odroid_audio.h"
#include "../components/odroid/odroid_system.h"
#include "../components/odroid/odroid_sdcard.h"
//#include "../components/odroid/odroid_overlay.h"

//#include "../components/ugui/ugui.h"
#include "../components/mulg/mulg.h"

#include "network.h"

extern int debug_trace;
extern void draw_text(uint16_t x, uint16_t y, const char* message);

extern unsigned int marble_x[];
extern unsigned int marble_y[];	// current position on screen
extern unsigned int marble_xp[];
extern unsigned int marble_yp[]; // current page
extern unsigned char level_buffer[MAX_HEIGHT][MAX_WIDTH];
extern unsigned short level_width, level_height;


//struct fb fb;
struct pcm pcm;


uint8_t* displayBuffer[2]; //= { fb0, fb0 }; //[160 * 144];
uint8_t currentBuffer;

uint8_t* framebuffer;
int frame = 0;
uint elapsedTime = 0;

uint8_t mulgLoops = 0;

int32_t* audioBuffer[2];
volatile uint8_t currentAudioBuffer = 0;
volatile uint16_t currentAudioSampleCount;
volatile int16_t* currentAudioBufferPtr;

odroid_battery_state battery_state;

char tempstring[512];
overlay_struct overlay;

const char* StateFileName = "/storage/gnuboy.sav";


#define AUDIO_SAMPLE_RATE (32000)

const char* SD_BASE_PATH = "/sd";

// --- MAIN
QueueHandle_t vidQueue;
QueueHandle_t audioQueue;

float Volume = 1.0f;

int pcm_submit()
{
    odroid_audio_submit(currentAudioBufferPtr, currentAudioSampleCount >> 1);

    return 1;
}


int BatteryPercent = 100;


void run_to_vblank()
{
  /* FRAME BEGIN */

  /* FIXME: djudging by the time specified this was intended
  to emulate through vblank phase which is handled at the
  end of the loop. */
//  cpu_emulate(2280);

  /* FIXME: R_LY >= 0; comparsion to zero can also be removed
  altogether, R_LY is always 0 at this point */
 // while (R_LY > 0 && R_LY < 144)
//  {
    /* Step through visible line scanning phase */
//    emu_step();
//  }

  /* VBLANK BEGIN */

  //vid_end();
  /*
  if ((frame % 2) == 0)
  {
      xQueueSend(vidQueue, &framebuffer, portMAX_DELAY);

      // swap buffers
      currentBuffer = currentBuffer ? 0 : 1;
      framebuffer = displayBuffer[currentBuffer];

      fb.ptr = framebuffer;
  }
*/

//  sound_mix();

  //if (pcm.pos > 100)
  /*
  {
        currentAudioBufferPtr = audioBuffer[currentAudioBuffer];
        currentAudioSampleCount = pcm.pos;

        void* tempPtr = 0x1234;
        xQueueSend(audioQueue, &tempPtr, portMAX_DELAY);

        // Swap buffers
        currentAudioBuffer = currentAudioBuffer ? 0 : 1;
        pcm.buf = audioBuffer[currentAudioBuffer];
        pcm.pos = 0;
  }*/

 // if (!(R_LCDC & 0x80)) {
    /* LCDC operation stopped */
    /* FIXME: djudging by the time specified, this is
    intended to emulate through visible line scanning
    phase, even though we are already at vblank here */
//    cpu_emulate(32832);
//  }

//  while (R_LY > 0) {
    /* Step through vblank phase */
//    emu_step();
//  }/*
}


uint16_t* menuFramebuffer = 0;
uint16_t myPalette[256];


volatile bool videoTaskIsRunning = false;
bool scaling_enabled = true;
bool previous_scale_enabled = true;

void videoTask(void *arg)
{
  esp_err_t ret;

  videoTaskIsRunning = true;
  

  uint8_t* bmp = NULL;
  
  while(1)
  {
	  		xQueuePeek(vidQueue, &bmp, portMAX_DELAY);

        if (bmp == 1) break;

		/*
        if (previous_scaling_enabled != scaling_enabled)
        {
            // Clear display
            ili9341_write_frame_nes(NULL, NULL, true);
            previous_scaling_enabled = scaling_enabled;
        }*/

        ili9341_write_frame_mulg(bmp, myPalette, scaling_enabled);

       odroid_input_battery_level_read(&battery_state);

		xQueueReceive(vidQueue, &bmp, portMAX_DELAY);
	  
	  /*
        xQueuePeek(vidQueue, &param, portMAX_DELAY);

        if (param == 1)
            break;
*/
/*
        if (previous_scale_enabled != scaling_enabled)
        {
            // Clear display
            ili9341_write_frame_gb(NULL, true, overlay);
            previous_scale_enabled = scaling_enabled;
        }
*/
		
		//ili9341_write_frame_nes(param, scaling_enabled,overlay);
		
		//ili9341_write_frame_nes(param, image_splash_palette);
		//ili9341_write_frame_scumm(image_splash, image_splash_palette);
		
		// draw overlay if required
//		if(overlay.visible){
//			ili9341_write_frame_rectangleLE(overlay.x, overlay.y, overlay.width, overlay.height, overlay.pixel_data); 
//		}

	/*	
        odroid_input_battery_level_read(&battery_state);

        xQueueReceive(vidQueue, &param, portMAX_DELAY);
		*/
    }


    // Draw hourglass
//    odroid_display_lock_gb_display();

//    odroid_display_show_hourglass();

//    odroid_display_unlock_gb_display();


    videoTaskIsRunning = false;
    vTaskDelete(NULL);

    while (1) {}
}


volatile bool AudioTaskIsRunning = false;
void audioTask(void* arg)
{
	/*
  // sound
  uint16_t* param;

  AudioTaskIsRunning = true;
  while(1)
  {
	
    xQueuePeek(audioQueue, &param, portMAX_DELAY);

    if (param == 0)
    {
        // TODO: determine if this is still needed
        abort();
    }
    else if (param == 1)
    {
        break;
    }
    else
    {
        pcm_submit();
    }

    xQueueReceive(audioQueue, &param, portMAX_DELAY);
  }

  printf("audioTask: exiting.\n");
  odroid_audio_terminate();

  AudioTaskIsRunning = false;
  vTaskDelete(NULL);

  while (1) {}
*/
  }


static void SaveState()
{
    // Save sram
    odroid_input_battery_monitor_enabled_set(0);
    odroid_system_led_set(1);

	/*
    char* romPath = odroid_settings_RomFilePath_get();
    if (romPath)
    {
        char* fileName = odroid_util_GetFileName(romPath);
        if (!fileName) abort();

        char* pathName = odroid_sdcard_create_savefile_path(SD_BASE_PATH, fileName);
        if (!pathName) abort();

        FILE* f = fopen(pathName, "w");
        if (f == NULL)
        {
            printf("%s: fopen save failed\n", __func__);
            abort();
        }

        savestate(f);
        fclose(f);

        printf("%s: savestate OK.\n", __func__);

        free(pathName);
        free(fileName);
        free(romPath);
    }
    else
    {
        FILE* f = fopen(StateFileName, "w");
        if (f == NULL)
        {
            printf("SaveState: fopen save failed\n");
        }
        else
        {
            savestate(f);
            fclose(f);

            printf("SaveState: savestate OK.\n");
        }
    }

	*/

    odroid_system_led_set(0);
    odroid_input_battery_monitor_enabled_set(1);
}

static void LoadState(const char* cartName)
{
	/*
	
    char* romName = odroid_settings_RomFilePath_get();
    if (romName)
    {
        char* fileName = odroid_util_GetFileName(romName);
        if (!fileName) abort();

        char* pathName = odroid_sdcard_create_savefile_path(SD_BASE_PATH, fileName);
        if (!pathName) abort();

        FILE* f = fopen(pathName, "r");
        if (f == NULL)
        {
            printf("LoadState: fopen load failed\n");
        }
        else
        {
            loadstate(f);
            fclose(f);

            vram_dirty();
            pal_dirty();
            sound_dirty();
            mem_updatemap();

            printf("LoadState: loadstate OK.\n");
        }

        free(pathName);
        free(fileName);
        free(romName);
    }
    else
    {
        FILE* f = fopen(StateFileName, "r");
        if (f == NULL)
        {
            printf("LoadState: fopen load failed\n");
        }
        else
        {
            loadstate(f);
            fclose(f);

            vram_dirty();
            pal_dirty();
            sound_dirty();
            mem_updatemap();

            printf("LoadState: loadstate OK.\n");
        }
    }

	pal_set(odroid_settings_GBPalette_get(), rom.hash);
	*/
	
    Volume = odroid_settings_Volume_get();
}

static void PowerDown()
{
    uint16_t* param = 1;

    // Clear audio to prevent studdering
    printf("PowerDown: stopping audio.\n");

    xQueueSend(audioQueue, &param, portMAX_DELAY);
    while (AudioTaskIsRunning) {}


    // Stop tasks
    printf("PowerDown: stopping tasks.\n");

    xQueueSend(vidQueue, &param, portMAX_DELAY);
    while (videoTaskIsRunning) {}


    // state
    printf("PowerDown: Saving state.\n");
    SaveState();

    // LCD
    printf("PowerDown: Powerdown LCD panel.\n");
    ili9341_poweroff();

    odroid_system_sleep();


    // Should never reach here
    abort();
}

static void DoMenuHome()
{
    esp_err_t err;
    uint16_t* param = 1;

    // Clear audio to prevent studdering
    printf("PowerDown: stopping audio.\n");

    xQueueSend(audioQueue, &param, portMAX_DELAY);
    while (AudioTaskIsRunning) {}


    // Stop tasks
    printf("PowerDown: stopping tasks.\n");

    xQueueSend(vidQueue, &param, portMAX_DELAY);
    while (videoTaskIsRunning) {}


    // state
    printf("PowerDown: Saving state.\n");
    SaveState();


    // Set menu application
    odroid_system_application_set(0);


    // Reset
    esp_restart();
}
static void DoMenuHomeNoSave()
{
    esp_err_t err;
    uint16_t* param = 1;

    // Clear audio to prevent studdering
    printf("PowerDown: stopping audio.\n");

    xQueueSend(audioQueue, &param, portMAX_DELAY);
    while (AudioTaskIsRunning) {}


    // Stop tasks
    printf("PowerDown: stopping tasks.\n");

    xQueueSend(vidQueue, &param, portMAX_DELAY);
    while (videoTaskIsRunning) {}


    // Set menu application
    odroid_system_application_set(0);


    // Reset
    esp_restart();
}


static void DisplayMessage(const char* message)
{
    overlay.x = (320 / 2) - (strlen(message) * 9 / 2);
    overlay.y = (240 / 2) + 8 + (12 / 2) + 32;
    overlay.width = strlen(message) * 9;
	overlay.height = 12;
		
	overlay.frames = 100;
	overlay.visible = 1;
}

/*
void draw_text(uint16_t x, uint16_t y, const char* message){
	UG_SetForecolor(1);
    UG_SetBackcolor(0);
//    UG_FillFrame(0, 0, overlay.width, overlay.height, C_BLACK);
  //  UG_PutString(100, 225, "Time: 00:07.6");
  UG_PutString(x, y, message);
}*/

void app_main(void)
{
    printf("Mulg GO (%s-%s).\n", COMPILEDATE, GITREV);

	printf("HEAP:0x%x (%#08x)\n", esp_get_free_heap_size(), heap_caps_get_free_size(MALLOC_CAP_INTERNAL));

    nvs_flash_init();

    odroid_system_init();

    odroid_input_gamepad_init();

	printf("HEAP:0x%x (%#08x)\n", esp_get_free_heap_size(), heap_caps_get_free_size(MALLOC_CAP_INTERNAL));

    // Boot state overrides
    bool forceConsoleReset = false;

    switch (esp_sleep_get_wakeup_cause())
    {
        case ESP_SLEEP_WAKEUP_EXT0:
        {
            printf("app_main: ESP_SLEEP_WAKEUP_EXT0 deep sleep wake\n");
            break;
        }

        case ESP_SLEEP_WAKEUP_EXT1:
        case ESP_SLEEP_WAKEUP_TIMER:
        case ESP_SLEEP_WAKEUP_TOUCHPAD:
        case ESP_SLEEP_WAKEUP_ULP:
        case ESP_SLEEP_WAKEUP_UNDEFINED:
        {
            printf("app_main: Non deep sleep startup\n");

            odroid_gamepad_state bootState = odroid_input_read_raw();

            if (bootState.values[ODROID_INPUT_MENU])
            {
                // Force return to factory app to recover from
                // ROM loading crashes

                // Set menu application
                odroid_system_application_set(0);

                // Reset
                esp_restart();
            }

            if (bootState.values[ODROID_INPUT_START])
            {
                // Reset emulator if button held at startup to
                // override save state
                forceConsoleReset = true;
            }

            break;
        }
        default:
            printf("app_main: Not a deep sleep reset\n");
            break;
    }

    if (odroid_settings_StartAction_get() == ODROID_START_ACTION_RESTART)
    {
        forceConsoleReset = true;
        odroid_settings_StartAction_set(ODROID_START_ACTION_NORMAL);
    }


    // Display
    ili9341_prepare();
    ili9341_init();
    //odroid_display_show_splash();

	printf("(Display Init)\nHEAP:0x%x (%#08x)\n", esp_get_free_heap_size(), heap_caps_get_free_size(MALLOC_CAP_INTERNAL));

	
    // Load ROM
//    loader_init(NULL);

	// Allocate overlay buffer
/*	
	overlay.visible = 0;
	overlay.x = 0;
	overlay.y = 0;
	overlay.width = 320;
	overlay.height = 12;
	overlay.pixel_data = heap_caps_malloc(overlay.width * overlay.height * 2, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);
	memset(overlay.pixel_data, 0, overlay.width * overlay.height * 2);
*/	
	
    // Clear display
//    ili9341_write_frame_gb(NULL, true, overlay);

    // Audio hardware
    odroid_audio_init(odroid_settings_AudioSink_get(), AUDIO_SAMPLE_RATE);

	printf("(Before Display Buffer init)\nHEAP:0x%x (%#08x)\n", esp_get_free_heap_size(), heap_caps_get_free_size(MALLOC_CAP_INTERNAL));

	
    // Allocate display buffers
    //displayBuffer[0] = heap_caps_malloc(320 * 240, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);
    //displayBuffer[1] = heap_caps_malloc(320 * 240, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);
//    displayBuffer[0] = heap_caps_malloc(160 * 160, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);
    //displayBuffer[1] = heap_caps_malloc(160 * 160, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);
    displayBuffer[0] = heap_caps_malloc(240 * 240, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);
    displayBuffer[1] = heap_caps_malloc(240 * 240, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);

    if (displayBuffer[0] == 0 || displayBuffer[1] == 0)
        abort();

    framebuffer = displayBuffer[0];

    for (int i = 0; i < 2; ++i)
    {
        //memset(displayBuffer[i], 0, 160*160);
		memset(displayBuffer[i], 0, 240*240);
    }

    printf("app_main: displayBuffer[0]=%p, [1]=%p\n", displayBuffer[0], displayBuffer[1]);

	printf("(Display Buffer init done)\nHEAP:0x%x (%#08x)\n", esp_get_free_heap_size(), heap_caps_get_free_size(MALLOC_CAP_INTERNAL));

		
    // blue led
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_2, 0);

    //  Charge
    odroid_input_battery_level_init();

    // video
    vidQueue = xQueueCreate(1, sizeof(uint16_t*));
    audioQueue = xQueueCreate(1, sizeof(uint16_t*));

    xTaskCreatePinnedToCore(&videoTask, "videoTask", 1024, NULL, 5, NULL, 1);
  //  xTaskCreatePinnedToCore(&audioTask, "audioTask", 2048, NULL, 5, NULL, 1); //768

	printf("(Task pinned)\nHEAP:0x%x (%#08x)\n", esp_get_free_heap_size(), heap_caps_get_free_size(MALLOC_CAP_INTERNAL));


    //debug_trace = 1;

//    emu_reset();
/*
    //&rtc.carry, &rtc.stop,
    rtc.d = 1;
    rtc.h = 1;
    rtc.m = 1;
    rtc.s = 1;
    rtc.t = 1;

    // vid_begin
    memset(&fb, 0, sizeof(fb));
    fb.w = 160;
  	fb.h = 144;
  	fb.pelsize = 2;
  	fb.pitch = fb.w * fb.pelsize;
  	fb.indexed = 0;
  	fb.ptr = framebuffer;
  	fb.enabled = 1;
  	fb.dirty = 0;
*/

    // Note: Magic number obtained by adjusting until audio buffer overflows stop.
    const int audioBufferLength = AUDIO_SAMPLE_RATE / 10 + 1;
    //printf("CHECKPOINT AUDIO: HEAP:0x%x - allocating 0x%x\n", esp_get_free_heap_size(), audioBufferLength * sizeof(int16_t) * 2 * 2);
    const int AUDIO_BUFFER_SIZE = audioBufferLength * sizeof(int16_t) * 2;
/*
    // pcm.len = count of 16bit samples (x2 for stereo)
    memset(&pcm, 0, sizeof(pcm));
    pcm.hz = AUDIO_SAMPLE_RATE;
  	pcm.stereo = 1;
  	pcm.len = audioBufferLength;//pcm.hz / 2 
  	pcm.buf = heap_caps_malloc(AUDIO_BUFFER_SIZE, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);
  	pcm.pos = 0;

    audioBuffer[0] = pcm.buf;
    audioBuffer[1] = heap_caps_malloc(AUDIO_BUFFER_SIZE, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);

    if (audioBuffer[0] == 0 || audioBuffer[1] == 0)
        abort();

	printf("(Audio Buffer)\nHEAP:0x%x (%#08x)\n", esp_get_free_heap_size(), heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
*/

//    sound_reset();


//    lcd_begin();


    // Load state
//    LoadState(rom.name);

//	sprintf(tempstring, "Rom hash: %d", rom.hash);
//	DisplayMessage(tempstring);
	
	uint next_tick=0;
	uint last_tick=0;
	uint8_t overflow = 0;
	uint tick=240*1000*40;
	
    uint startTime;
    uint stopTime;
    uint totalElapsedTime = 0;
    uint actualFrameCount = 0;
    odroid_gamepad_state lastJoysticState;

    ushort menuButtonFrameCount = 0;
    bool ignoreMenuButton = lastJoysticState.values[ODROID_INPUT_MENU];

    // Reset if button held at startup
    if (forceConsoleReset)
    {
  //      emu_reset();
    }


//    scaling_enabled = odroid_settings_ScaleDisabled_get(ODROID_SCALE_DISABLE_GB) ? false : true;

    odroid_input_gamepad_read(&lastJoysticState);

	//ili9341_write_frame_scumm(image_splash, image_splash_palette);
	
	//image_splash, image_splash_palette);
			// copy from SD card
	esp_err_t r = odroid_sdcard_open(SD_BASE_PATH);
	if (r != ESP_OK)
	{
		//odroid_display_show_sderr(ODROID_SD_ERR_NOCARD);
		abort();
	}

	printf("Test build in Level\n");
	printf("(Before Mulg load)\nHEAP:0x%x (%#08x)\n", esp_get_free_heap_size(), heap_caps_get_free_size(MALLOC_CAP_INTERNAL));

	
	mulg_load();

	printf("(Mulg load done.)\nHEAP:0x%x (%#08x)\n", esp_get_free_heap_size(), heap_caps_get_free_size(MALLOC_CAP_INTERNAL));

	
	// 1s = 240*1000000
	// 1ms = 240*1000
	//CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ=240
	//1 = totalElapsedTime / (CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ * 1000000.0f); // 240000000.0f; // (240Mhz)
	printf("CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ: %d\n", CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ );

	
	uint8_t network_client=0;
	uint8_t network_server=0;
	int i;
	uint8_t joystick_changed = false;
	uint32_t packages_send = 0;
	uint8_t buffer[1500];
	uint8_t cnt = 0;
	
    while (true)
    {
        odroid_gamepad_state joystick;
        odroid_input_gamepad_read(&joystick);
		
		joystick_changed = false;
		
		for(i = 0; i<10; i++){
				if(lastJoysticState.values[i] != joystick.values[i]){
					joystick_changed = true;
					break;
				}
		}
		

        if (ignoreMenuButton)
        {
            ignoreMenuButton = lastJoysticState.values[ODROID_INPUT_MENU];
        }

        if (!ignoreMenuButton && lastJoysticState.values[ODROID_INPUT_MENU] && joystick.values[ODROID_INPUT_MENU])
        {
            ++menuButtonFrameCount;
        }
        else
        {
            menuButtonFrameCount = 0;
        }

        //if (!lastJoysticState.Menu && joystick.Menu)
        if (menuButtonFrameCount > 60 * 1)
        {
            // Save state
            gpio_set_level(GPIO_NUM_2, 1);

            DoMenuHome();

            gpio_set_level(GPIO_NUM_2, 0);
        }

        if (!ignoreMenuButton && lastJoysticState.values[ODROID_INPUT_MENU] && !joystick.values[ODROID_INPUT_MENU])
        {
            // Save state
            //gpio_set_level(GPIO_NUM_2, 1);

            //DoMenu();
            DoMenuHomeNoSave();

            gpio_set_level(GPIO_NUM_2, 0);
        }


        if (!lastJoysticState.values[ODROID_INPUT_VOLUME] && joystick.values[ODROID_INPUT_VOLUME])
        {
            odroid_audio_volume_change();
            printf("main: Volume=%d\n", odroid_audio_volume_get());
        }


        // Network
        if (joystick.values[ODROID_INPUT_START] && !lastJoysticState.values[ODROID_INPUT_RIGHT] && joystick.values[ODROID_INPUT_RIGHT])
        {
			printf("(Before network init.)\nHEAP:0x%x (%#08x)\n", esp_get_free_heap_size(), heap_caps_get_free_size(MALLOC_CAP_INTERNAL));

			network_host_init();
        
			printf("%s: Waiting for Player 2 ...\n", network_hostname_get());
			//network_host_wait_for_connection();
			network_server = 1;
        }

		if (joystick.values[ODROID_INPUT_START] && !lastJoysticState.values[ODROID_INPUT_LEFT] && joystick.values[ODROID_INPUT_LEFT])
        {
			printf("Scanning for Hosts ...\n");
			network_client_init();

			network_client_connect_to_host();

			printf("Syncronizing with Host ...\n");
			network_client = 1;
			//tick = 240*1000*5;
			tick = 240*1000*7;
			//tick = tick/2;
        }

		
/*		

        pad_set(PAD_UP, joystick.values[ODROID_INPUT_UP]);
        pad_set(PAD_RIGHT, joystick.values[ODROID_INPUT_RIGHT]);
        pad_set(PAD_DOWN, joystick.values[ODROID_INPUT_DOWN]);
        pad_set(PAD_LEFT, joystick.values[ODROID_INPUT_LEFT]);

        pad_set(PAD_SELECT, joystick.values[ODROID_INPUT_SELECT]);
        pad_set(PAD_START, joystick.values[ODROID_INPUT_START]);

        pad_set(PAD_A, joystick.values[ODROID_INPUT_A]);
        pad_set(PAD_B, joystick.values[ODROID_INPUT_B]);
*/

/*
        startTime = xthal_get_ccount();
        run_to_vblank();
        stopTime = xthal_get_ccount();
*/

/*
		if(mulgLoops<1){
			mulg_run(0);
			mulgLoops++;
			//drawChar(100,200,'A',1);
			//ödraw_text(100,235, "Hello World!");
			xQueueSend(vidQueue, &framebuffer, portMAX_DELAY);
		}
*/
	
		startTime = xthal_get_ccount();
		
		if(startTime<last_tick){
			// Overflow done!
			if(overflow){
				//printf("Overflow occured: %d,%d,%d\n",startTime,last_tick,next_tick);
				//printf("Unsigned: %u,%u,%u\n",startTime,last_tick,next_tick);
				overflow = 0;
			}else{
				// printf("Unexcpected overflow: %d,%d,%d\n",startTime,last_tick,next_tick);
				// printf("Unsigned: %u,%u,%u\n",startTime,last_tick,next_tick);
				// force tick as last tick must have been missed (unexpected overflow)
				next_tick = 0;
			}
		}
		
		
		uint8_t screen_changed = 0;
		uint8_t js =0;
		uint8_t js2 =0;
			

		if((!overflow && startTime>next_tick)||(network_client && joystick_changed)){
		//if(mulgLoops<1){
			
			js =0;
			js2 = 0;
			
			//memset(framebuffer, 0, 240*240);
			
			
			//if(!(joystick.values[ODROID_INPUT_SELECT]||joystick.values[ODROID_INPUT_START])){
				if(joystick.values[ODROID_INPUT_UP]){
					js +=0x01;
				}if(joystick.values[ODROID_INPUT_RIGHT]){
					js +=0x02;
				}if(joystick.values[ODROID_INPUT_DOWN]){
					js +=0x04;
				}if(joystick.values[ODROID_INPUT_LEFT]){
					js +=0x08;
				}if(joystick.values[ODROID_INPUT_A]){
					js +=0x10;
				}if(joystick.values[ODROID_INPUT_B]){
					js +=0x20;
				}if(joystick.values[ODROID_INPUT_SELECT]){
					js +=0x40;
				}if(joystick.values[ODROID_INPUT_START]){
					js +=0x80;
				}
//			}

			
			if(network_client){
				buffer[0] = cnt++;
				buffer[1] = js;
				
				//int res = network_send(&js, 1);
				network_send(buffer, 2);
				
				packages_send++;
				
				
				bool received_screen = false;
				
				while((i = network_get_dgram_size())!=0){
					//printf("Received Marbleposition!\n");
					//printf("Available Byte: %d\n", i);
					//printf("Going to receive...\n");
					//int res = network_recv2(&js2, 1);
					int res = network_recv2(buffer, i);
					
					uint32_t* ibuffer = (uint32_t*)buffer;
					
					marble_x[0] = ibuffer[0]; // 0..3
					marble_y[0] = ibuffer[1]; // 4..7 
					marble_x[1] = ibuffer[2]; // 8..11
					marble_y[1] = ibuffer[3]; // 12..15

					marble_xp[0] = buffer[16]; //
					marble_yp[0] = buffer[17];
					marble_xp[1] = buffer[18];
					marble_yp[1] = buffer[19];

					level_width = buffer[20];
					level_height = buffer[21];
					
					for(i=0;i<level_width*level_height;i++){
						level_buffer[i/level_width][i%level_width] = buffer[22+i];
					}
				
					received_screen = true;

					
					
	/*				if(res!=0){
						printf("Error receive: %s\n", strerror(errno));
	}*/	
					
				}
				
				if(received_screen){
					screen_changed = mulg_draw_slave();
					
				}else{
					//printf("Received no Marbleposition!\n");
				}
				
				
				
			}else{
				memset(framebuffer, 0, 240*240);
			}
				
				
				

			

			
			
			if(network_server){
				js2 = 0xff;
				while((i = network_get_dgram_size())!=0){
					//printf("Available Byte: %d\n", i);
					//printf("Going to receive...\n");
					//int res = network_recv2(&js2, 1);
					int res = network_recv2(buffer, 2);
					//printf("received: %d-%d\n",buffer[0],buffer[1]);
					js2 = buffer[1];
	/*				if(res!=0){
						printf("Error receive: %s\n", strerror(errno));
						
					}*/
				}
				
				if(js2==0xff){
					printf("No Data received in loop %d!\n", mulgLoops);
					js2=0;
				}
			}
			
			if(!network_client){
				screen_changed = mulg_run(js,js2);
			}
			
			if(network_server){
				// Send Marble position to client
				if(screen_changed){
					uint32_t* ibuffer = (uint32_t*)buffer;
					
					ibuffer[0] = marble_x[0];
					ibuffer[1] = marble_y[0];
					ibuffer[2] = marble_x[1];
					ibuffer[3] = marble_y[1];
	
					buffer[16] = marble_xp[0];
					buffer[17] = marble_yp[0];
					buffer[18] = marble_xp[1];
					buffer[19] = marble_yp[1];
	
					buffer[20] = level_width;
					buffer[21] = level_height;
					for(i=0;i<level_width*level_height;i++){
						buffer[22+i] = level_buffer[i/level_width][i%level_width];
					}
	
					//printf("network_send nr of bytes: %d\n", 22 + level_width*level_height);
					network_send(buffer, 22 + level_width*level_height);
					//printf("network_send result: %s\n", strerror(errno));
				}
				
				
			}
			
			mulgLoops++;

		if(mulgLoops%50==0){
			          printf("HEAP:0x%x\n", esp_get_free_heap_size());
		}
		
//		    UG_FontSelect(&FONT_8X12);
//    overlay.x = (320 / 2) - (strlen(message) * 9 / 2);
//    overlay.y = (240 / 2) + 8 + (12 / 2) + 32;
//    overlay.width = strlen(message) * 9;
//	overlay.height = 12;
	
//	UG_SetForecolor(1);
    //UG_SetBackcolor(0);
//    UG_FillFrame(0, 0, overlay.width, overlay.height, C_BLACK);
//    UG_PutString(100, 225, "Time: 00:07.6");
	
//	overlay.frames = 100;
//	overlay.visible = 1;
		
		//320-240
		
			if(screen_changed){
				xQueueSend(vidQueue, &framebuffer, portMAX_DELAY);

				// swap buffers
				currentBuffer = currentBuffer ? 0 : 1;
				framebuffer = displayBuffer[currentBuffer];
			}

			next_tick = startTime + tick;
			if(next_tick<startTime){
				overflow = 1;
				//printf("Expecting Overflow\n");
			}
			
		}
		
		
		
		
		last_tick = startTime;
		
		
		//DisplayMessage("Hello World!");

//		overlay.pixel_data[0]=0xffff;
//		overlay.pixel_data[1]=0x1f;
//		overlay.pixel_data[25]=0x3e0;
//		overlay.pixel_data[26]=0x7c00;
		
		
//		if(!overlay.visible){
//			UG_FontSelect(&FONT_8X12);
//			UG_SetForecolor(C_BLACK);
//			UG_SetBackcolor(C_WHITE);
//			UG_FillFrame(0, 0, 25, 25, C_WHITE);
//			UG_PutString(0, 0, "abc");
//			overlay.visible = 1;
//		}



/*		if(overlay.visible && overlay.frames>0){
			overlay.frames--;
			if(overlay.frames==0){
				overlay.visible = 0;
			}
		}
*/
		
        lastJoysticState = joystick;
	
/*
        if (stopTime > startTime)
          elapsedTime = (stopTime - startTime);
        else
          elapsedTime = ((uint64_t)stopTime + (uint64_t)0xffffffff) - (startTime);

        totalElapsedTime += elapsedTime;
        ++frame;
        ++actualFrameCount;

        if (actualFrameCount == 60)
        {
          float seconds = totalElapsedTime / (CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ * 1000000.0f); // 240000000.0f; // (240Mhz)
          float fps = actualFrameCount / seconds;

          printf("HEAP:0x%x, FPS:%f, BATTERY:%d [%d]\n", esp_get_free_heap_size(), fps, battery_state.millivolts, battery_state.percentage);

          actualFrameCount = 0;
          totalElapsedTime = 0;
        }
		*/
    }
}
