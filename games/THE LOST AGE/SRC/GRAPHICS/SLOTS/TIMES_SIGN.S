@ Slots_TimesSign: its 16-colour palette, then 12 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_TimesSign
Slots_TimesSign:
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/SLOTS/TIMES_SIGN_DE.gbapal"
	.incbin "GRAPHICS/SLOTS/TIMES_SIGN_DE.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/SLOTS/TIMES_SIGN_ES.gbapal"
	.incbin "GRAPHICS/SLOTS/TIMES_SIGN_ES.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/SLOTS/TIMES_SIGN_FR.gbapal"
	.incbin "GRAPHICS/SLOTS/TIMES_SIGN_FR.4bpp16x16.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/SLOTS/TIMES_SIGN_IT.gbapal"
	.incbin "GRAPHICS/SLOTS/TIMES_SIGN_IT.4bpp16x16.lz"
	.else
	.incbin "GRAPHICS/SLOTS/TIMES_SIGN.gbapal"
	.incbin "GRAPHICS/SLOTS/TIMES_SIGN.4bpp16x16.lz"
	.endif
	.endif
	.endif
	.endif
