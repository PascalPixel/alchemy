@ LuckyDice_Sprites: its 256-colour palette, then 496 tiles of 8-bit colour packed
@ by the resource packer.
	.section .rodata
	.global LuckyDice_Sprites
LuckyDice_Sprites:
	.ifdef TBS_EDITION_JA
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_1_JA.gbapal"
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_JA.parts.lz"
	.else
	.ifdef TBS_EDITION_DE
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_1_DE.gbapal"
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_DE.parts.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_1_ES.gbapal"
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_ES.parts.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_1_FR.gbapal"
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_FR.parts.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_1_IT.gbapal"
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_IT.parts.lz"
	.else
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_1.gbapal"
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES.parts.lz"
	.endif
	.endif
	.endif
	.endif
	.endif
