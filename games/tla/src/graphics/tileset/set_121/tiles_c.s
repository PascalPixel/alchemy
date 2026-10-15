	.section .rodata
	.global Tileset_Set121TilesC
Tileset_Set121TilesC:
	.incbin "games/tla/src/graphics/tileset/set_121/tiles_c.4bpp.mtf"
	.byte 0x06	@ PACKER: left in the buffer by Tileset_Set121TilesB+0x12bb
