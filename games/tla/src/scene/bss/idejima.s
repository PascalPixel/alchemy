.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.space 0x00000008
	.global gIdejima_WaveLines
gIdejima_WaveLines:
	.space 0x00000280
	.global gIdejima_WavePhase
gIdejima_WavePhase:
	.space 0x00000004
	.global gIdejima_WaveBuffer
gIdejima_WaveBuffer:
