// #include <stdio.h>
// #include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_log.h"
// #include "hal/gpio_types.h"
// #include "driver/gpio.h"
// #include "freertos/ringbuf.h"
// #include "freertos/semphr.h"

// //Thu vien I2S
// //#include "driver/i2s.h" //API cu
// #include <driver/i2s_std.h> //API moi <- Dung cai nay
// #include <driver/i2s_types_legacy.h>
// #include "driver/i2s_types.h"
// #include "driver/i2s_common.h"

// //Thu vien SD card 
// #include "esp_vfs_fat.h"
// #include "sdmmc_cmd.h"
// #include "driver/sdmmc_types.h"
// #include "sdcard.h"

// //Thu vien cho SPI        
// #include <driver/spi_common.h>
// #include "driver/sdspi_host.h"
// #include  <driver/spi_master.h>

// //Include MAX30102 driver
// #include "max30102.h"

// #define TAG1 "INMP441"
// #define TAG2 "MAX30102"

// //Pin cau hinh INMP441 (I2S Mode)
// #define I2S_NUM I2S_NUM_0
// #define I2S_SCK_PIN 32              
// #define I2S_WS_PIN 25
// #define I2S_SD_PIN 33

// //Pin cau hinh MAX30102
// #define I2C_SDA_PIN 21
// #define I2C_SCL_PIN 22

// //Pin cau hinh SD Card (SPI Mode)
// #define MISO_PIN 19
// #define MOSI_PIN 23
// #define SCLK_PIN 18
// #define CS_PIN 5

// #define SAMPLE_RATE 4000 //Tan so lay mau 4000Hz
// #define dmaDesc 6
// #define dmaLen 32
// #define SAMPLE_BUFFER_SIZE (dmaLen * dmaDesc) //So byte cua buffer DMA cung cap cho = 192

// //Buffer cho data de luu vao SD card
// RingbufHandle_t buf_handle_inmp441;
// RingbufHandle_t buf_handle_max30102;

// TaskHandle_t readINMP441_handle = NULL;
// TaskHandle_t readMAX30102_handle = NULL;
// TaskHandle_t printData_handle = NULL;

// //Tao kenh rx cho i2s
// i2s_chan_handle_t rx_channel = NULL;

// //Bien toan cuc luu gia tri INMP441 va MAX30102
// // volatile unsigned long global_red = 0;
// // volatile unsigned long global_ir = 0;
// // volatile int16_t global_inmp441_data = 0;

// SemaphoreHandle_t print_mutex; 
// QueueHandle_t queue_inmp441;
// QueueHandle_t queue_max30102;

// void readINMP441_task(void *pvParameter);
// void readMAX30102_task(void *pvParameter);
// void printData_task(void *pvParameter);

// //Khoi tao giao thuc I2S
//  esp_err_t i2s_install(void){
//     ESP_LOGI(TAG1, "Cau hinh kenh i2s...");

//     //Cau hinh kenh rx cho i2s
//     i2s_chan_config_t chan_cfg = {
//         .id = I2S_NUM,
//         .role = I2S_ROLE_MASTER,
//         .dma_desc_num = dmaDesc, //6 * 32 = 192 bytes
//         .dma_frame_num = dmaLen, //32 bytes 
//         .auto_clear = true,
//     };

//     //Khoi tao RX channel va kiem tra loi 
//     ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, NULL, &rx_channel));

//     //Cau hinh che do i2s_std
//     i2s_std_config_t std_cfg = {
//         //Cau hinh clock
//         .clk_cfg = {
//             .sample_rate_hz = SAMPLE_RATE,
//             .clk_src = I2S_CLK_SRC_DEFAULT,
//             .mclk_multiple = I2S_MCLK_MULTIPLE_384,
//         },
//         //Cau hinh du lieu trong 1 frame
//         .slot_cfg = {
//             .data_bit_width = I2S_DATA_BIT_WIDTH_32BIT,
//             .slot_mode = I2S_SLOT_MODE_MONO,
//             .slot_bit_width = I2S_SLOT_BIT_WIDTH_32BIT,
//             .slot_mask = I2S_STD_SLOT_LEFT,
//         },
//         //Cau hinh GPIO
//         .gpio_cfg = {
//             .bclk = I2S_SCK_PIN,
//             .ws = I2S_WS_PIN,
//             .dout = I2S_PIN_NO_CHANGE,
//             .din = I2S_SD_PIN,
//             .mclk = GPIO_NUM_0,
//         }
//     };

//     //Khoi tao che do chuan 
//     ESP_ERROR_CHECK(i2s_channel_init_std_mode(rx_channel, &std_cfg));
//     //Bat kenh RX
//     ESP_ERROR_CHECK(i2s_channel_enable(rx_channel));

