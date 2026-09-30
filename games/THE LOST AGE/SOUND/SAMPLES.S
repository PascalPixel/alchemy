@ The waves the voices play, in the engine's wave record: a word whose bit
@ 30 loops the wave, the pitch at middle C in 1/1024 Hz, the loop start and
@ the last sample index, then signed 8-bit samples padded to a word. Each
@ sample is built from its WAV. A synthesised wave has no samples: its data
@ is the mixer's synthesis marker 0x80 and its waveform, pulse (0), saw (1)
@ or triangle (2), and a pulse adds its duty cycle and the speed, depth and
@ starting phase of the sweep that modulates the duty (RUNTIME_MIX_RENDER.S).
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
	.incbin "SOUND/SAMPLE/WAVE_00.PCM8.bin"
	.global Sound_Wave01
Sound_Wave01:
	.incbin "SOUND/SAMPLE/WAVE_01.PCM8.bin"
	.global Sound_Wave02
Sound_Wave02:
	.incbin "SOUND/SAMPLE/WAVE_02.PCM8.bin"
	.global Sound_Wave03
Sound_Wave03:
	.incbin "SOUND/SAMPLE/WAVE_03.PCM8.bin"
	.global Sound_Wave04
Sound_Wave04:
	.incbin "SOUND/SAMPLE/WAVE_04.PCM8.bin"
	.global Sound_Wave05
Sound_Wave05:
	.incbin "SOUND/SAMPLE/WAVE_05.PCM8.bin"
	.global Sound_Wave06
Sound_Wave06:
	.incbin "SOUND/SAMPLE/WAVE_06.PCM8.bin"
	.global Sound_Wave07
Sound_Wave07:
	.incbin "SOUND/SAMPLE/WAVE_07.PCM8.bin"
	.global Sound_Wave08
Sound_Wave08:
	.incbin "SOUND/SAMPLE/WAVE_08.PCM8.bin"
	.global Sound_Wave09
Sound_Wave09:
	.incbin "SOUND/SAMPLE/WAVE_09.PCM8.bin"
	.global Sound_Wave10
Sound_Wave10:
	.incbin "SOUND/SAMPLE/WAVE_10.PCM8.bin"
	.global Sound_Wave11
Sound_Wave11:
	.incbin "SOUND/SAMPLE/WAVE_11.PCM8.bin"
	.global Sound_Wave12
Sound_Wave12:
	.incbin "SOUND/SAMPLE/WAVE_12.PCM8.bin"
	.global Sound_Wave13
Sound_Wave13:
	.incbin "SOUND/SAMPLE/WAVE_13.PCM8.bin"
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
	.incbin "SOUND/SAMPLE/WAVE_14.PCM8.bin"
	.global Sound_Wave15
Sound_Wave15:
	.incbin "SOUND/SAMPLE/WAVE_15.PCM8.bin"
	.global Sound_Wave16
Sound_Wave16:
	.incbin "SOUND/SAMPLE/WAVE_16.PCM8.bin"
	.global Sound_Wave17
Sound_Wave17:
	.incbin "SOUND/SAMPLE/WAVE_17.PCM8.bin"
	.global Sound_Wave18
Sound_Wave18:
	.incbin "SOUND/SAMPLE/WAVE_18.PCM8.bin"
	.global Sound_Wave19
Sound_Wave19:
	.incbin "SOUND/SAMPLE/WAVE_19.PCM8.bin"
	.global Sound_Wave20
Sound_Wave20:
	.incbin "SOUND/SAMPLE/WAVE_20.PCM8.bin"
	.global Sound_Wave21
Sound_Wave21:
	.incbin "SOUND/SAMPLE/WAVE_21.PCM8.bin"
	.global Sound_Wave22
Sound_Wave22:
	.incbin "SOUND/SAMPLE/WAVE_22.PCM8.bin"
	.global Sound_Wave23
Sound_Wave23:
	.incbin "SOUND/SAMPLE/WAVE_23.PCM8.bin"
	.global Sound_Wave24
Sound_Wave24:
	.incbin "SOUND/SAMPLE/WAVE_24.PCM8.bin"
	.global Sound_Wave25
Sound_Wave25:
	.incbin "SOUND/SAMPLE/WAVE_25.PCM8.bin"
	.global Sound_Wave26
Sound_Wave26:
	.incbin "SOUND/SAMPLE/WAVE_26.PCM8.bin"
	.global Sound_Wave27
Sound_Wave27:
	.incbin "SOUND/SAMPLE/WAVE_27.PCM8.bin"
	.global Sound_Wave28
Sound_Wave28:
	.incbin "SOUND/SAMPLE/WAVE_28.PCM8.bin"
	.global Sound_Wave29
Sound_Wave29:
	.incbin "SOUND/SAMPLE/WAVE_29.PCM8.bin"
	.global Sound_Wave30
Sound_Wave30:
	.incbin "SOUND/SAMPLE/WAVE_30.PCM8.bin"
	.global Sound_Wave31
