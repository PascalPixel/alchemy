	.section .rodata
	.global Tileset_Set43TilesC
Tileset_Set43TilesC:
	.incbin "GRAPHICS/TILESET/SET_43/TILES_C.4bpp.mtf"
	.byte 0x7a, 0x40, 0xe2	@ PACKER: left in the buffer by Tileset_Set43TilesB+0x12f9
