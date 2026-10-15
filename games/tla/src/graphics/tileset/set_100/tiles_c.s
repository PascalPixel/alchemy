	.section .rodata
	.global Tileset_Set100TilesC
Tileset_Set100TilesC:
	.incbin "games/tla/src/graphics/tileset/set_100/tiles_c.4bpp.mtf"
	.byte 0x7b, 0xcf	@ PACKER: left in the buffer by Tileset_Set100TilesB+0x71e
