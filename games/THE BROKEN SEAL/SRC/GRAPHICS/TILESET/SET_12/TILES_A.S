	.section .rodata
	.global Tileset_Set12TilesA
Tileset_Set12TilesA:
	.incbin "GRAPHICS/TILESET/SET_12/TILES_A.4bpp.mtf"
	.byte 0x7a	@ PACKER: left in the buffer by Field_Map014+0x1887
