	.include "games/tla/include/game/ed_asm.h"
	.section .rodata
	.arm
	.global Cartridge_Restart
Cartridge_Restart:
	b	Entry				@ Entry.
	.space	156				@ Nintendo logo (written by gbafix).
	.ifdef TLA_EDITION_JA
	.ascii	"OUGONTAIYO_B"			@ Game title.
	.else
	.ascii	"GOLDEN_SUN_B"
	.endif
	.ascii	"AGF"				@ Game code.
	.ifdef TLA_EDITION_JA
	.ascii	"J"
	.endif
	.ifdef TLA_EDITION_EN
	.ascii	"E"
	.endif
	.ifdef TLA_EDITION_DE
	.ascii	"D"
	.endif
	.ifdef TLA_EDITION_ES
	.ascii	"S"
	.endif
	.ifdef TLA_EDITION_FR
	.ascii	"F"
	.endif
	.ifdef TLA_EDITION_IT
	.ascii	"I"
	.endif
	.ascii	"01"				@ Maker code.
	.byte	0x96				@ Fixed value.
	.byte	0				@ Main unit code.
	.byte	0				@ Device type.
	.space	7				@ Reserved.
	.byte	0				@ Software version.
	.byte	0				@ Complement check (calculated by gbafix).
	.space	2				@ Reserved.
