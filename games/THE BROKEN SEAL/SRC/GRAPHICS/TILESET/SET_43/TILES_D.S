	.section .rodata
	.global Tileset_Set43TilesD
Tileset_Set43TilesD:
	.incbin "GRAPHICS/TILESET/SET_43/TILES_D.4bpp.mtf"
	.byte 0x0c	@ PACKER: left in the buffer by Tileset_Set43TilesB+0x1943
