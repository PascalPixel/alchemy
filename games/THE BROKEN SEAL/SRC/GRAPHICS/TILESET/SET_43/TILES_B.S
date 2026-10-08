	.section .rodata
	.global Tileset_Set43TilesB
Tileset_Set43TilesB:
	.incbin "GRAPHICS/TILESET/SET_43/TILES_B.4bpp.mtf"
	.byte 0xe5	@ PACKER: left in the buffer by Tileset_Set43TilesA+0x27c7
