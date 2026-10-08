	.section .rodata
	.global Tileset_Set43TilesA
Tileset_Set43TilesA:
	.incbin "GRAPHICS/TILESET/SET_43/TILES_A.4bpp.mtf"
	.byte 0x2b	@ PACKER: left in the buffer by Resource_Data221+0x28bf
