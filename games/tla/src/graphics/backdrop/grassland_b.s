	.section .rodata
	.global Battle_GrasslandBBackdrop
Battle_GrasslandBBackdrop:
	.incbin "games/tla/src/graphics/backdrop/grassland_b.gbapal"
	.incbin "games/tla/src/graphics/backdrop/grassland_b.bitmap.d7"
	.byte 0x3e, 0x77	@ PACKER: left in the buffer by Battle_ForestBackdrop+0x3b9e
