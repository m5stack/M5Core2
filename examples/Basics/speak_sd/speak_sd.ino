/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 *
 * Play wav file from SD card
 *
 ****************************************************************
 * Prepare a file named "resources_speak_sd.wav" in the root of your SD card
 * Wav file format : 44100Hz 16bit mono
 *
 * Download it from https://github.com/m5stack/M5Core2/wiki
 * or you can make it.
 *   Example of conversion using ffmpeg
 *   ffmpeg -i input.wav -vn -ac 1 -ar 44100 -acodec pcm_s16le -f wav resources_speak_sd.wav
 ****************************************************************
*/
#include <M5Core2.h>

#define WAV_FILE_NAME "/resources_speak_sd.wav"

File wavFile;
uint8_t buffer[1024];
bool play{}, play_loop{true};

struct __attribute__((packed)) wav_header_t {
    char RIFF[4];
    uint32_t chunk_size;
    char WAVEfmt[8];
    uint32_t fmt_chunk_size;
    uint16_t audiofmt;
    uint16_t channel;
    uint32_t sample_rate;
    uint32_t byte_per_sec;
    uint16_t block_size;
    uint16_t bit_per_sample;
};
struct __attribute__((packed)) sub_chunk_t {
    char identifier[4];
    uint32_t chunk_size;
};

// Seek to the beginning of the audio data
bool seek_file(File& f) {
    f.seek(0);

    // Read header
    wav_header_t wheader{};
    if (f.read((uint8_t*)&wheader, sizeof(wheader)) != sizeof(wheader)) {
        return false;
    }

#if 0
    ESP_LOGI("wav", "RIFF           : %.4s" , wheader.RIFF          );
    ESP_LOGI("wav", "chunk_size     : %d"   , wheader.chunk_size    );
    ESP_LOGI("wav", "WAVEfmt        : %.8s" , wheader.WAVEfmt       );
    ESP_LOGI("wav", "fmt_chunk_size : %d"   , wheader.fmt_chunk_size);
    ESP_LOGI("wav", "audiofmt       : %d"   , wheader.audiofmt      );
    ESP_LOGI("wav", "channel        : %d"   , wheader.channel       );
    ESP_LOGI("wav", "sample_rate    : %d"   , wheader.sample_rate   );
    ESP_LOGI("wav", "byte_per_sec   : %d"   , wheader.byte_per_sec  );
    ESP_LOGI("wav", "block_size     : %d"   , wheader.block_size    );
    ESP_LOGI("wav", "bit_per_sample : %d"   , wheader.bit_per_sample);
#endif

    // Check format
    // 44100 16bit mono linear PCM
    if (memcmp(wheader.RIFF, "RIFF", 4) ||
        memcmp(wheader.WAVEfmt, "WAVEfmt ", 8) || wheader.audiofmt != 1 ||
        wheader.sample_rate != 44100 || wheader.bit_per_sample != 16 ||
        wheader.channel != 1) {
        Serial.printf("Illegal format");
        return false;
    }

    // Find data chunk
    sub_chunk_t c{};
    while (true) {
        if (f.read((uint8_t*)&c, sizeof(c)) != sizeof(c)) {
            return false;
        }
        if (memcmp(c.identifier, "data", 4) == 0) {
            break;
        }
        f.seek(f.position() + c.chunk_size);
    }
    return true;
}

void streaming(File& f) {
    // No more read
    if (!f.available()) {
        // rewind to top (playback loop)
        if (play_loop) {
            seek_file(f);
        }
        return;
    }
    // Read and play
    auto readed = f.read(buffer, sizeof(buffer));
    // I2S write is blocking until the end of write
    M5.Spk.PlaySound(buffer, readed);
}

void setup() {
    // LCDEnable,SDEnable,SerialEnable, I2CEnable, mbus_mode_t,SpeakerEnable
    M5.begin(true, true, true, true, mbus_mode_t::kMBusModeOutput, true);

    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.printf("Speak Test!");

    wavFile = SD.open(WAV_FILE_NAME);
    if (!wavFile) {
        M5.Lcd.clear(TFT_RED);
        M5.Lcd.setCursor(10, 10);
        M5.Lcd.printf("Failed to open");
        while (true) {
            delay(1000);
        }
    }

    play = seek_file(wavFile);
    if (!play) {
        M5.Lcd.setCursor(10, 10);
        M5.Lcd.printf("Illegal format");
    }
    M5.Lcd.clear(play ? TFT_DARKGREEN : TFT_PURPLE);
}

void loop() {
    if (play) {
        streaming(wavFile);

        auto pos = M5.Touch.getPressPoint();
        if (pos.x >= 0 && pos.x < M5.Lcd.width() && pos.y >= 0 &&
            pos.y < M5.Lcd.height()) {
            Serial.println("Repeat");
            seek_file(wavFile);
        }
    }
}
