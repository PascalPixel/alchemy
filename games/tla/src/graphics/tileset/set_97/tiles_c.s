	.section .rodata
	.global Tileset_Set97TilesC
Tileset_Set97TilesC:
	.incbin "games/tla/src/graphics/tileset/set_97/tiles_c.4bpp.mtf"
	.byte 0x28	@ PACKER: left in the buffer by Tileset_Set97TilesA+0x1e33
