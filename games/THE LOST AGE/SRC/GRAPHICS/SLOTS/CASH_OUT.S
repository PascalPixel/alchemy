	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ Slots_CashOutBanner: its 16-colour palette, then 80 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_CashOutBanner
Slots_CashOutBanner:
	.ifeq EDITION_INTERNATIONAL
	.incbin "GRAPHICS/SLOTS/CASH_OUT_JA.gbapal"
	.incbin "GRAPHICS/SLOTS/CASH_OUT_JA.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/SLOTS/CASH_OUT_DE.gbapal"
	.incbin "GRAPHICS/SLOTS/CASH_OUT_DE.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/SLOTS/CASH_OUT_ES.gbapal"
	.incbin "GRAPHICS/SLOTS/CASH_OUT_ES.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/SLOTS/CASH_OUT_FR.gbapal"
	.incbin "GRAPHICS/SLOTS/CASH_OUT_FR.4bpp32x32.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/SLOTS/CASH_OUT_IT.gbapal"
	.incbin "GRAPHICS/SLOTS/CASH_OUT_IT.4bpp32x32.lz"
	.else
	.incbin "GRAPHICS/SLOTS/CASH_OUT.gbapal"
	.incbin "GRAPHICS/SLOTS/CASH_OUT.4bpp32x32.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
