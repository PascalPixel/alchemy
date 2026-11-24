@ The voice groups (the sound library's ToneData): one row a voice, its kind,
@ key, length and pan, then its sample, wave pattern, duty or sub-group, and
@ its attack, decay, sustain and release.
	.set voice_direct, 0
	.set voice_square1, 1
	.set voice_square2, 2
	.set voice_wave, 3
	.set voice_noise, 4
	.set voice_direct_fixed, 8
	.set voice_square1_alt, 9
	.set voice_square2_alt, 10
	.set voice_wave_alt, 11
	.set voice_noise_alt, 12
	.set voice_drums, 128

	.macro voice kind:req, key:req, length:req, pan:req, target:req, attack:req, decay:req, sustain:req, release:req
	.byte voice_\kind, \key, \length, \pan
	.4byte \target
	.byte \attack, \decay, \sustain, \release
	.endm

	.section .rodata
	.balign 4

@ The music's voices, in General MIDI's program order.
	.global Sound_BgmVoices
Sound_BgmVoices:
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, Sound_WaveCelesta, 255, 0, 255, 165
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, Sound_WaveNylonGuitar, 255, 0, 255, 165
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, Sound_WaveFingerBass, 255, 0, 255, 165
	voice wave_alt, 60, 0, 0, Sound_CgbWavePickBass, 0, 2, 3, 3
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, Sound_WavePizzicatoStrings, 255, 0, 255, 165
	voice direct, 60, 0, 0, Sound_WaveHarp, 255, 0, 255, 165
	voice direct, 60, 0, 0, Sound_WaveTimpani, 255, 0, 255, 165
	voice direct, 60, 0, 0, Sound_WaveStrings, 255, 0, 255, 216
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, Sound_WaveChoir, 255, 0, 255, 165
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, Sound_WaveTrumpet, 255, 0, 255, 165
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, Sound_WaveBrass, 255, 0, 255, 165
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, Sound_WaveOboe, 255, 0, 255, 165
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, Sound_WavePiccolo, 255, 0, 255, 165
	voice direct, 60, 0, 0, Sound_WaveFlute, 255, 0, 255, 165
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, Sound_WavePanFlute, 255, 0, 255, 165
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, Sound_SynthPulse0, 85, 165, 193, 178
	voice direct, 60, 0, 0, Sound_SynthPulse1, 85, 165, 193, 178
	voice direct, 60, 0, 0, Sound_SynthPulse2, 85, 165, 193, 178
	voice direct, 60, 0, 0, Sound_SynthSaw, 85, 165, 167, 178
	voice direct, 60, 0, 0, Sound_SynthTriangle, 85, 127, 218, 165
	voice direct, 60, 0, 0, Sound_SynthPulse0, 22, 165, 193, 195
	voice direct, 60, 0, 0, Sound_SynthPulse1, 22, 165, 193, 195
	voice direct, 60, 0, 0, Sound_SynthPulse2, 22, 165, 193, 195
	voice direct, 60, 0, 0, Sound_SynthSaw, 22, 165, 167, 195
	voice direct, 60, 0, 0, Sound_SynthTriangle, 22, 127, 218, 195
	voice direct, 60, 0, 0, Sound_SynthPulse0, 255, 235, 90, 178
	voice direct, 60, 0, 0, Sound_SynthPulse1, 255, 235, 90, 178
	voice direct, 60, 0, 0, Sound_SynthPulse2, 255, 235, 90, 178
	voice direct, 60, 0, 0, Sound_SynthSaw, 255, 235, 103, 178
	voice direct, 60, 0, 0, Sound_SynthTriangle, 255, 235, 103, 165
	voice direct, 60, 0, 0, Sound_SynthPulse0, 255, 127, 90, 165
	voice direct, 60, 0, 0, Sound_SynthPulse0, 255, 127, 90, 165
	voice direct, 60, 0, 0, Sound_SynthPulse2, 255, 127, 90, 165
	voice direct, 60, 0, 0, Sound_SynthSaw, 255, 127, 90, 165
	voice direct, 60, 0, 0, Sound_SynthTriangle, 255, 127, 103, 165
	voice square1_alt, 60, 0, 0, 3, 0, 1, 7, 0
	voice square2_alt, 60, 0, 0, 3, 0, 1, 7, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice drums, 0, 0, 0, Sound_DrumVoices - 36 * 12, 0, 0, 0, 0

@ The drum kit, one voice a key from key 35, in General MIDI's percussion
@ order: bass drums, side stick, snares and clap, then the toms (one sample
@ pitched and panned apart) among the hi-hats, the crash and the ride.
	.global Sound_DrumVoices
Sound_DrumVoices:
	voice direct_fixed, 60, 0, 192, Sound_WaveBassDrum, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, Sound_WaveBassDrum, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, Sound_WaveSnare, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, Sound_WaveSnare, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, Sound_WaveSnare, 255, 0, 255, 216
	voice direct, 55, 0, 160, Sound_WaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, Sound_WaveClosedHiHat, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, Sound_WaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, Sound_WaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, Sound_WaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, Sound_WaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, Sound_WaveTom, 255, 0, 255, 216
	voice direct, 65, 0, 224, Sound_WaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, Sound_WaveCrashCymbal, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, Sound_WaveCrashCymbal, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, Sound_WaveRideCymbal, 255, 0, 255, 216

@ The sound effects' voices.
	.global Sound_SeVoices
Sound_SeVoices:
	voice square1_alt, 60, 0, 0, 2, 0, 4, 0, 4
	voice square1_alt, 60, 0, 0, 2, 0, 4, 0, 2
	voice square1_alt, 60, 0, 0, 2, 0, 4, 0, 0
	voice square1_alt, 60, 0, 0, 2, 0, 7, 15, 7
	voice square2_alt, 60, 0, 0, 2, 0, 4, 0, 4
	voice square2_alt, 60, 0, 0, 2, 0, 4, 0, 2
	voice square2_alt, 60, 0, 0, 2, 0, 4, 0, 0
	voice square2_alt, 60, 0, 0, 2, 0, 7, 15, 7
	voice square1_alt, 60, 0, 0, 3, 0, 1, 9, 4
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1_alt, 60, 0, 0, 3, 0, 4, 0, 4
	voice square1_alt, 60, 0, 0, 3, 0, 4, 0, 2
	voice square1_alt, 60, 0, 0, 3, 0, 4, 0, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice noise_alt, 60, 0, 0, 0, 0, 4, 0, 4
	voice noise_alt, 60, 0, 0, 0, 0, 4, 0, 2
	voice noise_alt, 60, 0, 0, 0, 0, 4, 0, 0
	voice noise_alt, 60, 0, 0, 0, 0, 7, 15, 7
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice noise_alt, 60, 0, 0, 1, 0, 4, 0, 4
	voice noise_alt, 60, 0, 0, 1, 0, 4, 0, 2
	voice noise_alt, 60, 0, 0, 1, 0, 4, 0, 0
	voice noise_alt, 60, 0, 0, 1, 0, 7, 15, 7
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice noise_alt, 60, 0, 0, 1, 0, 4, 0, 4
	voice noise_alt, 60, 0, 0, 1, 0, 4, 0, 2
	voice noise_alt, 60, 0, 0, 1, 0, 4, 0, 0
	voice noise_alt, 60, 0, 0, 0, 2, 0, 15, 2
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1_alt, 60, 0, 0, 2, 1, 6, 0, 6
	voice wave_alt, 60, 0, 0, Sound_CgbWaveSquare, 0, 4, 0, 2
	voice noise_alt, 60, 0, 0, 0, 2, 7, 10, 7
	voice noise_alt, 60, 0, 0, 0, 0, 7, 4, 3
	voice noise_alt, 60, 0, 0, 0, 5, 2, 6, 6
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, Sound_WaveSwitch, 255, 0, 255, 0
	voice direct, 60, 0, 0, Sound_WaveRumble, 255, 0, 255, 204
	voice direct, 60, 0, 0, Sound_WaveCrash, 255, 0, 255, 216
	voice direct, 60, 0, 0, Sound_WavePsynergySparkle, 255, 0, 255, 216
	voice direct, 60, 0, 0, Sound_WaveRoar, 255, 0, 255, 216
	voice direct, 60, 0, 0, Sound_WaveThunder, 255, 0, 255, 188
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, Sound_WaveCurse, 255, 0, 255, 204
	voice direct, 60, 0, 0, Sound_WaveBoom, 255, 0, 255, 204
	voice direct, 60, 0, 0, Sound_WaveBell, 255, 188, 154, 226
	voice direct, 60, 0, 0, Sound_SynthPulse2, 64, 232, 0, 232
	voice direct, 60, 0, 0, Sound_SynthPulse2, 64, 232, 0, 195
	voice direct, 60, 0, 0, Sound_SynthPulse2, 64, 232, 0, 0
	voice direct, 60, 0, 0, Sound_SynthPulse2, 64, 0, 255, 0
	voice direct, 60, 0, 0, Sound_SynthPulse1, 64, 0, 255, 0
	voice direct, 60, 0, 0, Sound_SynthPulse1, 64, 232, 0, 232
	voice direct, 60, 0, 0, Sound_SynthPulse1, 64, 232, 0, 195
	voice direct, 60, 0, 0, Sound_SynthPulse0, 64, 232, 0, 0
	voice direct, 60, 0, 0, Sound_SynthPulse1, 64, 0, 255, 0
	voice direct, 60, 0, 0, Sound_SynthPulse0, 64, 0, 255, 0
	voice direct, 60, 0, 0, Sound_WaveDrip, 255, 0, 255, 204
	voice direct, 60, 0, 0, Sound_WaveChime, 255, 242, 28, 165
	voice direct, 60, 0, 0, Sound_WaveColossoLettering, 255, 0, 255, 226
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1_alt, 60, 0, 0, 0, 0, 6, 0, 3

@ Apparent unused key-split tables: two 72-key rows, 0 for keys 0-35 and 1 for 36-71.
@ No voice references these; this edition has no key-split (0x40) voices.
@ TLA has the same tables in the corresponding position.
	.global Sound_UnusedKeySplitTables
Sound_UnusedKeySplitTables:
	.fill 36, 1, 0
	.fill 36, 1, 1
	.fill 36, 1, 0
	.fill 36, 1, 1
