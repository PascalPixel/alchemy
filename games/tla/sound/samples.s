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
	.global Sound_Wave00
Sound_Wave00:
	.incbin "games/tla/sound/sample/wave_00.pcm8.bin"
	.global Sound_Wave01
Sound_Wave01:
	.incbin "games/tla/sound/sample/wave_01.pcm8.bin"
	.global Sound_Wave02
Sound_Wave02:
	.incbin "games/tla/sound/sample/wave_02.pcm8.bin"
	.global Sound_Wave03
Sound_Wave03:
	.incbin "games/tla/sound/sample/wave_03.pcm8.bin"
	.global Sound_Wave04
Sound_Wave04:
	.incbin "games/tla/sound/sample/wave_04.pcm8.bin"
	.global Sound_Wave05
Sound_Wave05:
	.incbin "games/tla/sound/sample/wave_05.pcm8.bin"
	.global Sound_Wave06
Sound_Wave06:
	.incbin "games/tla/sound/sample/wave_06.pcm8.bin"
	.global Sound_Wave07
Sound_Wave07:
	.incbin "games/tla/sound/sample/wave_07.pcm8.bin"
	.global Sound_Wave08
Sound_Wave08:
	.incbin "games/tla/sound/sample/wave_08.pcm8.bin"
	.global Sound_Wave09
Sound_Wave09:
	.incbin "games/tla/sound/sample/wave_09.pcm8.bin"
	.global Sound_Wave10
Sound_Wave10:
	.incbin "games/tla/sound/sample/wave_10.pcm8.bin"
	.global Sound_Wave11
Sound_Wave11:
	.incbin "games/tla/sound/sample/wave_11.pcm8.bin"
	.global Sound_Wave12
Sound_Wave12:
	.incbin "games/tla/sound/sample/wave_12.pcm8.bin"
	.global Sound_Wave13
Sound_Wave13:
	.incbin "games/tla/sound/sample/wave_13.pcm8.bin"
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
	.global Sound_Wave14
Sound_Wave14:
	.incbin "games/tla/sound/sample/wave_14.pcm8.bin"
	.global Sound_Wave15
Sound_Wave15:
	.incbin "games/tla/sound/sample/wave_15.pcm8.bin"
	.global Sound_Wave16
Sound_Wave16:
	.incbin "games/tla/sound/sample/wave_16.pcm8.bin"
	.global Sound_Wave17
Sound_Wave17:
	.incbin "games/tla/sound/sample/wave_17.pcm8.bin"
	.global Sound_Wave18
Sound_Wave18:
	.incbin "games/tla/sound/sample/wave_18.pcm8.bin"
	.global Sound_Wave19
Sound_Wave19:
	.incbin "games/tla/sound/sample/wave_19.pcm8.bin"
	.global Sound_Wave20
Sound_Wave20:
	.incbin "games/tla/sound/sample/wave_20.pcm8.bin"
	.global Sound_Wave21
Sound_Wave21:
	.incbin "games/tla/sound/sample/wave_21.pcm8.bin"
	.global Sound_Wave22
Sound_Wave22:
	.incbin "games/tla/sound/sample/wave_22.pcm8.bin"
	.global Sound_Wave23
Sound_Wave23:
	.incbin "games/tla/sound/sample/wave_23.pcm8.bin"
	.global Sound_Wave24
Sound_Wave24:
	.incbin "games/tla/sound/sample/wave_24.pcm8.bin"
	.global Sound_Wave25
Sound_Wave25:
	.incbin "games/tla/sound/sample/wave_25.pcm8.bin"
	.global Sound_Wave26
Sound_Wave26:
	.incbin "games/tla/sound/sample/wave_26.pcm8.bin"
	.global Sound_Wave27
Sound_Wave27:
	.incbin "games/tla/sound/sample/wave_27.pcm8.bin"
	.global Sound_Wave28
Sound_Wave28:
	.incbin "games/tla/sound/sample/wave_28.pcm8.bin"
	.global Sound_Wave29
Sound_Wave29:
	.incbin "games/tla/sound/sample/wave_29.pcm8.bin"
	.global Sound_Wave30
Sound_Wave30:
	.incbin "games/tla/sound/sample/wave_30.pcm8.bin"
	.global Sound_Wave31
Sound_Wave31:
	.incbin "games/tla/sound/sample/wave_31.pcm8.bin"
	.global Sound_Wave32
Sound_Wave32:
	.incbin "games/tla/sound/sample/wave_32.pcm8.bin"
	.global Sound_Wave33
Sound_Wave33:
	.incbin "games/tla/sound/sample/wave_33.pcm8.bin"
	.global Sound_Wave34
Sound_Wave34:
	.incbin "games/tla/sound/sample/wave_34.pcm8.bin"
	.global Sound_Wave35
