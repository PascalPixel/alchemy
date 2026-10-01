	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ Slots_SmallDoubleUpBanner: its 16-colour palette, then 16 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_SmallDoubleUpBanner
Slots_SmallDoubleUpBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/SLOTS/SMALL_DOUBLE_UP_JA.gbapal"
	.incbin "GRAPHICS/SLOTS/SMALL_DOUBLE_UP_JA.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/SLOTS/SMALL_DOUBLE_UP_DE.gbapal"
	.incbin "GRAPHICS/SLOTS/SMALL_DOUBLE_UP_DE.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/SLOTS/SMALL_DOUBLE_UP_ES.gbapal"
	.incbin "GRAPHICS/SLOTS/SMALL_DOUBLE_UP_ES.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/SLOTS/SMALL_DOUBLE_UP_FR.gbapal"
	.incbin "GRAPHICS/SLOTS/SMALL_DOUBLE_UP_FR.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/SLOTS/SMALL_DOUBLE_UP_IT.gbapal"
	.incbin "GRAPHICS/SLOTS/SMALL_DOUBLE_UP_IT.4bpp16x16.lz"
	.else
	.incbin "GRAPHICS/SLOTS/SMALL_DOUBLE_UP.gbapal"
	.incbin "GRAPHICS/SLOTS/SMALL_DOUBLE_UP.4bpp16x16.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
