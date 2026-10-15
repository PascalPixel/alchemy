	.section .rodata
	.global Tileset_Set106TilesC
Tileset_Set106TilesC:
	.incbin "games/tla/src/graphics/tileset/set_106/tiles_c.4bpp.mtf"
	.byte 0x95, 0x3b	@ PACKER: left in the buffer by Tileset_Set106TilesB+0x1b7a
