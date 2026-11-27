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
	.global Sound_WaveCelesta
Sound_WaveCelesta:
	.incbin "games/tbs/sound/direct_sound_samples/celesta.pcm8.bin"
	.global Sound_WaveNylonGuitar
Sound_WaveNylonGuitar:
	.incbin "games/tbs/sound/direct_sound_samples/nylon_guitar.pcm8.bin"
	.global Sound_WaveFingerBass
Sound_WaveFingerBass:
	.incbin "games/tbs/sound/direct_sound_samples/finger_bass.pcm8.bin"
	.global Sound_WavePizzicatoStrings
Sound_WavePizzicatoStrings:
	.incbin "games/tbs/sound/direct_sound_samples/pizzicato_strings.pcm8.bin"
	.global Sound_WaveHarp
Sound_WaveHarp:
	.incbin "games/tbs/sound/direct_sound_samples/harp.pcm8.bin"
	.global Sound_WaveTimpani
Sound_WaveTimpani:
	.incbin "games/tbs/sound/direct_sound_samples/timpani.pcm8.bin"
	.global Sound_WaveStrings
Sound_WaveStrings:
	.incbin "games/tbs/sound/direct_sound_samples/strings.pcm8.bin"
	.global Sound_WaveChoir
Sound_WaveChoir:
	.incbin "games/tbs/sound/direct_sound_samples/choir.pcm8.bin"
	.global Sound_WaveTrumpet
Sound_WaveTrumpet:
	.incbin "games/tbs/sound/direct_sound_samples/trumpet.pcm8.bin"
	.global Sound_WaveBrass
Sound_WaveBrass:
	.incbin "games/tbs/sound/direct_sound_samples/brass.pcm8.bin"
	.global Sound_WaveOboe
Sound_WaveOboe:
	.incbin "games/tbs/sound/direct_sound_samples/oboe.pcm8.bin"
	.global Sound_WavePiccolo
Sound_WavePiccolo:
	.incbin "games/tbs/sound/direct_sound_samples/piccolo.pcm8.bin"
	.global Sound_WaveFlute
Sound_WaveFlute:
	.incbin "games/tbs/sound/direct_sound_samples/flute.pcm8.bin"
	.global Sound_WavePanFlute
Sound_WavePanFlute:
	.incbin "games/tbs/sound/direct_sound_samples/pan_flute.pcm8.bin"
	.global Sound_SynthPulse0
Sound_SynthPulse0:
	synth_pulse 16, 240, 224, 128
	.global Sound_SynthPulse1
Sound_SynthPulse1:
	synth_pulse 96, 32, 64, 128
	.global Sound_SynthPulse2
Sound_SynthPulse2:
	synth_pulse 128, 64, 64, 64
	.global Sound_SynthSaw
Sound_SynthSaw:
	synth_saw
	.global Sound_SynthTriangle
Sound_SynthTriangle:
	synth_triangle
	.global Sound_WaveBassDrum
Sound_WaveBassDrum:
	.incbin "games/tbs/sound/direct_sound_samples/bass_drum.pcm8.bin"
	.global Sound_WaveSnare
Sound_WaveSnare:
	.incbin "games/tbs/sound/direct_sound_samples/snare.pcm8.bin"
	.global Sound_WaveTom
Sound_WaveTom:
	.incbin "games/tbs/sound/direct_sound_samples/tom.pcm8.bin"
	.global Sound_WaveClosedHiHat
Sound_WaveClosedHiHat:
	.incbin "games/tbs/sound/direct_sound_samples/closed_hi_hat.pcm8.bin"
	.global Sound_WaveCrashCymbal
Sound_WaveCrashCymbal:
	.incbin "games/tbs/sound/direct_sound_samples/crash_cymbal.pcm8.bin"
	.global Sound_WaveRideCymbal
Sound_WaveRideCymbal:
	.incbin "games/tbs/sound/direct_sound_samples/ride_cymbal.pcm8.bin"
	.global Sound_WaveSwitch
Sound_WaveSwitch:
	.incbin "games/common/sound/direct_sound_samples/switch.pcm8.bin"
	.global Sound_WaveRumble
Sound_WaveRumble:
	.incbin "games/common/sound/direct_sound_samples/rumble.pcm8.bin"
	.global Sound_WaveCrash
Sound_WaveCrash:
	.incbin "games/common/sound/direct_sound_samples/crash.pcm8.bin"
	.global Sound_WavePsynergySparkle
Sound_WavePsynergySparkle:
	.incbin "games/common/sound/direct_sound_samples/psynergy_sparkle.pcm8.bin"
	.global Sound_WaveRoar
Sound_WaveRoar:
	.incbin "games/common/sound/direct_sound_samples/roar.pcm8.bin"
	.global Sound_WaveThunder
Sound_WaveThunder:
	.incbin "games/common/sound/direct_sound_samples/thunder.pcm8.bin"
	.global Sound_WaveCurse
Sound_WaveCurse:
	.incbin "games/common/sound/direct_sound_samples/curse.pcm8.bin"
	.global Sound_WaveBoom
Sound_WaveBoom:
	.incbin "games/common/sound/direct_sound_samples/boom.pcm8.bin"
	.global Sound_WaveBell
Sound_WaveBell:
	.incbin "games/common/sound/direct_sound_samples/bell.pcm8.bin"
	.global Sound_WaveDrip
Sound_WaveDrip:
	.incbin "games/common/sound/direct_sound_samples/drip.pcm8.bin"
	.global Sound_WaveChime
Sound_WaveChime:
	.incbin "games/common/sound/direct_sound_samples/chime.pcm8.bin"
	.global Sound_WaveColossoLettering
Sound_WaveColossoLettering:
	.incbin "games/common/sound/direct_sound_samples/colosso_lettering.pcm8.bin"
