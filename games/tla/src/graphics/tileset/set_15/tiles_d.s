	.section .rodata
	.global Tileset_Set15TilesD
Tileset_Set15TilesD:
	.incbin "games/tla/src/graphics/tileset/set_15/tiles_d.4bpp.mtf"
	.byte 0x70	@ PACKER: left in the buffer by Tileset_Set15TilesC+0x13f
