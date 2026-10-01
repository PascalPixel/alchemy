	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ Slots_OnePairBanner: its 16-colour palette, then 128 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_OnePairBanner
Slots_OnePairBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/SLOTS/ONE_PAIR_JA.gbapal"
	.incbin "GRAPHICS/SLOTS/ONE_PAIR_JA.4bpp.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/SLOTS/ONE_PAIR_DE.gbapal"
	.incbin "GRAPHICS/SLOTS/ONE_PAIR_DE.4bpp.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/SLOTS/ONE_PAIR_ES.gbapal"
	.incbin "GRAPHICS/SLOTS/ONE_PAIR_ES.4bpp.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/SLOTS/ONE_PAIR_FR.gbapal"
	.incbin "GRAPHICS/SLOTS/ONE_PAIR_FR.4bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/SLOTS/ONE_PAIR_IT.gbapal"
	.incbin "GRAPHICS/SLOTS/ONE_PAIR_IT.4bpp.lz"
	.else
	.incbin "GRAPHICS/SLOTS/ONE_PAIR.gbapal"
	.incbin "GRAPHICS/SLOTS/ONE_PAIR.4bpp.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
