	.section .rodata
	.global Tileset_Set104TilesC
Tileset_Set104TilesC:
	.incbin "games/tla/src/graphics/tileset/set_104/tiles_c.4bpp.mtf"
	.byte 0xac	@ PACKER: left in the buffer by Tileset_Set104TilesA+0x1cb7
