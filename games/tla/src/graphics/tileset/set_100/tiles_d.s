	.section .rodata
	.global Tileset_Set100TilesD
Tileset_Set100TilesD:
	.incbin "games/tla/src/graphics/tileset/set_100/tiles_d.4bpp.mtf"
	.byte 0x12, 0x05, 0x4a	@ PACKER: left in the buffer by Tileset_Set100TilesB+0x13bd
