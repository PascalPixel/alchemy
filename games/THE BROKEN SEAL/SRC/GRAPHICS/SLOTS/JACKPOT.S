@ Slots_Jackpot: its 16-colour palette, then 128 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_Jackpot
Slots_Jackpot:
	.ifdef TBS_EDITION_JA
	.incbin "GRAPHICS/SLOTS/JACKPOT_1_JA.gbapal"
	.incbin "GRAPHICS/SLOTS/JACKPOT_JA.parts.lz"
	.else
	.incbin "GRAPHICS/SLOTS/JACKPOT_1.gbapal"
	.incbin "GRAPHICS/SLOTS/JACKPOT.parts.lz"
	.endif
