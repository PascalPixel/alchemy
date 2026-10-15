	.section .rodata
	.global Tileset_Set90TilesD
Tileset_Set90TilesD:
	.incbin "games/tla/src/graphics/tileset/set_90/tiles_d.4bpp.mtf"
	.byte 0xe1	@ PACKER: left in the buffer by Tileset_Set90TilesC+0x5db
