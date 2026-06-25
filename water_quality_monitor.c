#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "mqtt_client.h"

#define WIFI_SSID        "YourWiFiSSID"
#define WIFI_PASSWORD    "YourWiFiPassword"
#define BLYNK_AUTH       "YourBlynkAuthToken"
#define BLYNK_HOST       "blynk.cloud"
#define BLYNK_PORT       80

#define TDS_ADC_CHANNEL       ADC1_CHANNEL_6
#define TURBIDITY_ADC_CHANNEL ADC1_CHANNEL_7
#define ALERT_LED_PIN         GPIO_NUM_2

#define SAFE                  0
#define MODERATELY_POLLUTED   1
#define SEVERELY_CONTAMINATED 2

typedef struct {
    float tds;
    float turbidity;
    int   quality;
    char  label[30];
} WaterData;

static const char *TAG = "WaterQuality";

void wifi_init(void);
void gpio_init(void);
void adc_init(void);
float read_tds(void);
float read_turbidity(void);
int   classify_water(float tds, float turbidity);
void  get_quality_label(int quality, char *label);
void  trigger_alert(int quality);
void  print_status(WaterData *data);
void  water_monitor_task(void *pvParameter);

void gpio_init(void)
{
    gpio_config_t io_conf;
    io_conf.intr_type    = GPIO_INTR_DISABLE;
    io_conf.mode         = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << ALERT_LED_PIN);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en   = 0;
    gpio_config(&io_conf);
    gpio_set_level(ALERT_LED_PIN, 0);
}

void adc_init(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(TDS_ADC_CHANNEL,       ADC_ATTEN_DB_11);
    adc1_config_channel_atten(TURBIDITY_ADC_CHANNEL, ADC_ATTEN_DB_11);
}

void wifi_init(void)
{
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid     = WIFI_SSID,
            .password = WIFI_PASSWORD,
        },
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
    esp_wifi_connect();

    ESP_LOGI(TAG, "Connecting to WiFi: %s", WIFI_SSID);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "WiFi Connected!");
}

float read_tds(void)
{
    int   raw = adc1_get_raw(TDS_ADC_CHANNEL);
    float tds = (raw / 4095.0f) * 1000.0f;
    return tds;
}

float read_turbidity(void)
{
    int   raw       = adc1_get_raw(TURBIDITY_ADC_CHANNEL);
    float turbidity = (raw / 4095.0f) * 100.0f;
    return turbidity;
}

int classify_water(float tds, float turbidity)
{
    if (tds < 300.0f && turbidity < 10.0f)
        return SAFE;
    else if (tds < 600.0f && turbidity < 50.0f)
        return MODERATELY_POLLUTED;
    else
        return SEVERELY_CONTAMINATED;
}

void get_quality_label(int quality, char *label)
{
    if (quality == SAFE)
        strcpy(label, "SAFE");
    else if (quality == MODERATELY_POLLUTED)
        strcpy(label, "MODERATELY POLLUTED");
    else
        strcpy(label, "SEVERELY CONTAMINATED");
}

void trigger_alert(int quality)
{
    if (quality == SAFE)
        gpio_set_level(ALERT_LED_PIN, 0);
    else
        gpio_set_level(ALERT_LED_PIN, 1);
}

void print_status(WaterData *data)
{
    ESP_LOGI(TAG, "------------------------------");
    ESP_LOGI(TAG, "TDS Value   : %.2f ppm",  data->tds);
    ESP_LOGI(TAG, "Turbidity   : %.2f NTU",  data->turbidity);
    ESP_LOGI(TAG, "Quality Code: %d",         data->quality);
    ESP_LOGI(TAG, "Status      : %s",         data->label);
    ESP_LOGI(TAG, "------------------------------");
}

void water_monitor_task(void *pvParameter)
{
    WaterData data;

    while (1)
    {
        data.tds       = read_tds();
        data.turbidity = read_turbidity();
        data.quality   = classify_water(data.tds, data.turbidity);
        get_quality_label(data.quality, data.label);

        print_status(&data);
        trigger_alert(data.quality);

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    gpio_init();
    adc_init();
    wifi_init();

    xTaskCreate(
        water_monitor_task,
        "water_monitor_task",
        4096,
        NULL,
        5,
        NULL
    );
}
