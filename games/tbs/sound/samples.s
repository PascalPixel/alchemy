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
	.incbin "games/tbs/sound/sample/wave_00.pcm8.bin"
	.global Sound_Wave01
Sound_Wave01:
	.incbin "games/tbs/sound/sample/wave_01.pcm8.bin"
	.global Sound_Wave02
Sound_Wave02:
	.incbin "games/tbs/sound/sample/wave_02.pcm8.bin"
	.global Sound_Wave03
Sound_Wave03:
	.incbin "games/tbs/sound/sample/wave_03.pcm8.bin"
	.global Sound_Wave04
Sound_Wave04:
	.incbin "games/tbs/sound/sample/wave_04.pcm8.bin"
	.global Sound_Wave05
Sound_Wave05:
	.incbin "games/tbs/sound/sample/wave_05.pcm8.bin"
	.global Sound_Wave06
Sound_Wave06:
	.incbin "games/tbs/sound/sample/wave_06.pcm8.bin"
	.global Sound_Wave07
Sound_Wave07:
	.incbin "games/tbs/sound/sample/wave_07.pcm8.bin"
	.global Sound_Wave08
Sound_Wave08:
	.incbin "games/tbs/sound/sample/wave_08.pcm8.bin"
	.global Sound_Wave09
Sound_Wave09:
	.incbin "games/tbs/sound/sample/wave_09.pcm8.bin"
	.global Sound_Wave10
Sound_Wave10:
	.incbin "games/tbs/sound/sample/wave_10.pcm8.bin"
	.global Sound_Wave11
Sound_Wave11:
	.incbin "games/tbs/sound/sample/wave_11.pcm8.bin"
	.global Sound_Wave12
Sound_Wave12:
	.incbin "games/tbs/sound/sample/wave_12.pcm8.bin"
	.global Sound_Wave13
Sound_Wave13:
	.incbin "games/tbs/sound/sample/wave_13.pcm8.bin"
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
	.incbin "games/tbs/sound/sample/wave_14.pcm8.bin"
	.global Sound_Wave15
Sound_Wave15:
	.incbin "games/tbs/sound/sample/wave_15.pcm8.bin"
	.global Sound_Wave16
Sound_Wave16:
	.incbin "games/tbs/sound/sample/wave_16.pcm8.bin"
	.global Sound_Wave17
Sound_Wave17:
	.incbin "games/tbs/sound/sample/wave_17.pcm8.bin"
	.global Sound_Wave18
Sound_Wave18:
	.incbin "games/tbs/sound/sample/wave_18.pcm8.bin"
	.global Sound_Wave19
Sound_Wave19:
	.incbin "games/tbs/sound/sample/wave_19.pcm8.bin"
	.global Sound_Wave20
Sound_Wave20:
	.incbin "games/common/sound/sample/wave_20.pcm8.bin"
	.global Sound_Wave21
Sound_Wave21:
	.incbin "games/common/sound/sample/wave_21.pcm8.bin"
	.global Sound_Wave22
Sound_Wave22:
	.incbin "games/common/sound/sample/wave_22.pcm8.bin"
	.global Sound_Wave23
Sound_Wave23:
	.incbin "games/common/sound/sample/wave_23.pcm8.bin"
	.global Sound_Wave24
Sound_Wave24:
	.incbin "games/common/sound/sample/wave_24.pcm8.bin"
	.global Sound_Wave25
Sound_Wave25:
	.incbin "games/common/sound/sample/wave_25.pcm8.bin"
	.global Sound_Wave26
Sound_Wave26:
	.incbin "games/common/sound/sample/wave_26.pcm8.bin"
	.global Sound_Wave27
Sound_Wave27:
	.incbin "games/common/sound/sample/wave_27.pcm8.bin"
	.global Sound_Wave28
Sound_Wave28:
	.incbin "games/common/sound/sample/wave_28.pcm8.bin"
	.global Sound_Wave29
Sound_Wave29:
	.incbin "games/common/sound/sample/wave_29.pcm8.bin"
	.global Sound_Wave30
Sound_Wave30:
	.incbin "games/common/sound/sample/wave_30.pcm8.bin"
	.global Sound_Wave31
Sound_Wave31:
	.incbin "games/common/sound/sample/wave_31.pcm8.bin"
