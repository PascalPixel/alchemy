@ Slots_Sprites: its 256-colour palette, then 496 tiles of 8-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_Sprites
Slots_Sprites:
	.ifdef TLA_EDITION_JA
	.incbin "GRAPHICS/SLOTS/SPRITES_1_JA.gbapal"
	.incbin "GRAPHICS/SLOTS/SPRITES_JA.parts.lz"
	.else
	.ifdef TLA_EDITION_DE
	.incbin "GRAPHICS/SLOTS/SPRITES_1_DE.gbapal"
	.incbin "GRAPHICS/SLOTS/SPRITES_DE.parts.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "GRAPHICS/SLOTS/SPRITES_1_ES.gbapal"
	.incbin "GRAPHICS/SLOTS/SPRITES_ES.parts.lz"
	.else
	.ifdef TLA_EDITION_FR
	.incbin "GRAPHICS/SLOTS/SPRITES_1_FR.gbapal"
	.incbin "GRAPHICS/SLOTS/SPRITES_FR.parts.lz"
	.else
	.ifdef TLA_EDITION_IT
	.incbin "GRAPHICS/SLOTS/SPRITES_1_IT.gbapal"
	.incbin "GRAPHICS/SLOTS/SPRITES_IT.parts.lz"
	.else
	.incbin "GRAPHICS/SLOTS/SPRITES_1.gbapal"
	.incbin "GRAPHICS/SLOTS/SPRITES.parts.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
