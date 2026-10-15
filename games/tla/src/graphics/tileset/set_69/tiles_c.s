	.section .rodata
	.global Tileset_Set69TilesC
Tileset_Set69TilesC:
	.incbin "games/tla/src/graphics/tileset/set_69/tiles_c.4bpp.mtf"
	.byte 0x72, 0x1c	@ PACKER: left in the buffer by Tileset_Set69TilesB+0xbae
