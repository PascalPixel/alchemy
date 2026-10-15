	.section .rodata
	.global Tileset_Set101TilesD
Tileset_Set101TilesD:
	.incbin "games/tla/src/graphics/tileset/set_101/tiles_d.4bpp.mtf"
	.byte 0x02, 0x09	@ PACKER: left in the buffer by Tileset_Set101TilesC+0xb6e
