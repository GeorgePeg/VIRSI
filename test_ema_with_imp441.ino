// Include I2S driver
#include <driver/i2s.h>
#include <math.h> // We need it for the function abs()

// Connections to INMP441 I2S microphone
#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 32

// Use I2S Processor 0
#define I2S_PORT I2S_NUM_0

// Define input buffer length
#define bufferLen 64
int16_t sBuffer[bufferLen];

//Compressor properties
const float THRESHOLD = 0.7;   // Threshold (from 0.0 to 1.0)
const float RATIO = 4.0;       // Ratio (4:1, it can be whatever you want)
const float EMA_ALPHA = 0.01;  // Smoothing factor of the EMA Filter (Attack/Release)
float emaLevel = 0.0;

void i2s_install() {
  // Set up I2S Processor configuration
  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = i2s_bits_per_sample_t(16),
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = bufferLen,
    .use_apll = false
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}

void i2s_setpin() {
  // Set I2S pin configuration
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  i2s_set_pin(I2S_PORT, &pin_config);
}

void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
  Serial.println(" ");

  delay(1000);

  // Set up I2S
  i2s_install();
  i2s_setpin();
  i2s_start(I2S_PORT);

  delay(500);
}

void loop() {
  // Get I2S data and place in data buffer
  size_t bytesIn = 0;
  esp_err_t result = i2s_read(I2S_PORT, &sBuffer, bufferLen * sizeof(int16_t), &bytesIn, portMAX_DELAY);

  if (result == ESP_OK) {
    // Read I2S data buffer
    int samples_read = bytesIn / sizeof(int16_t);
    if (samples_read > 0) {
      for (int16_t i = 0; i < samples_read; ++i) {
        float audioSample = (float)sBuffer[i] / 32768.0; // Κανονικοποίηση στο εύρος -1.0 έως 1.0

        //EMA value
        float absSample = abs(audioSample);
        emaLevel = (EMA_ALPHA * absSample) + ((1 - EMA_ALPHA) * emaLevel);

        //Compressor
        float compressedSample;
        if (emaLevel > THRESHOLD) {
          float gainReduction = (emaLevel - THRESHOLD) / RATIO;
          compressedSample = audioSample - (audioSample * gainReduction);
        } else {
          compressedSample = audioSample;
        }

        //Print the results inside the Serial Ploter
        Serial.print(audioSample);
        Serial.print(",");
        Serial.println(compressedSample);
      }
    }
  }
}