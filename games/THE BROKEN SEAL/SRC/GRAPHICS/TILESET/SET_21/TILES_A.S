	.section .rodata
	.global Tileset_Set21TilesA
Tileset_Set21TilesA:
	.incbin "GRAPHICS/TILESET/SET_21/TILES_A.4bpp.mtf"
	.byte 0xdc, 0x06, 0xaa	@ PACKER: left in the buffer by Field_Map027+0x2bad
