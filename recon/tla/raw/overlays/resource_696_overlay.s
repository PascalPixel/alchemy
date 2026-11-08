
	.include "games/tla/include/ed_asm.h"
	.set SHAMAN_GRAPHIC1_SCALE, 256
	.set SHAMAN_GRAPHIC4_START_SCALE, 512
	.set SHAMAN_GRAPHIC4_SMALL_SCALE, 256
	.set SHAMAN_GRAPHIC4_LARGE_SCALE, 384
	.ifdef TLA_EDITION_ES
	.set SHAMAN_GRAPHIC4_START_SCALE, 384
	.endif
	.ifdef TLA_EDITION_IT
	.set SHAMAN_GRAPHIC4_START_SCALE, 384
	.endif
	.ifdef TLA_EDITION_FR
	.set SHAMAN_GRAPHIC1_SCALE, 384
	.set SHAMAN_GRAPHIC4_SMALL_SCALE, 384
	.set SHAMAN_GRAPHIC4_LARGE_SCALE, 512
	.endif
	.set SHAMAN_GRAPHIC7_PALETTE, (2 * 32)
	.ifdef TLA_EDITION_DE
	.set SHAMAN_GRAPHIC7_PALETTE, (5 * 32)
	.endif
	.ifdef TLA_EDITION_ES
	.set SHAMAN_GRAPHIC7_PALETTE, (5 * 32)
	.endif
	.ifdef TLA_EDITION_FR
	.set SHAMAN_GRAPHIC7_PALETTE, (5 * 32)
	.endif
	.ifdef TLA_EDITION_IT
	.set SHAMAN_GRAPHIC7_PALETTE, (5 * 32)
	.endif
.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.space 0x00000004
	.global gData_02006350
gData_02006350:
	.space 0x00000140
	.global gData_02006490
gData_02006490:
	.space 0x00000004
	.global gData_02006494
gData_02006494:
	.space 0x00000004
	.global gData_02006498
gData_02006498:
	.space 0x00000004
	.global gData_0200649c
gData_0200649c:
	.space 0x00000004
	.global gData_020064a0
gData_020064a0:
	.space 0x00000004
	.global gData_020064a4
gData_020064a4:
	.space 0x00000004
	.global gData_020064a8
gData_020064a8:
	.space 0x00000004
	.global gData_020064ac
gData_020064ac:
	.space 0x00000004
	.global gData_020064b0
gData_020064b0:
	.space 0x00000004
	.global gData_020064b4
gData_020064b4:
	.space 0x00000004
	.global gData_020064b8
gData_020064b8:
	.space 0x00000004
	.global gData_020064bc
gData_020064bc:
	.space 0x00000004
	.global gData_020064c0
gData_020064c0:
	.space 0x00000004
	.global gData_020064c4
gData_020064c4:
	.space 0x0000000c
	.global gData_020064d0
gData_020064d0:
	.space 0x00000030
	.global gData_02006500
gData_02006500:
	.space 0x00000004
	.global gData_02006504
gData_02006504:
	.space 0x00000004
	.global gData_02006508
gData_02006508:
