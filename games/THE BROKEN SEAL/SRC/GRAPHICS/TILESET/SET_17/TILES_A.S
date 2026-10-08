	.section .rodata
	.global Tileset_Set17TilesA
Tileset_Set17TilesA:
	.incbin "GRAPHICS/TILESET/SET_17/TILES_A.4bpp.mtf"
	.byte 0xe1, 0xe8, 0xf4	@ PACKER: left in the buffer by Field_Map006+0x2d51
