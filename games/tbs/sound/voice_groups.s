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
	.global SoundBgmVoices
SoundBgmVoices:
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundWaveCelesta, 255, 0, 255, 165
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
	voice direct, 60, 0, 0, SoundWaveNylonGuitar, 255, 0, 255, 165
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundWaveFingerBass, 255, 0, 255, 165
	voice wave_alt, 60, 0, 0, SoundCgbWavePickBass, 0, 2, 3, 3
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
	voice direct, 60, 0, 0, SoundWavePizzicatoStrings, 255, 0, 255, 165
	voice direct, 60, 0, 0, SoundWaveHarp, 255, 0, 255, 165
	voice direct, 60, 0, 0, SoundWaveTimpani, 255, 0, 255, 165
	voice direct, 60, 0, 0, SoundWaveStrings, 255, 0, 255, 216
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundWaveChoir, 255, 0, 255, 165
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundWaveTrumpet, 255, 0, 255, 165
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundWaveBrass, 255, 0, 255, 165
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundWaveOboe, 255, 0, 255, 165
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundWavePiccolo, 255, 0, 255, 165
	voice direct, 60, 0, 0, SoundWaveFlute, 255, 0, 255, 165
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundWavePanFlute, 255, 0, 255, 165
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundSynthPulse0, 85, 165, 193, 178
	voice direct, 60, 0, 0, SoundSynthPulse1, 85, 165, 193, 178
	voice direct, 60, 0, 0, SoundSynthPulse2, 85, 165, 193, 178
	voice direct, 60, 0, 0, SoundSynthSaw, 85, 165, 167, 178
	voice direct, 60, 0, 0, SoundSynthTriangle, 85, 127, 218, 165
	voice direct, 60, 0, 0, SoundSynthPulse0, 22, 165, 193, 195
	voice direct, 60, 0, 0, SoundSynthPulse1, 22, 165, 193, 195
	voice direct, 60, 0, 0, SoundSynthPulse2, 22, 165, 193, 195
	voice direct, 60, 0, 0, SoundSynthSaw, 22, 165, 167, 195
	voice direct, 60, 0, 0, SoundSynthTriangle, 22, 127, 218, 195
	voice direct, 60, 0, 0, SoundSynthPulse0, 255, 235, 90, 178
	voice direct, 60, 0, 0, SoundSynthPulse1, 255, 235, 90, 178
	voice direct, 60, 0, 0, SoundSynthPulse2, 255, 235, 90, 178
	voice direct, 60, 0, 0, SoundSynthSaw, 255, 235, 103, 178
	voice direct, 60, 0, 0, SoundSynthTriangle, 255, 235, 103, 165
	voice direct, 60, 0, 0, SoundSynthPulse0, 255, 127, 90, 165
	voice direct, 60, 0, 0, SoundSynthPulse0, 255, 127, 90, 165
	voice direct, 60, 0, 0, SoundSynthPulse2, 255, 127, 90, 165
	voice direct, 60, 0, 0, SoundSynthSaw, 255, 127, 90, 165
	voice direct, 60, 0, 0, SoundSynthTriangle, 255, 127, 103, 165
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
	voice drums, 0, 0, 0, SoundDrumVoices - 36 * 12, 0, 0, 0, 0

@ The drum kit, one voice a key from key 35, in General MIDI's percussion
@ order: bass drums, side stick, snares and clap, then the toms (one sample
@ pitched and panned apart) among the hi-hats, the crash and the ride.
	.global SoundDrumVoices
SoundDrumVoices:
	voice direct_fixed, 60, 0, 192, SoundWaveBassDrum, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, SoundWaveBassDrum, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, SoundWaveSnare, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, SoundWaveSnare, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, SoundWaveSnare, 255, 0, 255, 216
	voice direct, 55, 0, 160, SoundWaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, SoundWaveClosedHiHat, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, SoundWaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, SoundWaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, SoundWaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, SoundWaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, SoundWaveTom, 255, 0, 255, 216
	voice direct, 65, 0, 224, SoundWaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, SoundWaveCrashCymbal, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, SoundWaveCrashCymbal, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, SoundWaveRideCymbal, 255, 0, 255, 216

@ The sound effects' voices.
	.global SoundSeVoices
SoundSeVoices:
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
	voice wave_alt, 60, 0, 0, SoundCgbWaveSquare, 0, 4, 0, 2
	voice noise_alt, 60, 0, 0, 0, 2, 7, 10, 7
	voice noise_alt, 60, 0, 0, 0, 0, 7, 4, 3
	voice noise_alt, 60, 0, 0, 0, 5, 2, 6, 6
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundWaveSwitch, 255, 0, 255, 0
	voice direct, 60, 0, 0, SoundWaveRumble, 255, 0, 255, 204
	voice direct, 60, 0, 0, SoundWaveCrash, 255, 0, 255, 216
	voice direct, 60, 0, 0, SoundWavePsynergySparkle, 255, 0, 255, 216
	voice direct, 60, 0, 0, SoundWaveRoar, 255, 0, 255, 216
	voice direct, 60, 0, 0, SoundWaveThunder, 255, 0, 255, 188
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundWaveCurse, 255, 0, 255, 204
	voice direct, 60, 0, 0, SoundWaveBoom, 255, 0, 255, 204
	voice direct, 60, 0, 0, SoundWaveBell, 255, 188, 154, 226
	voice direct, 60, 0, 0, SoundSynthPulse2, 64, 232, 0, 232
	voice direct, 60, 0, 0, SoundSynthPulse2, 64, 232, 0, 195
	voice direct, 60, 0, 0, SoundSynthPulse2, 64, 232, 0, 0
	voice direct, 60, 0, 0, SoundSynthPulse2, 64, 0, 255, 0
	voice direct, 60, 0, 0, SoundSynthPulse1, 64, 0, 255, 0
	voice direct, 60, 0, 0, SoundSynthPulse1, 64, 232, 0, 232
	voice direct, 60, 0, 0, SoundSynthPulse1, 64, 232, 0, 195
	voice direct, 60, 0, 0, SoundSynthPulse0, 64, 232, 0, 0
	voice direct, 60, 0, 0, SoundSynthPulse1, 64, 0, 255, 0
	voice direct, 60, 0, 0, SoundSynthPulse0, 64, 0, 255, 0
	voice direct, 60, 0, 0, SoundWaveDrip, 255, 0, 255, 204
	voice direct, 60, 0, 0, SoundWaveChime, 255, 242, 28, 165
	voice direct, 60, 0, 0, SoundWaveColossoLettering, 255, 0, 255, 226
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
	.global SoundUnusedKeySplitTables
SoundUnusedKeySplitTables:
	.fill 36, 1, 0
	.fill 36, 1, 1
	.fill 36, 1, 0
	.fill 36, 1, 1
