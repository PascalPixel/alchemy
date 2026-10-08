	.section .rodata
	.global Tileset_Set68TilesB
Tileset_Set68TilesB:
	.incbin "graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set68TilesA+0x13f
