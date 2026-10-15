	.section .rodata
	.global Tileset_Set119TilesC
Tileset_Set119TilesC:
	.incbin "games/tla/src/graphics/tileset/set_119/tiles_c.4bpp.mtf"
	.byte 0x06	@ PACKER: left in the buffer by Tileset_Set119TilesB+0x12bb
