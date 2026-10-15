	.section .rodata
	.global Tileset_Set19TilesC
Tileset_Set19TilesC:
	.incbin "games/tla/src/graphics/tileset/set_19/tiles_c.4bpp.mtf"
	.byte 0x83, 0xb4, 0x9c	@ PACKER: left in the buffer by Tileset_Set19TilesB+0x1f5
