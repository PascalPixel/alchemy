	.section .rodata
	.global Tileset_Set73TilesC
Tileset_Set73TilesC:
	.incbin "graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set73TilesA+0x13f
