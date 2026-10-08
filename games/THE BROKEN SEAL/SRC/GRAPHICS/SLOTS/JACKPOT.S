	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Slots_Jackpot
Slots_Jackpot:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/SLOTS/JACKPOT_1_JA.gbapal"
	.incbin "GRAPHICS/SLOTS/JACKPOT_JA.parts.lz"
	.else
	.incbin "GRAPHICS/SLOTS/JACKPOT_1.gbapal"
	.incbin "GRAPHICS/SLOTS/JACKPOT.parts.lz"
	.endif
	/* PACKER: left in the packer's buffer by earlier streams, which differ by edition */
	.ifdef TBS_EDITION_JA
	.byte 0xa2
	.else
	.byte 0x39
	.endif