//     ESP_LOGI(TAG1, "I2S da duoc cau hinh thanh cong!");
//     return ESP_OK;
//  }

//  //Task de doc du lieu INMP441 su dung buffer
//  void readINMP441_task(void *pvParameter){
//     ESP_LOGI(TAG1, "Bat dau doc du lieu tu INMP441...");
//     size_t bytes_Read = 0;
//     int16_t samplesRead = bytes_Read / sizeof(int32_t);
//     //Buffer de luu tru du lieu doc duoc tu buffer DMA
//     //Chuyen doi tu byte DMA sang so luong mau cua moi buffer 
//     int16_t buffer16[SAMPLE_BUFFER_SIZE / sizeof(int32_t) * 3 / 2] = {0}; //72 samples (144 bytes) de luu duoc 3 bytes sau khi dich cua buffer32
//     int32_t buffer32[SAMPLE_BUFFER_SIZE / sizeof(int32_t)] = {0}; //48 samples (192 bytes)

//     while(1){
//         vTaskDelay(1);
//         esp_err_t ret = i2s_channel_read(rx_channel, &buffer32, sizeof(buffer32), &bytes_Read, pdMS_TO_TICKS(500));
//         if(ret == ESP_OK){
//             for(size_t i = 0; i < samplesRead; i++){
//                 buffer16[i] = (int16_t)(buffer32[i] >> 8);
//                 xQueueSend(queue_inmp441, &buffer16[i], portMAX_DELAY);
//                 ESP_LOGI(TAG1, "INMP441 Raw Data: %d", buffer16[i]);
//             }
//         }else{
//             ESP_LOGE(TAG1, "Loi doc du lieu I2S: %s", esp_err_to_name(ret));
//         }
//         //vTaskDelay(10);
//     }
// }

// //Khoi tao I2C va doc du lieu MAX30102
// void readMAX30102_task(void *pvParameters){
//     i2c_dev_t dev; 
//     memset(&dev, 0, sizeof(i2c_dev_t));
//     ESP_ERROR_CHECK(max30102_initDesc(&dev, 0, I2C_SDA_PIN, I2C_SCL_PIN));

//     struct max30102_record record;
//     if(max30102_readPartID(&dev) == ESP_OK){
//         ESP_LOGI(TAG2, "Ket noi thanh cong MAX30102...");
//     }else{
//         ESP_LOGE(TAG2, "Khong ket noi duoc MAX30102...");
//     }
//     uint8_t powerLed = 0x1F;
//     uint8_t sampleAverage = 4;
//     uint8_t ledMode = 2;
//     int sampleRate = 500;
//     int pulseWidth = 411;
//     int adcRange = 16384;
//     ESP_ERROR_CHECK(max30102_init(powerLed, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange, &record, &dev));

//     while(1){
//         max30102_check(&record, &dev); 
//         while(max30102_available(&record)){
//             unsigned long red = max30102_getFIFORed(&record);
//             unsigned long ir = max30102_getFIFOIR(&record);
//             unsigned long data[2] = {red, ir};
//             xQueueSend(queue_max30102, data, portMAX_DELAY);
//             ESP_LOGI(TAG2, "MAX30102 Raw Data: RED:%lu, IR:%lu", red, ir);
//             max30102_nextSample(&record);
//         }
//         vTaskDelay(100);
//     }
// }

// //In du lieu thanh 3 cot
// void printData_task(void *pvParameter){
//     while(1){
//         int16_t inmp441_data;
//         unsigned long max_data[2];
//         if(xQueueReceive(queue_inmp441, &inmp441_data, 0) == pdTRUE 
//         && xQueueReceive(queue_max30102, &max_data, 0) == pdTRUE){
//             if(xSemaphoreTake(print_mutex, pdMS_TO_TICKS(100)) == pdTRUE){
//                 printf("%d,%lu,%lu\n", inmp441_data, max_data[0], max_data[1]);
//                 xSemaphoreGive(print_mutex);
//             }
//         }
//         vTaskDelay(pdMS_TO_TICKS(50));
//     }
// }
// void app_main(void){
//     ESP_LOGI(TAG1, "Dang khoi tao I2S...");
//     ESP_ERROR_CHECK(i2s_install());
//     ESP_ERROR_CHECK(i2cdev_init());

