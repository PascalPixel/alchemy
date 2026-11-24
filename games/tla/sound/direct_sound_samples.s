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
	.global Sound_WaveAcousticBassDrum
Sound_WaveAcousticBassDrum:
	.incbin "games/tla/sound/direct_sound_samples/acoustic_bass_drum.pcm8.bin"
	.global Sound_WaveBassDrum
Sound_WaveBassDrum:
	.incbin "games/tla/sound/direct_sound_samples/bass_drum.pcm8.bin"
	.global Sound_WaveSideStick
Sound_WaveSideStick:
	.incbin "games/tla/sound/direct_sound_samples/side_stick.pcm8.bin"
	.global Sound_WaveAcousticSnare
Sound_WaveAcousticSnare:
	.incbin "games/tla/sound/direct_sound_samples/acoustic_snare.pcm8.bin"
	.global Sound_WaveHandClap
Sound_WaveHandClap:
	.incbin "games/tla/sound/direct_sound_samples/hand_clap.pcm8.bin"
	.global Sound_WaveElectricSnare
Sound_WaveElectricSnare:
	.incbin "games/tla/sound/direct_sound_samples/electric_snare.pcm8.bin"
	.global Sound_WaveTom
Sound_WaveTom:
	.incbin "games/tla/sound/direct_sound_samples/tom.pcm8.bin"
	.global Sound_WaveClosedHiHat
Sound_WaveClosedHiHat:
	.incbin "games/tla/sound/direct_sound_samples/closed_hi_hat.pcm8.bin"
	.global Sound_WaveOpenHiHat
Sound_WaveOpenHiHat:
	.incbin "games/tla/sound/direct_sound_samples/open_hi_hat.pcm8.bin"
	.global Sound_WaveCrashCymbal
Sound_WaveCrashCymbal:
	.incbin "games/tla/sound/direct_sound_samples/crash_cymbal.pcm8.bin"
	.global Sound_WaveRideCymbal
Sound_WaveRideCymbal:
	.incbin "games/tla/sound/direct_sound_samples/ride_cymbal.pcm8.bin"
	.global Sound_WaveSwitch
Sound_WaveSwitch:
	.incbin "games/tla/sound/direct_sound_samples/switch.pcm8.bin"
	.global Sound_WaveRumble
Sound_WaveRumble:
	.incbin "games/tla/sound/direct_sound_samples/rumble.pcm8.bin"
	.global Sound_WaveCrash
Sound_WaveCrash:
	.incbin "games/tla/sound/direct_sound_samples/crash.pcm8.bin"
	.global Sound_WavePsynergySparkle
Sound_WavePsynergySparkle:
	.incbin "games/tla/sound/direct_sound_samples/psynergy_sparkle.pcm8.bin"
	.global Sound_WaveRoar
Sound_WaveRoar:
	.incbin "games/tla/sound/direct_sound_samples/roar.pcm8.bin"
	.global Sound_WaveThunder
Sound_WaveThunder:
	.incbin "games/tla/sound/direct_sound_samples/thunder.pcm8.bin"
	.global Sound_WaveSparks
Sound_WaveSparks:
	.incbin "games/tla/sound/direct_sound_samples/sparks.pcm8.bin"
	.global Sound_WaveCurse
Sound_WaveCurse:
	.incbin "games/tla/sound/direct_sound_samples/curse.pcm8.bin"
	.global Sound_WaveBoom
Sound_WaveBoom:
	.incbin "games/tla/sound/direct_sound_samples/boom.pcm8.bin"
	.global Sound_WaveBell
Sound_WaveBell:
	.incbin "games/tla/sound/direct_sound_samples/bell.pcm8.bin"
	.global Sound_WaveDrip
Sound_WaveDrip:
	.incbin "games/tla/sound/direct_sound_samples/drip.pcm8.bin"
	.global Sound_WaveChime
