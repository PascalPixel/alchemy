	.section .rodata
	.global Tileset_Set13TilesA
Tileset_Set13TilesA:
	.incbin "GRAPHICS/TILESET/SET_13/TILES_A.4bpp.mtf"
	.byte 0xfe, 0x4c	@ PACKER: left in the buffer by Field_Map015+0x1fe2
