	.section .rodata
	.global Tileset_Set25TilesA
Tileset_Set25TilesA:
	.incbin "GRAPHICS/TILESET/SET_25/TILES_A.4bpp.mtf"
	.byte 0x06	@ PACKER: left in the buffer by Field_Map032+0x1c57