//     queue_inmp441 = xQueueCreate(10, sizeof(int16_t));
//     queue_max30102 = xQueueCreate(10, sizeof(unsigned long) * 2);   
//     print_mutex = xSemaphoreCreateMutex();
//     if(!print_mutex || !queue_inmp441 || !queue_max30102){
//         ESP_LOGE("MAIN", "Khong the khoi tao Queues va Mutex");
//         return;
//     }
//     //Uu tien inmp441 vi co toc do lay mau cao hon
//     xTaskCreatePinnedToCore(readINMP441_task, "readINMP441", 1023 * 15, NULL, 6,  &readINMP441_handle, 0); 
//     //Cho max su dung core khac de tranh tranh chap tai nguyen CPU
//     xTaskCreatePinnedToCore(readMAX30102_task, "readMAX30102", 1023 * 5, NULL, 5, &readMAX30102_handle, 1);
//     xTaskCreatePinnedToCore(printData_task, "printData", 2048, NULL, 6, &printData_handle, 1);
// }
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "hal/gpio_types.h"
#include "driver/gpio.h"
#include "freertos/ringbuf.h"
#include "freertos/semphr.h"

//Thu vien I2S
//#include "driver/i2s.h" //API cu
#include <driver/i2s_std.h> //API moi <- Dung cai nay
#include <driver/i2s_types_legacy.h>
#include "driver/i2s_types.h"
#include "driver/i2s_common.h"

//Thu vien SD card 
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_types.h"

//Thu vien cho SPI        
#include <driver/spi_common.h>
#include "driver/sdspi_host.h"
#include  <driver/spi_master.h>

//Include MAX30102 driver
#include "max30102.h"

#define TAG1 "INMP441"
#define TAG2 "MAX30102"

//Pin cau hinh INMP441 (I2S Mode)
#define I2S_NUM I2S_NUM_0
#define I2S_SCK_PIN 32              
#define I2S_WS_PIN 25
#define I2S_SD_PIN 33

//Pin cau hinh MAX30102 (I2C Mode)
#define I2C_SCL_PIN 22
#define I2C_SDA_PIN 21

//Pin cau hinh SD Card (SPI Mode)
#define MISO_PIN 19
#define MOSI_PIN 23
#define SCLK_PIN 18
#define CS_PIN 5

#define SAMPLE_RATE 4000 //Tan so lay mau 4000Hz
#define dmaDesc 6
#define dmaLen 32
#define SAMPLE_BUFFER_SIZE (dmaLen * dmaDesc) //So byte cua buffer DMA cung cap cho = 192

//Buffer de luu tru du lieu doc duoc tu buffer DMA
//Chuyen doi tu byte DMA sang so luong mau cua moi buffer 
int16_t buffer16[SAMPLE_BUFFER_SIZE / sizeof(int32_t) * 3 / 2] = {0}; //72 samples (144 bytes) de luu duoc 3 bytes sau khi dich cua buffer32
int32_t buffer32[SAMPLE_BUFFER_SIZE / sizeof(int32_t)] = {0}; //48 samples (192 bytes)

//Buffer cho data de luu vao SD card
RingbufHandle_t buf_handle_inmp441;
RingbufHandle_t buf_handle_max30102;

TaskHandle_t readINMP441_handle = NULL;
TaskHandle_t readMAX30102_handle = NULL;
TaskHandle_t printData_handle = NULL;

//Tao kenh rx cho i2s
i2s_chan_handle_t rx_channel = NULL;

//Bien toan cuc luu gia tri INMP441 va MAX30102
volatile unsigned long global_red = 0;
volatile unsigned long global_ir = 0;
volatile int16_t global_inmp441_data = 0;

//Semaphore de dong bo 
SemaphoreHandle_t print_mutex; 

void readINMP441_task(void *pvParameter);
void readMAX30102_task(void *pvParameter);
void printData_task(void *pvParameter);

