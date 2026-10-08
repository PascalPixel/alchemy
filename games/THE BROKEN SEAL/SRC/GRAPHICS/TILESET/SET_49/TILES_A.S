	.section .rodata
	.global Tileset_Set49TilesA
Tileset_Set49TilesA:
	.incbin "GRAPHICS/TILESET/SET_49/TILES_A.4bpp.mtf"
	.byte 0x00, 0xc1, 0x01	@ PACKER: left in the buffer by Field_Map076+0x2345
