@ The samples the voices play: a recorded one is its file of
@ direct_sound_samples/, a synthesized one a header that names its waveform.
	.macro synth waveform:req
	.4byte 1 << 30, 17140000, 0, 0
	.byte 0x80, \waveform
	.endm
	.macro synth_pulse duty:req, speed:req, depth:req, phase:req
	synth 0
	.byte \duty, \speed, \depth, \phase
	.balign 4
	.endm
	.macro synth_saw
	synth 1
	.balign 4
	.endm
	.macro synth_triangle
	synth 2
	.balign 4
	.endm

	.section .rodata
	.balign 4
	.global SoundWaveCelesta
SoundWaveCelesta:
	.incbin "games/tbs/sound/direct_sound_samples/celesta.pcm8.bin"
	.global SoundWaveNylonGuitar
SoundWaveNylonGuitar:
	.incbin "games/tbs/sound/direct_sound_samples/nylon_guitar.pcm8.bin"
	.global SoundWaveFingerBass
SoundWaveFingerBass:
	.incbin "games/tbs/sound/direct_sound_samples/finger_bass.pcm8.bin"
	.global SoundWavePizzicatoStrings
SoundWavePizzicatoStrings:
	.incbin "games/tbs/sound/direct_sound_samples/pizzicato_strings.pcm8.bin"
	.global SoundWaveHarp
SoundWaveHarp:
	.incbin "games/tbs/sound/direct_sound_samples/harp.pcm8.bin"
	.global SoundWaveTimpani
SoundWaveTimpani:
	.incbin "games/tbs/sound/direct_sound_samples/timpani.pcm8.bin"
	.global SoundWaveStrings
SoundWaveStrings:
	.incbin "games/tbs/sound/direct_sound_samples/strings.pcm8.bin"
	.global SoundWaveChoir
SoundWaveChoir:
	.incbin "games/tbs/sound/direct_sound_samples/choir.pcm8.bin"
	.global SoundWaveTrumpet
SoundWaveTrumpet:
	.incbin "games/tbs/sound/direct_sound_samples/trumpet.pcm8.bin"
	.global SoundWaveBrass
SoundWaveBrass:
	.incbin "games/tbs/sound/direct_sound_samples/brass.pcm8.bin"
	.global SoundWaveOboe
SoundWaveOboe:
	.incbin "games/tbs/sound/direct_sound_samples/oboe.pcm8.bin"
	.global SoundWavePiccolo
SoundWavePiccolo:
	.incbin "games/tbs/sound/direct_sound_samples/piccolo.pcm8.bin"
	.global SoundWaveFlute
SoundWaveFlute:
	.incbin "games/tbs/sound/direct_sound_samples/flute.pcm8.bin"
	.global SoundWavePanFlute
SoundWavePanFlute:
	.incbin "games/tbs/sound/direct_sound_samples/pan_flute.pcm8.bin"
	.global SoundSynthPulse0
SoundSynthPulse0:
	synth_pulse 16, 240, 224, 128
	.global SoundSynthPulse1
SoundSynthPulse1:
	synth_pulse 96, 32, 64, 128
	.global SoundSynthPulse2
SoundSynthPulse2:
	synth_pulse 128, 64, 64, 64
	.global SoundSynthSaw
SoundSynthSaw:
	synth_saw
	.global SoundSynthTriangle
SoundSynthTriangle:
	synth_triangle
	.global SoundWaveBassDrum
SoundWaveBassDrum:
	.incbin "games/tbs/sound/direct_sound_samples/bass_drum.pcm8.bin"
	.global SoundWaveSnare
SoundWaveSnare:
	.incbin "games/tbs/sound/direct_sound_samples/snare.pcm8.bin"
	.global SoundWaveTom
SoundWaveTom:
	.incbin "games/tbs/sound/direct_sound_samples/tom.pcm8.bin"
	.global SoundWaveClosedHiHat
SoundWaveClosedHiHat:
	.incbin "games/tbs/sound/direct_sound_samples/closed_hi_hat.pcm8.bin"
	.global SoundWaveCrashCymbal
SoundWaveCrashCymbal:
	.incbin "games/tbs/sound/direct_sound_samples/crash_cymbal.pcm8.bin"
	.global SoundWaveRideCymbal
SoundWaveRideCymbal:
	.incbin "games/tbs/sound/direct_sound_samples/ride_cymbal.pcm8.bin"
	.global SoundWaveSwitch
SoundWaveSwitch:
	.incbin "games/tbs/sound/direct_sound_samples/switch.pcm8.bin"
	.global SoundWaveRumble
SoundWaveRumble:
	.incbin "games/tbs/sound/direct_sound_samples/rumble.pcm8.bin"
	.global SoundWaveCrash
SoundWaveCrash:
	.incbin "games/tbs/sound/direct_sound_samples/crash.pcm8.bin"
	.global SoundWavePsynergySparkle
SoundWavePsynergySparkle:
	.incbin "games/tbs/sound/direct_sound_samples/psynergy_sparkle.pcm8.bin"
	.global SoundWaveRoar
SoundWaveRoar:
	.incbin "games/tbs/sound/direct_sound_samples/roar.pcm8.bin"
	.global SoundWaveThunder
SoundWaveThunder:
	.incbin "games/tbs/sound/direct_sound_samples/thunder.pcm8.bin"
	.global SoundWaveCurse
SoundWaveCurse:
	.incbin "games/tbs/sound/direct_sound_samples/curse.pcm8.bin"
	.global SoundWaveBoom
SoundWaveBoom:
	.incbin "games/tbs/sound/direct_sound_samples/boom.pcm8.bin"
	.global SoundWaveBell
SoundWaveBell:
	.incbin "games/tbs/sound/direct_sound_samples/bell.pcm8.bin"
	.global SoundWaveDrip
SoundWaveDrip:
	.incbin "games/tbs/sound/direct_sound_samples/drip.pcm8.bin"
	.global SoundWaveChime
SoundWaveChime:
	.incbin "games/tbs/sound/direct_sound_samples/chime.pcm8.bin"
	.global SoundWaveColossoLettering
SoundWaveColossoLettering:
	.incbin "games/tbs/sound/direct_sound_samples/colosso_lettering.pcm8.bin"
