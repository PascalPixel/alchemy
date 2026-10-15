	.section .rodata
	.global Tileset_Set41TilesD
Tileset_Set41TilesD:
	.incbin "games/tla/src/graphics/tileset/set_41/tiles_d.4bpp.mtf"
	.byte 0x13	@ PACKER: left in the buffer by Tileset_Set41TilesA+0xc17
