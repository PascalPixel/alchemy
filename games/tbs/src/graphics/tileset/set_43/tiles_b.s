	.section .rodata
	.global Tileset_Set43TilesB
Tileset_Set43TilesB:
	.incbin "graphics/tileset/set_43/tiles_b.4bpp.mtf"
	.byte 0xe5	@ PACKER: left in the buffer by Tileset_Set43TilesA+0x27c7
