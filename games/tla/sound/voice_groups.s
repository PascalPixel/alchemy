	.macro voice kind:req, key:req, length:req, pan:req, target:req, attack:req, decay:req, sustain:req, release:req
	.byte \kind, \key, \length, \pan
	.4byte \target
	.byte \attack, \decay, \sustain, \release
	.endm

	.section .rodata
	.balign 4
	.global Sound_BgmVoices
Sound_BgmVoices:
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WaveCelesta, 255, 0, 255, 165
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WaveNylonGuitar, 255, 0, 255, 165
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WaveFingerBass, 255, 0, 255, 165
	voice 0x0b, 60, 0, 0, Sound_CgbWavePickBass, 0, 2, 3, 3
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WavePizzicatoStrings, 255, 0, 255, 165
	voice 0x00, 60, 0, 0, Sound_WaveHarp, 255, 0, 255, 165
	voice 0x00, 60, 0, 0, Sound_WaveTimpani, 255, 0, 255, 165
	voice 0x00, 60, 0, 0, Sound_WaveStrings, 255, 0, 255, 216
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WaveChoir, 255, 0, 255, 165
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WaveTrumpet, 255, 0, 255, 165
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WaveBrass, 255, 0, 255, 165
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WaveOboe, 255, 0, 255, 165
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WavePiccolo, 255, 0, 255, 165
	voice 0x00, 60, 0, 0, Sound_WaveFlute, 255, 0, 255, 165
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WavePanFlute, 255, 0, 255, 165
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_SynthPulse0, 85, 165, 193, 178
	voice 0x00, 60, 0, 0, Sound_SynthPulse1, 85, 165, 193, 178
	voice 0x00, 60, 0, 0, Sound_SynthPulse2, 85, 165, 193, 178
	voice 0x00, 60, 0, 0, Sound_SynthSaw, 85, 165, 167, 178
	voice 0x00, 60, 0, 0, Sound_SynthTriangle, 85, 127, 218, 165
	voice 0x00, 60, 0, 0, Sound_SynthPulse0, 22, 165, 193, 195
	voice 0x00, 60, 0, 0, Sound_SynthPulse1, 22, 165, 193, 195
	voice 0x00, 60, 0, 0, Sound_SynthPulse2, 22, 165, 193, 195
	voice 0x00, 60, 0, 0, Sound_SynthSaw, 22, 165, 167, 195
	voice 0x00, 60, 0, 0, Sound_SynthTriangle, 22, 127, 218, 195
	voice 0x00, 60, 0, 0, Sound_SynthPulse0, 255, 235, 90, 178
	voice 0x00, 60, 0, 0, Sound_SynthPulse1, 255, 235, 90, 178
	voice 0x00, 60, 0, 0, Sound_SynthPulse2, 255, 235, 90, 178
	voice 0x00, 60, 0, 0, Sound_SynthSaw, 255, 235, 103, 178
	voice 0x00, 60, 0, 0, Sound_SynthTriangle, 255, 235, 103, 165
	voice 0x00, 60, 0, 0, Sound_SynthPulse0, 255, 127, 90, 165
	voice 0x00, 60, 0, 0, Sound_SynthPulse0, 255, 127, 90, 165
	voice 0x00, 60, 0, 0, Sound_SynthPulse2, 255, 127, 90, 165
	voice 0x00, 60, 0, 0, Sound_SynthSaw, 255, 127, 90, 165
	voice 0x00, 60, 0, 0, Sound_SynthTriangle, 255, 127, 103, 165
	voice 0x09, 60, 0, 0, 3, 0, 1, 7, 0
	voice 0x0a, 60, 0, 0, 3, 0, 1, 7, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WaveBanjo, 255, 235, 103, 178
	voice 0x00, 60, 0, 0, Sound_WaveShamisen, 255, 242, 103, 178
	voice 0x00, 60, 0, 0, Sound_WaveKoto, 255, 0, 255, 165
	voice 0x00, 60, 0, 0, Sound_WaveKalimba, 255, 0, 255, 165
	voice 0x00, 60, 0, 0, Sound_WaveBagpipe, 255, 0, 255, 165
	voice 0x00, 60, 0, 0, Sound_WaveFiddle, 255, 235, 103, 178
	voice 0x00, 60, 0, 0, Sound_WaveShanai, 255, 0, 255, 165
	voice 0x00, 60, 0, 0, Sound_WaveTinkleBell, 255, 0, 255, 165
	voice 0x00, 60, 0, 0, Sound_WaveAgogo, 255, 0, 255, 165
	voice 0x00, 60, 0, 0, Sound_WaveSteelDrums, 255, 235, 103, 178
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 80, 0, 0, Sound_WaveTaikoDrum, 255, 0, 255, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x80, 0, 0, 0, Sound_DrumVoices - 35 * 12, 0, 0, 0, 0
@ The drum kit, one voice a key from key 35, in General MIDI's percussion
@ order: bass drums, side stick, snares and clap, then the toms (keys 41, 43,
@ 45, 47, 48 and 50, one sample pitched and panned apart) among the hi-hats,
@ the crash and the ride. Key 44, General MIDI's pedal hi-hat, plays a tom.
Sound_DrumVoices:
	voice 0x00, 60, 0, 192, Sound_WaveAcousticBassDrum, 255, 0, 255, 165
	voice 0x08, 60, 0, 192, Sound_WaveBassDrum, 255, 0, 255, 216
	voice 0x00, 60, 0, 197, Sound_WaveSideStick, 255, 0, 255, 165
	voice 0x08, 60, 0, 197, Sound_WaveAcousticSnare, 255, 0, 255, 216
	voice 0x08, 60, 0, 192, Sound_WaveHandClap, 255, 0, 255, 216
	voice 0x08, 60, 0, 197, Sound_WaveElectricSnare, 255, 0, 255, 216
	voice 0x00, 50, 0, 152, Sound_WaveTom, 255, 0, 255, 216
	voice 0x08, 60, 0, 202, Sound_WaveClosedHiHat, 128, 0, 255, 216
	voice 0x00, 55, 0, 172, Sound_WaveTom, 255, 0, 255, 216
	voice 0x08, 60, 0, 192, Sound_WaveTom, 255, 0, 255, 216
	voice 0x08, 60, 0, 192, Sound_WaveTom, 255, 0, 255, 216
	voice 0x08, 60, 0, 202, Sound_WaveOpenHiHat, 255, 0, 255, 165
	voice 0x08, 60, 0, 192, Sound_WaveTom, 255, 0, 255, 216
	voice 0x00, 65, 0, 212, Sound_WaveTom, 255, 0, 255, 216
	voice 0x08, 60, 0, 202, Sound_WaveCrashCymbal, 255, 252, 0, 165
	voice 0x00, 70, 0, 232, Sound_WaveTom, 255, 0, 255, 216
	voice 0x08, 60, 0, 172, Sound_WaveRideCymbal, 255, 249, 0, 165

	.global Sound_SeVoices
