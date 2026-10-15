	.section .rodata
	.global Tileset_Set53TilesD
Tileset_Set53TilesD:
	.incbin "games/tla/src/graphics/tileset/set_53/tiles_d.4bpp.mtf"
	.byte 0x7c, 0xfe, 0x81	@ PACKER: left in the buffer by Tileset_Set53TilesC+0x3a1
