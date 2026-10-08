	.section .rodata
	.global Tileset_Set62TilesD
Tileset_Set62TilesD:
	.incbin "GRAPHICS/TILESET/SET_62/TILES_D.4bpp.mtf"
	.byte 0xa3	@ PACKER: left in the buffer by Tileset_Set62TilesB+0x1b7b
