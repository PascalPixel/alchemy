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
	voice direct, 60, 0, 0, SoundWaveBanjo, 255, 235, 103, 178
	voice direct, 60, 0, 0, SoundWaveShamisen, 255, 242, 103, 178
	voice direct, 60, 0, 0, SoundWaveKoto, 255, 0, 255, 165
	voice direct, 60, 0, 0, SoundWaveKalimba, 255, 0, 255, 165
	voice direct, 60, 0, 0, SoundWaveBagpipe, 255, 0, 255, 165
	voice direct, 60, 0, 0, SoundWaveFiddle, 255, 235, 103, 178
	voice direct, 60, 0, 0, SoundWaveShanai, 255, 0, 255, 165
	voice direct, 60, 0, 0, SoundWaveTinkleBell, 255, 0, 255, 165
	voice direct, 60, 0, 0, SoundWaveAgogo, 255, 0, 255, 165
	voice direct, 60, 0, 0, SoundWaveSteelDrums, 255, 235, 103, 178
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 80, 0, 0, SoundWaveTaikoDrum, 255, 0, 255, 0
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
	voice drums, 0, 0, 0, SoundDrumVoices - 35 * 12, 0, 0, 0, 0

@ The drum kit, one voice a key from key 35, in General MIDI's percussion
@ order: bass drums, side stick, snares and clap, then the toms (one sample
@ pitched and panned apart) among the hi-hats, the crash and the ride.
	.global SoundDrumVoices
SoundDrumVoices:
	voice direct, 60, 0, 192, SoundWaveAcousticBassDrum, 255, 0, 255, 165
	voice direct_fixed, 60, 0, 192, SoundWaveBassDrum, 255, 0, 255, 216
	voice direct, 60, 0, 197, SoundWaveSideStick, 255, 0, 255, 165
	voice direct_fixed, 60, 0, 197, SoundWaveAcousticSnare, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, SoundWaveHandClap, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 197, SoundWaveElectricSnare, 255, 0, 255, 216
	voice direct, 50, 0, 152, SoundWaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 202, SoundWaveClosedHiHat, 128, 0, 255, 216
	voice direct, 55, 0, 172, SoundWaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, SoundWaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 192, SoundWaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 202, SoundWaveOpenHiHat, 255, 0, 255, 165
	voice direct_fixed, 60, 0, 192, SoundWaveTom, 255, 0, 255, 216
	voice direct, 65, 0, 212, SoundWaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 202, SoundWaveCrashCymbal, 255, 252, 0, 165
	voice direct, 70, 0, 232, SoundWaveTom, 255, 0, 255, 216
	voice direct_fixed, 60, 0, 172, SoundWaveRideCymbal, 255, 249, 0, 165

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
	voice direct, 60, 0, 0, SoundWaveSparks, 255, 0, 255, 204
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
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundWaveEyesGlow, 255, 0, 255, 226
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundWaveRing, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWaveSpiral, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWaveBloop, 255, 0, 255, 226
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundWaveDoubleThud, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWavePound, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWaveSeaOfTimeOpens, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWaveHiss, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWaveHighDrone, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWaveHowl, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWaveJawsOpen, 255, 0, 255, 226
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundWaveHiss, 255, 0, 255, 0
	voice direct, 60, 0, 0, SoundWaveHiss, 255, 0, 255, 204
	voice direct, 60, 0, 0, SoundWaveHiss, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWaveSurf, 255, 0, 255, 0
	voice direct, 60, 0, 0, SoundWaveSurf, 255, 0, 255, 204
	voice direct, 60, 0, 0, SoundWaveSurf, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWaveIceCrack, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWaveStatueStirs, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWaveCurrent, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWaveCrackle, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWaveWail, 255, 0, 255, 226
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundWaveWhoosh, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWaveStatueRise, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWaveSpinAway, 255, 0, 255, 226
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice square1, 60, 0, 0, 2, 0, 0, 15, 0
	voice direct, 60, 0, 0, SoundWaveBirdCall, 255, 0, 255, 226
	voice direct, 60, 0, 0, SoundWaveWade, 128, 0, 255, 226

@ What look like two keysplit tables, unused: keys 0-35 take 0 and 36-71
@ take 1, twice over. No voice refers to them; The Broken Seal has the
@ same tables in the same place.
	.global SoundUnusedKeySplitTables
SoundUnusedKeySplitTables:
	.fill 36, 1, 0
	.fill 36, 1, 1
	.fill 36, 1, 0
	.fill 36, 1, 1
