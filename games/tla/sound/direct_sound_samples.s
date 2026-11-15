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
	.incbin "games/tla/sound/direct_sound_samples/celesta.pcm8.bin"
	.global Sound_WaveNylonGuitar
Sound_WaveNylonGuitar:
	.incbin "games/tla/sound/direct_sound_samples/nylon_guitar.pcm8.bin"
	.global Sound_WaveFingerBass
Sound_WaveFingerBass:
	.incbin "games/tla/sound/direct_sound_samples/finger_bass.pcm8.bin"
	.global Sound_WavePizzicatoStrings
Sound_WavePizzicatoStrings:
	.incbin "games/tla/sound/direct_sound_samples/pizzicato_strings.pcm8.bin"
	.global Sound_WaveHarp
Sound_WaveHarp:
	.incbin "games/tla/sound/direct_sound_samples/harp.pcm8.bin"
	.global Sound_WaveTimpani
Sound_WaveTimpani:
	.incbin "games/tla/sound/direct_sound_samples/timpani.pcm8.bin"
	.global Sound_WaveStrings
Sound_WaveStrings:
	.incbin "games/tla/sound/direct_sound_samples/strings.pcm8.bin"
	.global Sound_WaveChoir
Sound_WaveChoir:
	.incbin "games/tla/sound/direct_sound_samples/choir.pcm8.bin"
	.global Sound_WaveTrumpet
Sound_WaveTrumpet:
	.incbin "games/tla/sound/direct_sound_samples/trumpet.pcm8.bin"
	.global Sound_WaveBrass
Sound_WaveBrass:
	.incbin "games/tla/sound/direct_sound_samples/brass.pcm8.bin"
	.global Sound_WaveOboe
Sound_WaveOboe:
	.incbin "games/tla/sound/direct_sound_samples/oboe.pcm8.bin"
	.global Sound_WavePiccolo
Sound_WavePiccolo:
	.incbin "games/tla/sound/direct_sound_samples/piccolo.pcm8.bin"
	.global Sound_WaveFlute
Sound_WaveFlute:
	.incbin "games/tla/sound/direct_sound_samples/flute.pcm8.bin"
	.global Sound_WavePanFlute
Sound_WavePanFlute:
	.incbin "games/tla/sound/direct_sound_samples/pan_flute.pcm8.bin"
	.global Sound_SynthPulse0
Sound_SynthPulse0:
	synth_pulse 0x10, 0xf0, 0xe0, 0x80
	.global Sound_SynthPulse1
Sound_SynthPulse1:
	synth_pulse 0x60, 0x20, 0x40, 0x80
	.global Sound_SynthPulse2
Sound_SynthPulse2:
	synth_pulse 0x80, 0x40, 0x40, 0x40
	.global Sound_SynthSaw
Sound_SynthSaw:
	synth_saw
	.global Sound_SynthTriangle
Sound_SynthTriangle:
	synth_triangle
	.global Sound_WaveBanjo
Sound_WaveBanjo:
	.incbin "games/tla/sound/direct_sound_samples/banjo.pcm8.bin"
	.global Sound_WaveShamisen
Sound_WaveShamisen:
	.incbin "games/tla/sound/direct_sound_samples/shamisen.pcm8.bin"
	.global Sound_WaveKoto
Sound_WaveKoto:
	.incbin "games/tla/sound/direct_sound_samples/koto.pcm8.bin"
	.global Sound_WaveKalimba
Sound_WaveKalimba:
	.incbin "games/tla/sound/direct_sound_samples/kalimba.pcm8.bin"
	.global Sound_WaveBagpipe
Sound_WaveBagpipe:
	.incbin "games/tla/sound/direct_sound_samples/bagpipe.pcm8.bin"
	.global Sound_WaveFiddle
Sound_WaveFiddle:
	.incbin "games/tla/sound/direct_sound_samples/fiddle.pcm8.bin"
	.global Sound_WaveShanai
