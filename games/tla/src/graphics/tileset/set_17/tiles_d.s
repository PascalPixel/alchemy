	.section .rodata
	.global Tileset_Set17TilesD
Tileset_Set17TilesD:
	.incbin "games/tla/src/graphics/tileset/set_17/tiles_d.4bpp.mtf"
	.byte 0x70	@ PACKER: left in the buffer by Tileset_Set17TilesC+0x13f
