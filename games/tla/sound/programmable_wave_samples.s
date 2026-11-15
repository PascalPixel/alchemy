@ The wave channel's shapes, 32 four-bit steps each, named by their shape:
@ the pulses by how much of the cycle is high. Only the square (a sound
@ effect's voice) and the pick bass (Sound_BgmVoices slot 34, General MIDI's
@ Electric Bass (pick)) are voices; the others are no voice's.
	.section .rodata
	.balign 4
	.global Sound_CgbWaveSine
Sound_CgbWaveSine:
	.incbin "games/tla/sound/programmable_wave_samples/sine.bin"
	.global Sound_CgbWaveTriangle
Sound_CgbWaveTriangle:
	.incbin "games/tla/sound/programmable_wave_samples/triangle.bin"
	.global Sound_CgbWaveSawtooth
Sound_CgbWaveSawtooth:
	.incbin "games/tla/sound/programmable_wave_samples/sawtooth.bin"
	.global Sound_CgbWaveRampWithPlateau
Sound_CgbWaveRampWithPlateau:
	.incbin "games/tla/sound/programmable_wave_samples/ramp_with_plateau.bin"
	.global Sound_CgbWavePulseEighth
Sound_CgbWavePulseEighth:
	.incbin "games/tla/sound/programmable_wave_samples/pulse_eighth.bin"
	.global Sound_CgbWavePulseQuarter
Sound_CgbWavePulseQuarter:
	.incbin "games/tla/sound/programmable_wave_samples/pulse_quarter.bin"
	.global Sound_CgbWavePulseThreeEighths
Sound_CgbWavePulseThreeEighths:
	.incbin "games/tla/sound/programmable_wave_samples/pulse_three_eighths.bin"
	.global Sound_CgbWaveSquare
Sound_CgbWaveSquare:
	.incbin "games/tla/sound/programmable_wave_samples/square.bin"
	.global Sound_CgbWavePickBass
Sound_CgbWavePickBass:
	.incbin "games/tla/sound/programmable_wave_samples/pick_bass.bin"