Sound_WaveChime:
	.incbin "games/tla/sound/direct_sound_samples/chime.pcm8.bin"
	.global Sound_WaveColossoLettering
Sound_WaveColossoLettering:
	.incbin "games/tla/sound/direct_sound_samples/colosso_lettering.pcm8.bin"
	.global Sound_WaveEyesGlow
Sound_WaveEyesGlow:
	.incbin "games/tla/sound/direct_sound_samples/eyes_glow.pcm8.bin"
	.global Sound_WaveRing
Sound_WaveRing:
	.incbin "games/tla/sound/direct_sound_samples/ring.pcm8.bin"
	.global Sound_WaveSpiral
Sound_WaveSpiral:
	.incbin "games/tla/sound/direct_sound_samples/spiral.pcm8.bin"
	.global Sound_WaveBloop
Sound_WaveBloop:
	.incbin "games/tla/sound/direct_sound_samples/bloop.pcm8.bin"
	.global Sound_WaveDoubleThud
Sound_WaveDoubleThud:
	.incbin "games/tla/sound/direct_sound_samples/double_thud.pcm8.bin"
	.global Sound_WavePound
Sound_WavePound:
	.incbin "games/tla/sound/direct_sound_samples/pound.pcm8.bin"
	.global Sound_WaveSeaOfTimeOpens
Sound_WaveSeaOfTimeOpens:
	.incbin "games/tla/sound/direct_sound_samples/sea_of_time_opens.pcm8.bin"
	.global Sound_WaveHiss
Sound_WaveHiss:
	.incbin "games/tla/sound/direct_sound_samples/hiss.pcm8.bin"
	.global Sound_WaveHighDrone
Sound_WaveHighDrone:
	.incbin "games/tla/sound/direct_sound_samples/high_drone.pcm8.bin"
	.global Sound_WaveHowl
Sound_WaveHowl:
	.incbin "games/tla/sound/direct_sound_samples/howl.pcm8.bin"
	.global Sound_WaveJawsOpen
Sound_WaveJawsOpen:
	.incbin "games/tla/sound/direct_sound_samples/jaws_open.pcm8.bin"
	.global Sound_WaveSurf
Sound_WaveSurf:
	.incbin "games/tla/sound/direct_sound_samples/surf.pcm8.bin"
	.global Sound_WaveIceCrack
Sound_WaveIceCrack:
	.incbin "games/tla/sound/direct_sound_samples/ice_crack.pcm8.bin"
	.global Sound_WaveStatueStirs
Sound_WaveStatueStirs:
	.incbin "games/tla/sound/direct_sound_samples/statue_stirs.pcm8.bin"
	.global Sound_WaveCurrent
Sound_WaveCurrent:
	.incbin "games/tla/sound/direct_sound_samples/current.pcm8.bin"
	.global Sound_WaveCrackle
Sound_WaveCrackle:
	.incbin "games/tla/sound/direct_sound_samples/crackle.pcm8.bin"
	.global Sound_WaveWail
Sound_WaveWail:
	.incbin "games/tla/sound/direct_sound_samples/wail.pcm8.bin"
	.global Sound_WaveWhoosh
Sound_WaveWhoosh:
	.incbin "games/tla/sound/direct_sound_samples/whoosh.pcm8.bin"
	.global Sound_WaveStatueRise
Sound_WaveStatueRise:
	.incbin "games/tla/sound/direct_sound_samples/statue_rise.pcm8.bin"
	.global Sound_WaveSpinAway
Sound_WaveSpinAway:
	.incbin "games/tla/sound/direct_sound_samples/spin_away.pcm8.bin"
	.global Sound_WaveBirdCall
Sound_WaveBirdCall:
	.incbin "games/tla/sound/direct_sound_samples/bird_call.pcm8.bin"
	.global Sound_WaveWade
Sound_WaveWade:
	.incbin "games/tla/sound/direct_sound_samples/wade.pcm8.bin"