//Khoi tao giao thuc I2S
esp_err_t i2s_install(void){
    ESP_LOGI(TAG1, "Cau hinh kenh i2s...");

    //Cau hinh kenh rx cho i2s
    i2s_chan_config_t chan_cfg = {
        .id = I2S_NUM,
        .role = I2S_ROLE_MASTER,
        .dma_desc_num = dmaDesc, //6 * 32 = 192 bytes
        .dma_frame_num = dmaLen, //32 bytes 
        .auto_clear = true,
    };

    //Khoi tao RX channel va kiem tra loi 
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, NULL, &rx_channel));

    //Cau hinh che do i2s_std
    i2s_std_config_t std_cfg = {
        //Cau hinh clock
        .clk_cfg = {
            .sample_rate_hz = SAMPLE_RATE,
            .clk_src = I2S_CLK_SRC_DEFAULT,
            .mclk_multiple = I2S_MCLK_MULTIPLE_384,
        },
        //Cau hinh du lieu trong 1 frame
        .slot_cfg = {
            .data_bit_width = I2S_DATA_BIT_WIDTH_32BIT,
            .slot_mode = I2S_SLOT_MODE_MONO,
            .slot_bit_width = I2S_SLOT_BIT_WIDTH_32BIT,
            .slot_mask = I2S_STD_SLOT_LEFT,
        },
        //Cau hinh GPIO
        .gpio_cfg = {
            .bclk = I2S_SCK_PIN,
            .ws = I2S_WS_PIN,
            .dout = I2S_PIN_NO_CHANGE,
            .din = I2S_SD_PIN,
            .mclk = GPIO_NUM_0,
        }
    };

    //Khoi tao che do chuan 
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(rx_channel, &std_cfg));
    //Bat kenh RX
    ESP_ERROR_CHECK(i2s_channel_enable(rx_channel));

    ESP_LOGI(TAG1, "I2S da duoc cau hinh thanh cong!");
    return ESP_OK;
 }

 //Task de doc du lieu INMP441 su dung buffer
 void readINMP441_task(void *pvParameter){
    ESP_LOGI(TAG1, "Bat dau doc du lieu tu INMP441...");
    size_t bytes_Read = 0;
    while(1){
        vTaskDelay(1);
        esp_err_t ret = i2s_channel_read(rx_channel, &buffer32, sizeof(buffer32), &bytes_Read, pdMS_TO_TICKS(500));
        if(ret == ESP_ERR_TIMEOUT){
            ESP_LOGE(TAG1, "INMP441, Loi timeout: %s", esp_err_to_name(ret));
            continue;
        }else if(ret != ESP_OK){
            ESP_LOGE(TAG1, "INMP441, Loi khong xac dinh: %s", esp_err_to_name(ret));
            break;
        }
        int16_t samplesRead = bytes_Read / sizeof(int32_t);
        for(size_t i = 0; i < samplesRead; i++){
            buffer16[i] = (int16_t)(buffer32[i] >> 8);
            global_inmp441_data = buffer16[i];
        }
    }
}

//Khoi tao I2C va doc du lieu MAX30102
void readMAX30102_task(void *pvParameters){
    i2c_dev_t dev;  
    memset(&dev, 0, sizeof(i2c_dev_t));
    ESP_ERROR_CHECK(max30102_initDesc(&dev, 0, I2C_SDA_PIN, I2C_SCL_PIN));
    struct max30102_record record;

    if(max30102_readPartID(&dev) == ESP_OK){
        ESP_LOGI(TAG2, "Ket noi thanh cong MAX30102...");
    }else{
        ESP_LOGE(TAG2, "Khong ket noi duoc MAX30102...");
    }
    uint8_t powerLed = 0x1F;
    uint8_t sampleAverage = 4;
    uint8_t ledMode = 2;
    int sampleRate = 500;
    int pulseWidth = 411;
    int adcRange = 16384;

    ESP_ERROR_CHECK(max30102_init(powerLed, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange, &record, &dev));

    while(1){
        max30102_check(&record, &dev); 
        while(max30102_available(&record)){
            global_red = max30102_getFIFORed(&record);
            global_ir = max30102_getFIFOIR(&record);
            max30102_nextSample(&record);
        }
    }
}

//In du lieu thanh 3 cot
void printData_task(void *pvParameter){
    while(1){
        //Lay mutex truoc khi in
        if(xSemaphoreTake(print_mutex, pdMS_TO_TICKS(100)) == pdTRUE){
            printf("%d,%lu,%lu\n", global_inmp441_data, global_red, global_ir);
            xSemaphoreGive(print_mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
void app_main(void){
    ESP_LOGI(TAG1, "Dang khoi tao I2S...");
    ESP_ERROR_CHECK(i2s_install());
    ESP_ERROR_CHECK(i2cdev_init());

    //Khoi tao Semaphore
    print_mutex = xSemaphoreCreateMutex();
    if(print_mutex == NULL){
        ESP_LOGE("MAIN", "Khong the khoi tao Mutex");
        return;
    }
    //Uu tien inmp441 vi co toc do lay mau cao hon
    xTaskCreatePinnedToCore(readINMP441_task, "readINMP441", 1023 * 15, NULL, 6,  &readINMP441_handle, 0); 
    //Cho max su dung core khac de tranh tranh chap tai nguyen CPU
    xTaskCreatePinnedToCore(readMAX30102_task, "readMAX30102", 1023 * 5, NULL, 5, &readMAX30102_handle, 1);
    xTaskCreatePinnedToCore(printData_task, "printData", 2048, NULL, 6, &printData_handle, 1);
}   