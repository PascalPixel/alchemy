	.section .rodata
	.global Tileset_Set77TilesB
Tileset_Set77TilesB:
	.incbin "games/tla/src/graphics/tileset/set_77/tiles_b.4bpp.mtf"
	.byte 0x28	@ PACKER: left in the buffer by Tileset_Set77TilesA+0x11b7
