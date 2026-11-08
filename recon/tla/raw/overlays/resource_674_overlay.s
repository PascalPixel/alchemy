.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.global gData_02004f04
gData_02004f04:
	.space 0x0000000c
	.ifdef TLA_EDITION_JA
	.space 0x00000004 @ the next object sits on 16 bytes, and the Japanese image ends 4 short of that
	.endif
	.global gData_02004f10
gData_02004f10:
	.space 0x00000280
	.global gData_02005190
gData_02005190:
	.space 0x00000004
	.global gData_02005194
gData_02005194:
	.space 0x00000004
	.global gData_02005198
gData_02005198:
	.space 0x00000004
	.global gData_0200519c
gData_0200519c:
