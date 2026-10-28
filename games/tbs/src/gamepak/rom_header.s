@ Why assembly: the cartridge header: the BIOS starts the game at its first word, a branch
@ to System_Entry inside a block of header data, a linkage no procedure-call
@ standard produces.
	.include "games/tbs/include/ed_asm.h"
	.section .rodata
	.arm
	.global Rom_Start
Rom_Start:
	b	System_Entry			@ Entry point
	.space	156				@ Nintendo logo, written by gbafix
	.ifdef TBS_EDITION_JA
	.ascii	"OugonTaiyo_A"			@ Game title
	.else
	.ifdef TBS_EDITION_EN
	.ascii	"Golden_Sun_A"
	.else
	.ascii	"GOLDEN_SUN_A"
	.endif
	.endif
	.ascii	"AGS"				@ Game code
	.global gRom_LanguageCode
gRom_LanguageCode:
	.ifdef TBS_EDITION_JA
	.ascii	"J"
	.endif
	.ifdef TBS_EDITION_EN
	.ascii	"E"
	.endif
	.ifdef TBS_EDITION_DE
	.ascii	"D"
	.endif
	.ifdef TBS_EDITION_ES
	.ascii	"S"
	.endif
	.ifdef TBS_EDITION_FR
	.ascii	"F"
	.endif
	.ifdef TBS_EDITION_IT
	.ascii	"I"
	.endif
	.ascii	"01"				@ Maker code
	.byte	0x96				@ Fixed value
	.byte	0				@ Main unit code
	.byte	0				@ Device type
	.space	7				@ Reserved
	.byte	0				@ Software version
	.byte	0				@ Complement check, calculated by gbafix
	.space	2				@ Reserved