Sound_SeVoices:
	voice 0x09, 60, 0, 0, 2, 0, 4, 0, 4
	voice 0x09, 60, 0, 0, 2, 0, 4, 0, 2
	voice 0x09, 60, 0, 0, 2, 0, 4, 0, 0
	voice 0x09, 60, 0, 0, 2, 0, 7, 15, 7
	voice 0x0a, 60, 0, 0, 2, 0, 4, 0, 4
	voice 0x0a, 60, 0, 0, 2, 0, 4, 0, 2
	voice 0x0a, 60, 0, 0, 2, 0, 4, 0, 0
	voice 0x0a, 60, 0, 0, 2, 0, 7, 15, 7
	voice 0x09, 60, 0, 0, 3, 0, 1, 9, 4
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x09, 60, 0, 0, 3, 0, 4, 0, 4
	voice 0x09, 60, 0, 0, 3, 0, 4, 0, 2
	voice 0x09, 60, 0, 0, 3, 0, 4, 0, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x0c, 60, 0, 0, 0, 0, 4, 0, 4
	voice 0x0c, 60, 0, 0, 0, 0, 4, 0, 2
	voice 0x0c, 60, 0, 0, 0, 0, 4, 0, 0
	voice 0x0c, 60, 0, 0, 0, 0, 7, 15, 7
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x0c, 60, 0, 0, 1, 0, 4, 0, 4
	voice 0x0c, 60, 0, 0, 1, 0, 4, 0, 2
	voice 0x0c, 60, 0, 0, 1, 0, 4, 0, 0
	voice 0x0c, 60, 0, 0, 1, 0, 7, 15, 7
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x0c, 60, 0, 0, 1, 0, 4, 0, 4
	voice 0x0c, 60, 0, 0, 1, 0, 4, 0, 2
	voice 0x0c, 60, 0, 0, 1, 0, 4, 0, 0
	voice 0x0c, 60, 0, 0, 0, 2, 0, 15, 2
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x09, 60, 0, 0, 2, 1, 6, 0, 6
	voice 0x0b, 60, 0, 0, Sound_CgbWaveSquare, 0, 4, 0, 2
	voice 0x0c, 60, 0, 0, 0, 2, 7, 10, 7
	voice 0x0c, 60, 0, 0, 0, 0, 7, 4, 3
	voice 0x0c, 60, 0, 0, 0, 5, 2, 6, 6
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WaveSwitch, 255, 0, 255, 0
	voice 0x00, 60, 0, 0, Sound_WaveRumble, 255, 0, 255, 204
	voice 0x00, 60, 0, 0, Sound_WaveCrash, 255, 0, 255, 216
	voice 0x00, 60, 0, 0, Sound_WavePsynergySparkle, 255, 0, 255, 216
	voice 0x00, 60, 0, 0, Sound_WaveRoar, 255, 0, 255, 216
	voice 0x00, 60, 0, 0, Sound_WaveThunder, 255, 0, 255, 188
	voice 0x00, 60, 0, 0, Sound_WaveSparks, 255, 0, 255, 204
	voice 0x00, 60, 0, 0, Sound_WaveCurse, 255, 0, 255, 204
	voice 0x00, 60, 0, 0, Sound_WaveBoom, 255, 0, 255, 204
	voice 0x00, 60, 0, 0, Sound_WaveBell, 255, 188, 154, 226
	voice 0x00, 60, 0, 0, Sound_SynthPulse2, 64, 232, 0, 232
	voice 0x00, 60, 0, 0, Sound_SynthPulse2, 64, 232, 0, 195
	voice 0x00, 60, 0, 0, Sound_SynthPulse2, 64, 232, 0, 0
	voice 0x00, 60, 0, 0, Sound_SynthPulse2, 64, 0, 255, 0
	voice 0x00, 60, 0, 0, Sound_SynthPulse1, 64, 0, 255, 0
	voice 0x00, 60, 0, 0, Sound_SynthPulse1, 64, 232, 0, 232
	voice 0x00, 60, 0, 0, Sound_SynthPulse1, 64, 232, 0, 195
	voice 0x00, 60, 0, 0, Sound_SynthPulse0, 64, 232, 0, 0
	voice 0x00, 60, 0, 0, Sound_SynthPulse1, 64, 0, 255, 0
	voice 0x00, 60, 0, 0, Sound_SynthPulse0, 64, 0, 255, 0
	voice 0x00, 60, 0, 0, Sound_WaveDrip, 255, 0, 255, 204
	voice 0x00, 60, 0, 0, Sound_WaveChime, 255, 242, 28, 165
	voice 0x00, 60, 0, 0, Sound_WaveColossoLettering, 255, 0, 255, 226
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x09, 60, 0, 0, 0, 0, 6, 0, 3
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WaveEyesGlow, 255, 0, 255, 226
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WaveRing, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_WaveSpiral, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_Wave52, 255, 0, 255, 226
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_Wave53, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_WavePound, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_WaveSeaOfTimeOpens, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_WaveHiss, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_Wave57, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_WaveHowl, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_WaveJawsOpen, 255, 0, 255, 226
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WaveHiss, 255, 0, 255, 0
	voice 0x00, 60, 0, 0, Sound_WaveHiss, 255, 0, 255, 204
	voice 0x00, 60, 0, 0, Sound_WaveHiss, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_WaveSurf, 255, 0, 255, 0
	voice 0x00, 60, 0, 0, Sound_WaveSurf, 255, 0, 255, 204
	voice 0x00, 60, 0, 0, Sound_WaveSurf, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_WaveIceCrack, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_WaveStatueStirs, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_WaveCurrent, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_WaveCrackle, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_Wave65, 255, 0, 255, 226
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WaveWhoosh, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_WaveStatueRise, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_WaveSpinAway, 255, 0, 255, 226
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x01, 60, 0, 0, 2, 0, 0, 15, 0
	voice 0x00, 60, 0, 0, Sound_WaveBirdCall, 255, 0, 255, 226
	voice 0x00, 60, 0, 0, Sound_WaveWade, 128, 0, 255, 226

@ What look like two keysplit tables, unused: keys 0-35 take 0 and 36-71
@ take 1, twice over. No voice refers to them; The Broken Seal has the
@ same tables in the same place.
	.global Sound_UnusedKeySplitTables
Sound_UnusedKeySplitTables:
	.fill 36, 1, 0
	.fill 36, 1, 1
	.fill 36, 1, 0
	.fill 36, 1, 1
