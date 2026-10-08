	.section .rodata
	.global Tileset_Set02TilesA
Tileset_Set02TilesA:
	.incbin "GRAPHICS/TILESET/SET_02/TILES_A.4bpp.mtf"
	.byte 0xfc, 0x89, 0x00	@ PACKER: left in the buffer by Field_Map003+0x27b9
