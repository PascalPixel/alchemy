	.section .rodata
	.global Tileset_Set43TilesA
Tileset_Set43TilesA:
	.incbin "graphics/tileset/set_43/tiles_a.4bpp.mtf"
	.byte 0x2b	@ PACKER: left in the buffer by Tileset_Set42TilesA+0x28bf
