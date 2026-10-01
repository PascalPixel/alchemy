	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ Slots_LowerBanner: its 16-colour palette, then 48 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_LowerBanner
Slots_LowerBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/SLOTS/LOWER_JA.gbapal"
	.incbin "GRAPHICS/SLOTS/LOWER_JA.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/SLOTS/LOWER_DE.gbapal"
	.incbin "GRAPHICS/SLOTS/LOWER_DE.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/SLOTS/LOWER_ES.gbapal"
	.incbin "GRAPHICS/SLOTS/LOWER_ES.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/SLOTS/LOWER_FR.gbapal"
	.incbin "GRAPHICS/SLOTS/LOWER_FR.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/SLOTS/LOWER_IT.gbapal"
	.incbin "GRAPHICS/SLOTS/LOWER_IT.4bpp32x32.lz"
	.else
	.incbin "GRAPHICS/SLOTS/LOWER.gbapal"
	.incbin "GRAPHICS/SLOTS/LOWER.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
