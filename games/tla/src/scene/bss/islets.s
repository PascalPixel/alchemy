.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.global gIslets_WaveTableIndex
gIslets_WaveTableIndex:
	.space 0x0000000c
	.ifdef TLA_EDITION_JA
	.space 0x00000004 @ the next object sits on 16 bytes, and the Japanese image ends 4 short of that
	.endif
	.global gIslets_WaveLines
gIslets_WaveLines:
	.space 0x00000280
	.global gIslets_SecondCrateZ
gIslets_SecondCrateZ:
	.space 0x00000004
	.global gIslets_FirstCrateZ
gIslets_FirstCrateZ:
	.space 0x00000004
	.global gIslets_SecondCrateX
gIslets_SecondCrateX:
	.space 0x00000004
	.global gIslets_FirstCrateX
gIslets_FirstCrateX:
