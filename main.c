#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <stdio.h>

ma_decoder decoder;
ma_device device;
float globalVolume = 1.0f; // Initial volume

// Callback function to fill audio data
void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	ma_decoder_read_pcm_frames(&decoder, pOutput, frameCount, NULL);

	float* out = (float*)pOutput;
//	for (int i = 0; i < frameCount * pDevice->playback.channels; i += 2) {
////		*out *= globalVolume;
//		out[i] *= globalVolume;
//		out[i+1] *= 1.0f - globalVolume;
//		out += 2;
//	}
	for (int i = 0; i < frameCount; ++i) {
		// Adjust volume for left channel
		out[2*i] *= globalVolume;
		// Adjust volume for right channel
		out[2*i + 1] *= 1.0f - globalVolume;
	}
}

// Function to change global volume
void setVolume(float volume) {
	if (volume < 0.0f) {
		volume = 0.0f;
	} else if (volume > 1.0f) {
		volume = 1.0f;
	}
	globalVolume = volume;
}

int main() {
	// Initialize miniaudio decoder
	ma_result result = ma_decoder_init_file("NECRY TALKIE - bloom ｜UPBEAT - Piano Cover.mp3", NULL, &decoder);
	if (result != MA_SUCCESS) {
		printf("Failed to initialize decoder\n");
		return -1;
	}

	// Set up device configuration
	ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
	deviceConfig.playback.format   = decoder.outputFormat;
	deviceConfig.playback.channels = decoder.outputChannels;
	deviceConfig.sampleRate        = decoder.outputSampleRate;
	deviceConfig.dataCallback      = data_callback;

	// Initialize audio playback device
	result = ma_device_init(NULL, &deviceConfig, &device);
	if (result != MA_SUCCESS) {
		printf("Failed to initialize playback device\n");
		ma_decoder_uninit(&decoder);
		return -1;
	}

	// Start playback
	result = ma_device_start(&device);
	if (result != MA_SUCCESS) {
		printf("Failed to start playback device\n");
		ma_device_uninit(&device);
		ma_decoder_uninit(&decoder);
		return -1;
	}

	// Loop to change volume
	printf("Press '+' to increase volume, '-' to decrease, or 'q' to quit...\n");
	char input;
//	while ((input = getchar()) != 'q') {
//		if (input == '+') {
//			setVolume(globalVolume + 0.1f);
//			printf("Volume increased to %.1f\n", globalVolume);
//		} else if (input == '-') {
//			setVolume(globalVolume - 0.1f);
//			printf("Volume decreased to %.1f\n", globalVolume);
//		}
//	}
	float i = 1.0f;
	while (1) {
		if (globalVolume == 1.0f || globalVolume == 0.0f) {
			i *= -1.0f;
		}
		printf("Current volume: %f\n", globalVolume + i * 0.01f);
		setVolume(globalVolume + i*0.01f);
		ma_sleep(10);
	}
	// Cleanup
	ma_device_uninit(&device);
	ma_decoder_uninit(&decoder);

	return 0;
}