Sound_WaveShanai:
	.incbin "games/tla/sound/direct_sound_samples/shanai.pcm8.bin"
	.global Sound_WaveTinkleBell
Sound_WaveTinkleBell:
	.incbin "games/tla/sound/direct_sound_samples/tinkle_bell.pcm8.bin"
	.global Sound_WaveAgogo
Sound_WaveAgogo:
	.incbin "games/tla/sound/direct_sound_samples/agogo.pcm8.bin"
	.global Sound_WaveSteelDrums
Sound_WaveSteelDrums:
	.incbin "games/tla/sound/direct_sound_samples/steel_drums.pcm8.bin"
	.global Sound_WaveTaikoDrum
Sound_WaveTaikoDrum:
	.incbin "games/tla/sound/direct_sound_samples/taiko_drum.pcm8.bin"
	.global Sound_Wave25
Sound_Wave25:
	.incbin "games/tla/sound/direct_sound_samples/wave_25.pcm8.bin"
	.global Sound_Wave26
Sound_Wave26:
	.incbin "games/tla/sound/direct_sound_samples/wave_26.pcm8.bin"
	.global Sound_Wave27
Sound_Wave27:
	.incbin "games/tla/sound/direct_sound_samples/wave_27.pcm8.bin"
	.global Sound_Wave28
Sound_Wave28:
	.incbin "games/tla/sound/direct_sound_samples/wave_28.pcm8.bin"
	.global Sound_Wave29
Sound_Wave29:
	.incbin "games/tla/sound/direct_sound_samples/wave_29.pcm8.bin"
	.global Sound_Wave30
Sound_Wave30:
	.incbin "games/tla/sound/direct_sound_samples/wave_30.pcm8.bin"
	.global Sound_Wave31
Sound_Wave31:
	.incbin "games/tla/sound/direct_sound_samples/wave_31.pcm8.bin"
	.global Sound_Wave32
Sound_Wave32:
	.incbin "games/tla/sound/direct_sound_samples/wave_32.pcm8.bin"
	.global Sound_Wave33
Sound_Wave33:
	.incbin "games/tla/sound/direct_sound_samples/wave_33.pcm8.bin"
	.global Sound_Wave34
Sound_Wave34:
	.incbin "games/tla/sound/direct_sound_samples/wave_34.pcm8.bin"
	.global Sound_Wave35
Sound_Wave35:
	.incbin "games/tla/sound/direct_sound_samples/wave_35.pcm8.bin"
	.global Sound_WaveSwitch
Sound_WaveSwitch:
	.incbin "games/tla/sound/direct_sound_samples/switch.pcm8.bin"
	.global Sound_WaveRumble
Sound_WaveRumble:
	.incbin "games/tla/sound/direct_sound_samples/rumble.pcm8.bin"
	.global Sound_WaveCrash
Sound_WaveCrash:
	.incbin "games/tla/sound/direct_sound_samples/crash.pcm8.bin"
	.global Sound_Wave39
Sound_Wave39:
	.incbin "games/tla/sound/direct_sound_samples/wave_39.pcm8.bin"
	.global Sound_WaveRoar
Sound_WaveRoar:
	.incbin "games/tla/sound/direct_sound_samples/roar.pcm8.bin"
	.global Sound_Wave41
Sound_Wave41:
	.incbin "games/tla/sound/direct_sound_samples/wave_41.pcm8.bin"
	.global Sound_Wave42
Sound_Wave42:
	.incbin "games/tla/sound/direct_sound_samples/wave_42.pcm8.bin"
	.global Sound_Wave43
Sound_Wave43:
	.incbin "games/tla/sound/direct_sound_samples/wave_43.pcm8.bin"
	.global Sound_Wave44
Sound_Wave44:
	.incbin "games/tla/sound/direct_sound_samples/wave_44.pcm8.bin"
	.global Sound_WaveBell
