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
	.incbin "COMMON/SOUND/SAMPLE/WAVE_20.PCM8.bin"
	.global Sound_Wave21
Sound_Wave21:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_21.PCM8.bin"
	.global Sound_Wave22
Sound_Wave22:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_22.PCM8.bin"
	.global Sound_Wave23
Sound_Wave23:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_23.PCM8.bin"
	.global Sound_Wave24
Sound_Wave24:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_24.PCM8.bin"
	.global Sound_Wave25
Sound_Wave25:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_25.PCM8.bin"
	.global Sound_Wave26
Sound_Wave26:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_26.PCM8.bin"
	.global Sound_Wave27
Sound_Wave27:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_27.PCM8.bin"
	.global Sound_Wave28
Sound_Wave28:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_28.PCM8.bin"
	.global Sound_Wave29
Sound_Wave29:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_29.PCM8.bin"
	.global Sound_Wave30
Sound_Wave30:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_30.PCM8.bin"
	.global Sound_Wave31
Sound_Wave31:
	.incbin "COMMON/SOUND/SAMPLE/WAVE_31.PCM8.bin"
