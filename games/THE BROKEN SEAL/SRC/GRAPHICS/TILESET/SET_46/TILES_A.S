	.section .rodata
	.global Tileset_Set46TilesA
Tileset_Set46TilesA:
	.incbin "GRAPHICS/TILESET/SET_46/TILES_A.4bpp.mtf"
	.byte 0x63	@ PACKER: left in the buffer by Field_Map068+0x2d9b
