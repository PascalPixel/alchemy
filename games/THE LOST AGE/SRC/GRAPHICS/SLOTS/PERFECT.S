	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ Slots_PerfectBanner: its 16-colour palette, then 128 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_PerfectBanner
Slots_PerfectBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/SLOTS/PERFECT_JA.gbapal"
	.incbin "GRAPHICS/SLOTS/PERFECT_JA.4bpp.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/SLOTS/PERFECT_DE.gbapal"
	.incbin "GRAPHICS/SLOTS/PERFECT_DE.4bpp.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/SLOTS/PERFECT_ES.gbapal"
	.incbin "GRAPHICS/SLOTS/PERFECT_ES.4bpp.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/SLOTS/PERFECT_FR.gbapal"
	.incbin "GRAPHICS/SLOTS/PERFECT_FR.4bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/SLOTS/PERFECT_IT.gbapal"
	.incbin "GRAPHICS/SLOTS/PERFECT_IT.4bpp.lz"
	.else
	.incbin "GRAPHICS/SLOTS/PERFECT.gbapal"
	.incbin "GRAPHICS/SLOTS/PERFECT.4bpp.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
