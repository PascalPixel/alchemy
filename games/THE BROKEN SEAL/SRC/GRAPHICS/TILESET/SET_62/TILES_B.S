	.section .rodata
	.global Tileset_Set62TilesB
Tileset_Set62TilesB:
	.incbin "GRAPHICS/TILESET/SET_62/TILES_B.4bpp.mtf"
	.byte 0xba	@ PACKER: left in the buffer by Tileset_Set62TilesA+0x1c43
