	.section .rodata
	.global Tileset_Set83TilesC
Tileset_Set83TilesC:
	.incbin "games/tla/src/graphics/tileset/set_83/tiles_c.4bpp.mtf"
	.byte 0x95, 0xeb	@ PACKER: left in the buffer by Tileset_Set83TilesA+0x212e
