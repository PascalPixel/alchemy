	.section .rodata
	.global Tileset_Set59TilesA
Tileset_Set59TilesA:
	.incbin "GRAPHICS/TILESET/SET_59/TILES_A.4bpp.mtf"
	.byte 0xe0	@ PACKER: left in the buffer by Field_Map123+0x2a13
