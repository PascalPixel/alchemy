@ Slots_YouLoseBanner: its 16-colour palette, then 128 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_YouLoseBanner
Slots_YouLoseBanner:
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/SLOTS/YOU_LOSE_DE.gbapal"
	.incbin "GRAPHICS/SLOTS/YOU_LOSE_DE.4bpp.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/SLOTS/YOU_LOSE_ES.gbapal"
	.incbin "GRAPHICS/SLOTS/YOU_LOSE_ES.4bpp.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/SLOTS/YOU_LOSE_FR.gbapal"
	.incbin "GRAPHICS/SLOTS/YOU_LOSE_FR.4bpp.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/SLOTS/YOU_LOSE_IT.gbapal"
	.incbin "GRAPHICS/SLOTS/YOU_LOSE_IT.4bpp.lz"
	.else
	.incbin "GRAPHICS/SLOTS/YOU_LOSE.gbapal"
	.incbin "GRAPHICS/SLOTS/YOU_LOSE.4bpp.lz"
	.endif
	.endif
	.endif
	.endif