Sound_Wave35:
	.incbin "games/tla/sound/sample/wave_35.pcm8.bin"
	.global Sound_Wave36
Sound_Wave36:
	.incbin "games/tla/sound/sample/wave_36.pcm8.bin"
	.global Sound_Wave37
Sound_Wave37:
	.incbin "games/tla/sound/sample/wave_37.pcm8.bin"
	.global Sound_Wave38
Sound_Wave38:
	.incbin "games/tla/sound/sample/wave_38.pcm8.bin"
	.global Sound_Wave39
Sound_Wave39:
	.incbin "games/tla/sound/sample/wave_39.pcm8.bin"
	.global Sound_Wave40
Sound_Wave40:
	.incbin "games/tla/sound/sample/wave_40.pcm8.bin"
	.global Sound_Wave41
Sound_Wave41:
	.incbin "games/tla/sound/sample/wave_41.pcm8.bin"
	.global Sound_Wave42
Sound_Wave42:
	.incbin "games/tla/sound/sample/wave_42.pcm8.bin"
	.global Sound_Wave43
Sound_Wave43:
	.incbin "games/tla/sound/sample/wave_43.pcm8.bin"
	.global Sound_Wave44
Sound_Wave44:
	.incbin "games/tla/sound/sample/wave_44.pcm8.bin"
	.global Sound_Wave45
Sound_Wave45:
	.incbin "games/tla/sound/sample/wave_45.pcm8.bin"
	.global Sound_Wave46
Sound_Wave46:
	.incbin "games/tla/sound/sample/wave_46.pcm8.bin"
	.global Sound_Wave47
Sound_Wave47:
	.incbin "games/tla/sound/sample/wave_47.pcm8.bin"
	.global Sound_Wave48
Sound_Wave48:
	.incbin "games/tla/sound/sample/wave_48.pcm8.bin"
	.global Sound_Wave49
Sound_Wave49:
	.incbin "games/tla/sound/sample/wave_49.pcm8.bin"
	.global Sound_Wave50
Sound_Wave50:
	.incbin "games/tla/sound/sample/wave_50.pcm8.bin"
	.global Sound_Wave51
Sound_Wave51:
	.incbin "games/tla/sound/sample/wave_51.pcm8.bin"
	.global Sound_Wave52
Sound_Wave52:
	.incbin "games/tla/sound/sample/wave_52.pcm8.bin"
	.global Sound_Wave53
Sound_Wave53:
	.incbin "games/tla/sound/sample/wave_53.pcm8.bin"
	.global Sound_Wave54
Sound_Wave54:
	.incbin "games/tla/sound/sample/wave_54.pcm8.bin"
	.global Sound_Wave55
Sound_Wave55:
	.incbin "games/tla/sound/sample/wave_55.pcm8.bin"
	.global Sound_Wave56
Sound_Wave56:
	.incbin "games/tla/sound/sample/wave_56.pcm8.bin"
	.global Sound_Wave57
Sound_Wave57:
	.incbin "games/tla/sound/sample/wave_57.pcm8.bin"
	.global Sound_Wave58
Sound_Wave58:
	.incbin "games/tla/sound/sample/wave_58.pcm8.bin"
	.global Sound_Wave59
Sound_Wave59:
	.incbin "games/tla/sound/sample/wave_59.pcm8.bin"
	.global Sound_Wave60
Sound_Wave60:
	.incbin "games/tla/sound/sample/wave_60.pcm8.bin"
	.global Sound_Wave61
Sound_Wave61:
	.incbin "games/tla/sound/sample/wave_61.pcm8.bin"
	.global Sound_Wave62
Sound_Wave62:
	.incbin "games/tla/sound/sample/wave_62.pcm8.bin"
	.global Sound_Wave63
Sound_Wave63:
	.incbin "games/tla/sound/sample/wave_63.pcm8.bin"
	.global Sound_Wave64
Sound_Wave64:
	.incbin "games/tla/sound/sample/wave_64.pcm8.bin"
	.global Sound_Wave65
Sound_Wave65:
	.incbin "games/tla/sound/sample/wave_65.pcm8.bin"
	.global Sound_Wave66
Sound_Wave66:
	.incbin "games/tla/sound/sample/wave_66.pcm8.bin"
	.global Sound_Wave67
Sound_Wave67:
	.incbin "games/tla/sound/sample/wave_67.pcm8.bin"
	.global Sound_Wave68
Sound_Wave68:
	.incbin "games/tla/sound/sample/wave_68.pcm8.bin"
	.global Sound_Wave69
Sound_Wave69:
	.incbin "games/tla/sound/sample/wave_69.pcm8.bin"
	.global Sound_Wave70
Sound_Wave70:
	.incbin "games/tla/sound/sample/wave_70.pcm8.bin"
