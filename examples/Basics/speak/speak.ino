/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/core2
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/core2
*
* describe：Speaker example.  喇叭示例
* date：2022/3/12
*******************************************************************************
*/
#include <M5Core2.h>
#include <driver/i2s.h>

extern const unsigned char previewR[120264];  // Referring to external data (Dingdong audio files are
                                              // stored inside).
                                              // 引用外部数据（dingdong音频文件存储在内）

#define CONFIG_I2S_BCK_PIN     12  // 定义I2S相关端口
#define CONFIG_I2S_LRCK_PIN    0
#define CONFIG_I2S_DATA_PIN    2
#define CONFIG_I2S_DATA_IN_PIN 34

#define Speak_I2S_NUMBER I2S_NUM_0  // 定义扬声器端口

#define MODE_MIC  0  // 定义工作模式
#define MODE_SPK  1
#define DATA_SIZE 1024

bool InitI2SSpeakOrMic(int mode)
{  // Init I2S.  初始化I2S
    esp_err_t err = ESP_OK;

    i2s_driver_uninstall(Speak_I2S_NUMBER);  // Uninstall the I2S driver.  卸载I2S驱动
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER),         // Set the I2S operating mode.
                                                       // 设置I2S工作模式
        .sample_rate     = 44100,                      // Set the I2S sampling rate.  设置I2S采样率
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,  // Fixed 12-bit stereo MSB.
                                                       // 固定为12位立体声MSB
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,  // Set the channel format. 设置频道格式
#if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 1, 0)
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,  // Set the format of the communication.
#else                                                       // 设置通讯格式
        .communication_format = I2S_COMM_FORMAT_I2S,
#endif
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,  // Set the interrupt flag.  设置中断的标志
        .dma_buf_count    = 2,                     // DMA buffer count.  DMA缓冲区计数
        .dma_buf_len      = 128,                   // DMA buffer length.  DMA缓冲区长度
    };
    if (mode == MODE_MIC) {
        i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
    } else {
        i2s_config.mode               = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
        i2s_config.use_apll           = false;  // I2S clock setup.  I2S时钟设置
        i2s_config.tx_desc_auto_clear = true;   // Enables auto-cleanup descriptors for understreams.
                                                // 开启欠流自动清除描述符
    }
    // Install and drive I2S.  安装并驱动I2S
    err += i2s_driver_install(Speak_I2S_NUMBER, &i2s_config, 0, NULL);

    i2s_pin_config_t tx_pin_config;

#if (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 3, 0))
    tx_pin_config.mck_io_num = I2S_PIN_NO_CHANGE;
#endif

    tx_pin_config.bck_io_num = CONFIG_I2S_BCK_PIN;        // Link the BCK to the CONFIG_I2S_BCK_PIN pin.
                                                          // 将BCK链接至CONFIG_I2S_BCK_PIN引脚
    tx_pin_config.ws_io_num    = CONFIG_I2S_LRCK_PIN;     //          ...
    tx_pin_config.data_out_num = CONFIG_I2S_DATA_PIN;     //       ...
    tx_pin_config.data_in_num  = CONFIG_I2S_DATA_IN_PIN;  //      ...
    err += i2s_set_pin(Speak_I2S_NUMBER,
                       &tx_pin_config);  // Set the I2S pin number. 设置I2S引脚编号
    err += i2s_set_clk(Speak_I2S_NUMBER, 44100, I2S_BITS_PER_SAMPLE_16BIT,
                       I2S_CHANNEL_MONO);  // Set the clock and bitwidth used by I2S Rx and Tx.
                                           // 设置I2S RX、Tx使用的时钟和位宽
    return true;
}

void DisplayInit(void)
{                                // Initialize the display. 显示屏初始化
    M5.Lcd.fillScreen(WHITE);    // Set the screen background color to white.
                                 // 设置屏幕背景色为白色
    M5.Lcd.setTextColor(BLACK);  // Set the text color to black.  设置文字颜色为黑色
    M5.Lcd.setTextSize(2);       // Set font size to 2.  设置字体大小为2
}

void SpeakInit(void)
{                               // 初始化扬声器
    M5.Axp.SetSpkEnable(true);  // 启用扬声器电源
    InitI2SSpeakOrMic(MODE_SPK);
}

void DingDong(void)
{
    size_t bytes_written = 0;
    i2s_write(Speak_I2S_NUMBER, previewR, 120264, &bytes_written, portMAX_DELAY);
}

// 在 M5Core
// 启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。
void setup()
{
    delay(1000);
    M5.begin(true, true, true, true);  // Init M5Core2.  初始化 M5Core2
    DisplayInit();
    M5.Lcd.setTextColor(RED);
    M5.Lcd.setCursor(10,
                     10);          // Set the cursor at (10,10).  将光标设在（10，10）处
    M5.Lcd.printf("Speak Test!");  // The screen prints the formatted string and
                                   // wraps it.  屏幕打印格式化字符串并换行
    M5.Lcd.fillRect(0, 210, 320, 30, GREEN);
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.setCursor(10, 220);
    M5.Lcd.printf("Press to listen DingDong!");

    SpeakInit();
    DingDong();
    delay(100);
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop()
{
    TouchPoint_t pos = M5.Touch.getPressPoint();  // Stores the touch coordinates in pos.
    // 将触摸坐标存储在pos.内
    Serial.println(pos);
    if (pos.y > 210) {
        delay(100);
        DingDong();
    }
    delay(10);
}
