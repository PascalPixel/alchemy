	.section .rodata
	.global Tileset_Set34TilesD
Tileset_Set34TilesD:
	.incbin "games/tla/src/graphics/tileset/set_34/tiles_d.4bpp.mtf"
	.byte 0x20, 0x3c	@ PACKER: left in the buffer by Tileset_Set34TilesC+0xf1e
