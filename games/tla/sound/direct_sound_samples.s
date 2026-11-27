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
	.incbin "games/tla/sound/direct_sound_samples/celesta.pcm8.bin"
	.global SoundWaveNylonGuitar
SoundWaveNylonGuitar:
	.incbin "games/tla/sound/direct_sound_samples/nylon_guitar.pcm8.bin"
	.global SoundWaveFingerBass
SoundWaveFingerBass:
	.incbin "games/tla/sound/direct_sound_samples/finger_bass.pcm8.bin"
	.global SoundWavePizzicatoStrings
SoundWavePizzicatoStrings:
	.incbin "games/tla/sound/direct_sound_samples/pizzicato_strings.pcm8.bin"
	.global SoundWaveHarp
SoundWaveHarp:
	.incbin "games/tla/sound/direct_sound_samples/harp.pcm8.bin"
	.global SoundWaveTimpani
SoundWaveTimpani:
	.incbin "games/tla/sound/direct_sound_samples/timpani.pcm8.bin"
	.global SoundWaveStrings
SoundWaveStrings:
	.incbin "games/tla/sound/direct_sound_samples/strings.pcm8.bin"
	.global SoundWaveChoir
SoundWaveChoir:
	.incbin "games/tla/sound/direct_sound_samples/choir.pcm8.bin"
	.global SoundWaveTrumpet
SoundWaveTrumpet:
	.incbin "games/tla/sound/direct_sound_samples/trumpet.pcm8.bin"
	.global SoundWaveBrass
SoundWaveBrass:
	.incbin "games/tla/sound/direct_sound_samples/brass.pcm8.bin"
	.global SoundWaveOboe
SoundWaveOboe:
	.incbin "games/tla/sound/direct_sound_samples/oboe.pcm8.bin"
	.global SoundWavePiccolo
SoundWavePiccolo:
	.incbin "games/tla/sound/direct_sound_samples/piccolo.pcm8.bin"
	.global SoundWaveFlute
SoundWaveFlute:
	.incbin "games/tla/sound/direct_sound_samples/flute.pcm8.bin"
	.global SoundWavePanFlute
SoundWavePanFlute:
	.incbin "games/tla/sound/direct_sound_samples/pan_flute.pcm8.bin"
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
	.global SoundWaveBanjo
SoundWaveBanjo:
	.incbin "games/tla/sound/direct_sound_samples/banjo.pcm8.bin"
	.global SoundWaveShamisen
SoundWaveShamisen:
	.incbin "games/tla/sound/direct_sound_samples/shamisen.pcm8.bin"
	.global SoundWaveKoto
SoundWaveKoto:
	.incbin "games/tla/sound/direct_sound_samples/koto.pcm8.bin"
	.global SoundWaveKalimba
SoundWaveKalimba:
	.incbin "games/tla/sound/direct_sound_samples/kalimba.pcm8.bin"
	.global SoundWaveBagpipe
SoundWaveBagpipe:
	.incbin "games/tla/sound/direct_sound_samples/bagpipe.pcm8.bin"
	.global SoundWaveFiddle
SoundWaveFiddle:
	.incbin "games/tla/sound/direct_sound_samples/fiddle.pcm8.bin"
	.global SoundWaveShanai
SoundWaveShanai:
	.incbin "games/tla/sound/direct_sound_samples/shanai.pcm8.bin"
	.global SoundWaveTinkleBell
SoundWaveTinkleBell:
	.incbin "games/tla/sound/direct_sound_samples/tinkle_bell.pcm8.bin"
	.global SoundWaveAgogo
SoundWaveAgogo:
	.incbin "games/tla/sound/direct_sound_samples/agogo.pcm8.bin"
	.global SoundWaveSteelDrums
SoundWaveSteelDrums:
	.incbin "games/tla/sound/direct_sound_samples/steel_drums.pcm8.bin"
	.global SoundWaveTaikoDrum
SoundWaveTaikoDrum:
	.incbin "games/tla/sound/direct_sound_samples/taiko_drum.pcm8.bin"
	.global SoundWaveAcousticBassDrum
SoundWaveAcousticBassDrum:
	.incbin "games/tla/sound/direct_sound_samples/acoustic_bass_drum.pcm8.bin"
	.global SoundWaveBassDrum
SoundWaveBassDrum:
	.incbin "games/tla/sound/direct_sound_samples/bass_drum.pcm8.bin"
	.global SoundWaveSideStick
SoundWaveSideStick:
	.incbin "games/tla/sound/direct_sound_samples/side_stick.pcm8.bin"
	.global SoundWaveAcousticSnare
SoundWaveAcousticSnare:
	.incbin "games/tla/sound/direct_sound_samples/acoustic_snare.pcm8.bin"
	.global SoundWaveHandClap
SoundWaveHandClap:
	.incbin "games/tla/sound/direct_sound_samples/hand_clap.pcm8.bin"
	.global SoundWaveElectricSnare
SoundWaveElectricSnare:
	.incbin "games/tla/sound/direct_sound_samples/electric_snare.pcm8.bin"
	.global SoundWaveTom
SoundWaveTom:
	.incbin "games/tla/sound/direct_sound_samples/tom.pcm8.bin"
	.global SoundWaveClosedHiHat
SoundWaveClosedHiHat:
	.incbin "games/tla/sound/direct_sound_samples/closed_hi_hat.pcm8.bin"
	.global SoundWaveOpenHiHat
SoundWaveOpenHiHat:
	.incbin "games/tla/sound/direct_sound_samples/open_hi_hat.pcm8.bin"
	.global SoundWaveCrashCymbal
SoundWaveCrashCymbal:
	.incbin "games/tla/sound/direct_sound_samples/crash_cymbal.pcm8.bin"
	.global SoundWaveRideCymbal