Sound_Wave31:
	.incbin "SOUND/SAMPLE/WAVE_31.PCM8.bin"
	.global Sound_Wave32
Sound_Wave32:
	.incbin "SOUND/SAMPLE/WAVE_32.PCM8.bin"
	.global Sound_Wave33
Sound_Wave33:
	.incbin "SOUND/SAMPLE/WAVE_33.PCM8.bin"
	.global Sound_Wave34
Sound_Wave34:
	.incbin "SOUND/SAMPLE/WAVE_34.PCM8.bin"
	.global Sound_Wave35
Sound_Wave35:
	.incbin "SOUND/SAMPLE/WAVE_35.PCM8.bin"
	.global Sound_Wave36
Sound_Wave36:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_20.PCM8.bin"
	.global Sound_Wave37
Sound_Wave37:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_21.PCM8.bin"
	.global Sound_Wave38
Sound_Wave38:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_22.PCM8.bin"
	.global Sound_Wave39
Sound_Wave39:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_23.PCM8.bin"
	.global Sound_Wave40
Sound_Wave40:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_24.PCM8.bin"
	.global Sound_Wave41
Sound_Wave41:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_25.PCM8.bin"
	.global Sound_Wave42
Sound_Wave42:
	.incbin "SOUND/SAMPLE/WAVE_42.PCM8.bin"
	.global Sound_Wave43
Sound_Wave43:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_26.PCM8.bin"
	.global Sound_Wave44
Sound_Wave44:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_27.PCM8.bin"
	.global Sound_Wave45
Sound_Wave45:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_28.PCM8.bin"
	.global Sound_Wave46
Sound_Wave46:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_29.PCM8.bin"
	.global Sound_Wave47
Sound_Wave47:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_30.PCM8.bin"
	.global Sound_Wave48
Sound_Wave48:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_31.PCM8.bin"
	.global Sound_Wave49
Sound_Wave49:
	.incbin "SOUND/SAMPLE/WAVE_49.PCM8.bin"
	.global Sound_Wave50
Sound_Wave50:
	.incbin "SOUND/SAMPLE/WAVE_50.PCM8.bin"
	.global Sound_Wave51
Sound_Wave51:
	.incbin "SOUND/SAMPLE/WAVE_51.PCM8.bin"
	.global Sound_Wave52
Sound_Wave52:
	.incbin "SOUND/SAMPLE/WAVE_52.PCM8.bin"
	.global Sound_Wave53
Sound_Wave53:
	.incbin "SOUND/SAMPLE/WAVE_53.PCM8.bin"
	.global Sound_Wave54
Sound_Wave54:
	.incbin "SOUND/SAMPLE/WAVE_54.PCM8.bin"
	.global Sound_Wave55
Sound_Wave55:
	.incbin "SOUND/SAMPLE/WAVE_55.PCM8.bin"
	.global Sound_Wave56
Sound_Wave56:
	.incbin "SOUND/SAMPLE/WAVE_56.PCM8.bin"
	.global Sound_Wave57
Sound_Wave57:
	.incbin "SOUND/SAMPLE/WAVE_57.PCM8.bin"
	.global Sound_Wave58
Sound_Wave58:
	.incbin "SOUND/SAMPLE/WAVE_58.PCM8.bin"
	.global Sound_Wave59
Sound_Wave59:
	.incbin "SOUND/SAMPLE/WAVE_59.PCM8.bin"
	.global Sound_Wave60
Sound_Wave60:
	.incbin "SOUND/SAMPLE/WAVE_60.PCM8.bin"
	.global Sound_Wave61
Sound_Wave61:
	.incbin "SOUND/SAMPLE/WAVE_61.PCM8.bin"
	.global Sound_Wave62
Sound_Wave62:
	.incbin "SOUND/SAMPLE/WAVE_62.PCM8.bin"
	.global Sound_Wave63
Sound_Wave63:
	.incbin "SOUND/SAMPLE/WAVE_63.PCM8.bin"
	.global Sound_Wave64
Sound_Wave64:
	.incbin "SOUND/SAMPLE/WAVE_64.PCM8.bin"
	.global Sound_Wave65
Sound_Wave65:
	.incbin "SOUND/SAMPLE/WAVE_65.PCM8.bin"
	.global Sound_Wave66
Sound_Wave66:
	.incbin "SOUND/SAMPLE/WAVE_66.PCM8.bin"
	.global Sound_Wave67
Sound_Wave67:
	.incbin "SOUND/SAMPLE/WAVE_67.PCM8.bin"
	.global Sound_Wave68
Sound_Wave68:
	.incbin "SOUND/SAMPLE/WAVE_68.PCM8.bin"
	.global Sound_Wave69
Sound_Wave69:
	.incbin "SOUND/SAMPLE/WAVE_69.PCM8.bin"
	.global Sound_Wave70
Sound_Wave70:
	.incbin "SOUND/SAMPLE/WAVE_70.PCM8.bin"
