	.section .rodata
	.global Tileset_Set100TilesB
Tileset_Set100TilesB:
	.incbin "games/tla/src/graphics/tileset/set_100/tiles_b.4bpp.mtf"
	.byte 0xc7	@ PACKER: left in the buffer by Tileset_Set100TilesA+0x1acb
