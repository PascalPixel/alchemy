@ Slots_Sprites: its 256-colour palette, then 496 tiles of 8-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Slots_Sprites
Slots_Sprites:
	.incbin "GRAPHICS/SLOTS/SPRITES_1.gbapal"
	.incbin "GRAPHICS/SLOTS/SPRITES.parts.lz"
