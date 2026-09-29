@ The CGB wave channel's wave RAM patterns, each 32 four-bit samples,
@ built from the PCM4 files beside the samples.
	.section .rodata
	.balign 4
	.global Sound_CgbWave0
Sound_CgbWave0:
	.incbin "SOUND/SAMPLE/CGB_WAVE_0.bin"
	.global Sound_CgbWave1
Sound_CgbWave1:
	.incbin "SOUND/SAMPLE/CGB_WAVE_1.bin"
	.global Sound_CgbWave2
Sound_CgbWave2:
	.incbin "SOUND/SAMPLE/CGB_WAVE_2.bin"
	.global Sound_CgbWave3
Sound_CgbWave3:
	.incbin "SOUND/SAMPLE/CGB_WAVE_3.bin"
	.global Sound_CgbWave4
Sound_CgbWave4:
	.incbin "SOUND/SAMPLE/CGB_WAVE_4.bin"
	.global Sound_CgbWave5
Sound_CgbWave5:
	.incbin "SOUND/SAMPLE/CGB_WAVE_5.bin"
	.global Sound_CgbWave6
Sound_CgbWave6:
	.incbin "SOUND/SAMPLE/CGB_WAVE_6.bin"
	.global Sound_CgbWave7
Sound_CgbWave7:
	.incbin "SOUND/SAMPLE/CGB_WAVE_7.bin"
	.global Sound_CgbWave8
Sound_CgbWave8:
	.incbin "SOUND/SAMPLE/CGB_WAVE_8.bin"
