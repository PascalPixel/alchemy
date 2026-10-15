	.section .rodata
	.global Tileset_Set106TilesD
Tileset_Set106TilesD:
	.incbin "games/tla/src/graphics/tileset/set_106/tiles_d.4bpp.mtf"
	.byte 0x82, 0x94, 0x06	@ PACKER: left in the buffer by Tileset_Set106TilesC+0x449
