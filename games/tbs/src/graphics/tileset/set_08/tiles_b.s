	.section .rodata
	.global Tileset_Set08TilesB
Tileset_Set08TilesB:
	.incbin "graphics/tileset/set_08/tiles_b.4bpp.mtf"
	.byte 0x18	@ PACKER: left in the buffer by Tileset_Set08TilesA+0x1a4b
