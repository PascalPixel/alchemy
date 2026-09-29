@ LuckyDice_Sprites: its 256-colour palette, then 496 tiles of 8-bit colour packed
@ by the resource packer.
	.section .rodata
	.global LuckyDice_Sprites
LuckyDice_Sprites:
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES_1.gbapal"
	.incbin "GRAPHICS/LUCKY_DICE/SPRITES.parts.lz"
