	.section .rodata
	.global Tileset_Set101TilesA
Tileset_Set101TilesA:
	.incbin "games/tla/src/graphics/tileset/set_101/tiles_a.4bpp.mtf"
	.byte 0x5b	@ PACKER: left in the buffer by Tileset_Set100TilesA+0x1fd7
