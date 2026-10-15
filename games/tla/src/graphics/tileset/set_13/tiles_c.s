	.section .rodata
	.global Tileset_Set13TilesC
Tileset_Set13TilesC:
	.incbin "games/tla/src/graphics/tileset/set_13/tiles_c.4bpp.mtf"
	.byte 0xd1	@ PACKER: left in the buffer by Tileset_Set13TilesA+0x238f