SoundWaveRideCymbal:
	.incbin "games/tla/sound/direct_sound_samples/ride_cymbal.pcm8.bin"
	.global SoundWaveSwitch
SoundWaveSwitch:
	.incbin "games/tla/sound/direct_sound_samples/switch.pcm8.bin"
	.global SoundWaveRumble
SoundWaveRumble:
	.incbin "games/tla/sound/direct_sound_samples/rumble.pcm8.bin"
	.global SoundWaveCrash
SoundWaveCrash:
	.incbin "games/tla/sound/direct_sound_samples/crash.pcm8.bin"
	.global SoundWavePsynergySparkle
SoundWavePsynergySparkle:
	.incbin "games/tla/sound/direct_sound_samples/psynergy_sparkle.pcm8.bin"
	.global SoundWaveRoar
SoundWaveRoar:
	.incbin "games/tla/sound/direct_sound_samples/roar.pcm8.bin"
	.global SoundWaveThunder
SoundWaveThunder:
	.incbin "games/tla/sound/direct_sound_samples/thunder.pcm8.bin"
	.global SoundWaveSparks
SoundWaveSparks:
	.incbin "games/tla/sound/direct_sound_samples/sparks.pcm8.bin"
	.global SoundWaveCurse
SoundWaveCurse:
	.incbin "games/tla/sound/direct_sound_samples/curse.pcm8.bin"
	.global SoundWaveBoom
SoundWaveBoom:
	.incbin "games/tla/sound/direct_sound_samples/boom.pcm8.bin"
	.global SoundWaveBell
SoundWaveBell:
	.incbin "games/tla/sound/direct_sound_samples/bell.pcm8.bin"
	.global SoundWaveDrip
SoundWaveDrip:
	.incbin "games/tla/sound/direct_sound_samples/drip.pcm8.bin"
	.global SoundWaveChime
SoundWaveChime:
	.incbin "games/tla/sound/direct_sound_samples/chime.pcm8.bin"
	.global SoundWaveColossoLettering
SoundWaveColossoLettering:
	.incbin "games/tla/sound/direct_sound_samples/colosso_lettering.pcm8.bin"
	.global SoundWaveEyesGlow
SoundWaveEyesGlow:
	.incbin "games/tla/sound/direct_sound_samples/eyes_glow.pcm8.bin"
	.global SoundWaveRing
SoundWaveRing:
	.incbin "games/tla/sound/direct_sound_samples/ring.pcm8.bin"
	.global SoundWaveSpiral
SoundWaveSpiral:
	.incbin "games/tla/sound/direct_sound_samples/spiral.pcm8.bin"
	.global SoundWaveBloop
SoundWaveBloop:
	.incbin "games/tla/sound/direct_sound_samples/bloop.pcm8.bin"
	.global SoundWaveDoubleThud
SoundWaveDoubleThud:
	.incbin "games/tla/sound/direct_sound_samples/double_thud.pcm8.bin"
	.global SoundWavePound
SoundWavePound:
	.incbin "games/tla/sound/direct_sound_samples/pound.pcm8.bin"
	.global SoundWaveSeaOfTimeOpens
SoundWaveSeaOfTimeOpens:
	.incbin "games/tla/sound/direct_sound_samples/sea_of_time_opens.pcm8.bin"
	.global SoundWaveHiss
SoundWaveHiss:
	.incbin "games/tla/sound/direct_sound_samples/hiss.pcm8.bin"
	.global SoundWaveHighDrone
SoundWaveHighDrone:
	.incbin "games/tla/sound/direct_sound_samples/high_drone.pcm8.bin"
	.global SoundWaveHowl
SoundWaveHowl:
	.incbin "games/tla/sound/direct_sound_samples/howl.pcm8.bin"
	.global SoundWaveJawsOpen
SoundWaveJawsOpen:
	.incbin "games/tla/sound/direct_sound_samples/jaws_open.pcm8.bin"
	.global SoundWaveSurf
SoundWaveSurf:
	.incbin "games/tla/sound/direct_sound_samples/surf.pcm8.bin"
	.global SoundWaveIceCrack
SoundWaveIceCrack:
	.incbin "games/tla/sound/direct_sound_samples/ice_crack.pcm8.bin"
	.global SoundWaveStatueStirs
SoundWaveStatueStirs:
	.incbin "games/tla/sound/direct_sound_samples/statue_stirs.pcm8.bin"
	.global SoundWaveCurrent
SoundWaveCurrent:
	.incbin "games/tla/sound/direct_sound_samples/current.pcm8.bin"
	.global SoundWaveCrackle
SoundWaveCrackle:
	.incbin "games/tla/sound/direct_sound_samples/crackle.pcm8.bin"
	.global SoundWaveWail
SoundWaveWail:
	.incbin "games/tla/sound/direct_sound_samples/wail.pcm8.bin"
	.global SoundWaveWhoosh
SoundWaveWhoosh:
	.incbin "games/tla/sound/direct_sound_samples/whoosh.pcm8.bin"
	.global SoundWaveStatueRise
SoundWaveStatueRise:
	.incbin "games/tla/sound/direct_sound_samples/statue_rise.pcm8.bin"
	.global SoundWaveSpinAway
SoundWaveSpinAway:
	.incbin "games/tla/sound/direct_sound_samples/spin_away.pcm8.bin"
	.global SoundWaveBirdCall
SoundWaveBirdCall:
	.incbin "games/tla/sound/direct_sound_samples/bird_call.pcm8.bin"
	.global SoundWaveWade
SoundWaveWade:
	.incbin "games/tla/sound/direct_sound_samples/wade.pcm8.bin"
