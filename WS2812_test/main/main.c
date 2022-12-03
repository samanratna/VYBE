#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "led_strip.h"

static const char *TAG = "example";

#define RMT_TX_CHANNEL RMT_CHANNEL_0

#define EXAMPLE_CHASE_SPEED_MS (10)
#define LED_NUMBER 59

#define IDEAL 1
#define BREATHING 2
#define CLEAR 3
#define RUNNING_TRAIL 4

float f;
float bright = 0.5;
float f1;
int count = 0;
uint8_t valRed = 255;
uint8_t valGreen = 255;
uint8_t valBlue = 255;
uint8_t LEDState;



void app_main(void)
{
    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(32, RMT_TX_CHANNEL);
    // set counter clock to 40MHz
    config.clk_div = 2;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    // install ws2812 driver
    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(LED_NUMBER, (led_strip_dev_t)config.channel);
    led_strip_t *strip = led_strip_new_rmt_ws2812(&strip_config);
    if(!strip) 
    {
        ESP_LOGE(TAG, "install WS2812 driver failed");
    }
    // Clear LED strip (turn off all LEDs)
    ESP_ERROR_CHECK(strip->clear(strip, 100)); 
    vTaskDelay(pdMS_TO_TICKS(50));

LEDState = IDEAL;
   
    while(1)
    {
        switch(LEDState)
        {
          case IDEAL:
                // printf("ideal\n");
                for (int i = 0; i < LED_NUMBER; i++) 
                {
                    // ESP_ERROR_CHECK(strip->set_pixel(strip, i, 48, 245, 185)); //turquoise color
                    // ESP_ERROR_CHECK(strip->set_pixel(strip, i, 48, 255, 140)); //turquoise color  strip
                    ESP_ERROR_CHECK(strip->set_pixel(strip, i, bright*valRed, bright*valGreen, bright*valBlue)); //turquoise color  strip
                }
                ESP_ERROR_CHECK(strip->refresh(strip, 50));
                break;

          case BREATHING:
                //printf("charging\n");
                for(int j = 1; j < 35; j ++)
                {
                    f = 0.025 * j;
                    for (int i = 0; i < LED_NUMBER; i++) 
                    {
                        // ESP_ERROR_CHECK(strip->set_pixel(strip, i, f*48, f*245, f*185)); //turquoise color
                        // ESP_ERROR_CHECK(strip->set_pixel(strip, i, f*48, f*255, f*140)); //turquoise color  strip
                        ESP_ERROR_CHECK(strip->set_pixel(strip, i, f*valRed, f*valGreen, f*valBlue)); //turquoise color  strip

                    }
                    ESP_ERROR_CHECK(strip->refresh(strip, 50));
                    vTaskDelay(pdMS_TO_TICKS(50));
                }

                for(int j = 35; j >= 1; j--)
                {
                    f = 0.025 * j;
                    for (int i = 0; i < LED_NUMBER; i ++) 
                    {
                        // ESP_ERROR_CHECK(strip->set_pixel(strip, i, f*48, f*245, f*185));
                        // ESP_ERROR_CHECK(strip->set_pixel(strip, i, f*48, f*255, f*140)); //strip
                        ESP_ERROR_CHECK(strip->set_pixel(strip, i, f*valRed, f*valGreen, f*valBlue)); //strip
                    }
                    ESP_ERROR_CHECK(strip->refresh(strip, 50));
                    vTaskDelay(pdMS_TO_TICKS(50));
                } 
                strip->clear(strip, 20);
                // vTaskDelay(1);

                break;
          case CLEAR:
                printf("clear\n");
                for (int i = 0; i < LED_NUMBER; i++) 
                {
                    ESP_ERROR_CHECK(strip->set_pixel(strip, i, 0, 0, 0));
                }
                ESP_ERROR_CHECK(strip->refresh(strip, 10)); 
                strip->clear(strip, 20);
                break;
                // vTaskDelay(1);

          case RUNNING_TRAIL:
                for(int j = 0; j < LED_NUMBER - 10; j++)
                {
                    for(int i = j; i < j + 6; i++)
                    {

                        ESP_ERROR_CHECK(strip->set_pixel(strip, i, 0.1*160, 0.1*50, 0.1*250));
                        ESP_ERROR_CHECK(strip->set_pixel(strip, i+1, 0.5*160, 0.5*50, 0.5*250));
                        ESP_ERROR_CHECK(strip->set_pixel(strip, i+2, 0.7*160, 0.7*50, 0.7*250));
                        ESP_ERROR_CHECK(strip->set_pixel(strip, i+3, 160, 50, 250));
                        ESP_ERROR_CHECK(strip->set_pixel(strip, i+4, 160, 50, 250));
                        ESP_ERROR_CHECK(strip->set_pixel(strip, i+5, 160, 50, 250));
                    }
                    ESP_ERROR_CHECK(strip->refresh(strip, 100));
                    vTaskDelay(pdMS_TO_TICKS(10));
                    ESP_ERROR_CHECK(strip->set_pixel(strip, j, 0, 0, 0));
                    ESP_ERROR_CHECK(strip->refresh(strip, 10));   
                } 
                strip->clear(strip, 10); 
                for(int j = LED_NUMBER - 10; j >= 10; j--)
                {
                    for(int i = j; i > j - 6; i--)
                    {
                        ESP_ERROR_CHECK(strip->set_pixel(strip, i-5, 0.1*160, 0.1*50, 0.1*250));
                        ESP_ERROR_CHECK(strip->set_pixel(strip, i-4, 0.5*160, 0.5*50, 0.5*250));
                        ESP_ERROR_CHECK(strip->set_pixel(strip, i-3, 0.7*160, 0.7*50, 0.7*250));
                        ESP_ERROR_CHECK(strip->set_pixel(strip, i-2, 160, 50, 250));
                        ESP_ERROR_CHECK(strip->set_pixel(strip, i-1, 160, 50, 250));
                        ESP_ERROR_CHECK(strip->set_pixel(strip, i, 160, 50, 250));
                    }
                    ESP_ERROR_CHECK(strip->refresh(strip, 100));
                    vTaskDelay(pdMS_TO_TICKS(10));
                    ESP_ERROR_CHECK(strip->set_pixel(strip, j, 0, 0, 0));
                    ESP_ERROR_CHECK(strip->refresh(strip, 10));   
                } 
                strip->clear(strip, 10); 
                vTaskDelay(1);
                break;
        }
        vTaskDelay(1);
    }
}