Sound_WaveBell:
	.incbin "games/tla/sound/direct_sound_samples/bell.pcm8.bin"
	.global Sound_WaveDrip
Sound_WaveDrip:
	.incbin "games/tla/sound/direct_sound_samples/drip.pcm8.bin"
	.global Sound_WaveChime
Sound_WaveChime:
	.incbin "games/tla/sound/direct_sound_samples/chime.pcm8.bin"
	.global Sound_Wave48
Sound_Wave48:
	.incbin "games/tla/sound/direct_sound_samples/wave_48.pcm8.bin"
	.global Sound_Wave49
Sound_Wave49:
	.incbin "games/tla/sound/direct_sound_samples/wave_49.pcm8.bin"
	.global Sound_Wave50
Sound_Wave50:
	.incbin "games/tla/sound/direct_sound_samples/wave_50.pcm8.bin"
	.global Sound_Wave51
Sound_Wave51:
	.incbin "games/tla/sound/direct_sound_samples/wave_51.pcm8.bin"
	.global Sound_Wave52
Sound_Wave52:
	.incbin "games/tla/sound/direct_sound_samples/wave_52.pcm8.bin"
	.global Sound_Wave53
Sound_Wave53:
	.incbin "games/tla/sound/direct_sound_samples/wave_53.pcm8.bin"
	.global Sound_WavePound
Sound_WavePound:
	.incbin "games/tla/sound/direct_sound_samples/pound.pcm8.bin"
	.global Sound_Wave55
Sound_Wave55:
	.incbin "games/tla/sound/direct_sound_samples/wave_55.pcm8.bin"
	.global Sound_Wave56
Sound_Wave56:
	.incbin "games/tla/sound/direct_sound_samples/wave_56.pcm8.bin"
	.global Sound_Wave57
Sound_Wave57:
	.incbin "games/tla/sound/direct_sound_samples/wave_57.pcm8.bin"
	.global Sound_WaveHowl
Sound_WaveHowl:
	.incbin "games/tla/sound/direct_sound_samples/howl.pcm8.bin"
	.global Sound_Wave59
Sound_Wave59:
	.incbin "games/tla/sound/direct_sound_samples/wave_59.pcm8.bin"
	.global Sound_Wave60
Sound_Wave60:
	.incbin "games/tla/sound/direct_sound_samples/wave_60.pcm8.bin"
	.global Sound_WaveIceCrack
Sound_WaveIceCrack:
	.incbin "games/tla/sound/direct_sound_samples/ice_crack.pcm8.bin"
	.global Sound_Wave62
Sound_Wave62:
	.incbin "games/tla/sound/direct_sound_samples/wave_62.pcm8.bin"
	.global Sound_WaveCurrent
Sound_WaveCurrent:
	.incbin "games/tla/sound/direct_sound_samples/current.pcm8.bin"
	.global Sound_Wave64
Sound_Wave64:
	.incbin "games/tla/sound/direct_sound_samples/wave_64.pcm8.bin"
	.global Sound_Wave65
Sound_Wave65:
	.incbin "games/tla/sound/direct_sound_samples/wave_65.pcm8.bin"
	.global Sound_Wave66
Sound_Wave66:
	.incbin "games/tla/sound/direct_sound_samples/wave_66.pcm8.bin"
	.global Sound_Wave67
Sound_Wave67:
	.incbin "games/tla/sound/direct_sound_samples/wave_67.pcm8.bin"
	.global Sound_WaveSpinAway
Sound_WaveSpinAway:
	.incbin "games/tla/sound/direct_sound_samples/spin_away.pcm8.bin"
	.global Sound_Wave69
Sound_Wave69:
	.incbin "games/tla/sound/direct_sound_samples/wave_69.pcm8.bin"
	.global Sound_WaveWade
Sound_WaveWade:
	.incbin "games/tla/sound/direct_sound_samples/wade.pcm8.bin"
