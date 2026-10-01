	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ Slots_Jackpot: its 16-colour palette, then 128 tiles of 4-bit colour packed
@ by the resource packer.
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
