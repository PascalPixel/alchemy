	.section .rodata
	.global Tileset_Set33TilesA
Tileset_Set33TilesA:
	.incbin "GRAPHICS/TILESET/SET_33/TILES_A.4bpp.mtf"
	.byte 0x00, 0x80, 0x57	@ PACKER: left in the buffer by Field_Map042+0x1145
