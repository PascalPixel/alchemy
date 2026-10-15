	.section .rodata
	.global Tileset_Set97TilesD
Tileset_Set97TilesD:
	.incbin "games/tla/src/graphics/tileset/set_97/tiles_d.4bpp.mtf"
	.byte 0x05, 0xe0	@ PACKER: left in the buffer by Tileset_Set97TilesC+0xab6
