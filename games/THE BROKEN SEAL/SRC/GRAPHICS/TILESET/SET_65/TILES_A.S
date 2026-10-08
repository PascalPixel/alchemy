	.section .rodata
	.global Tileset_Set65TilesA
Tileset_Set65TilesA:
	.incbin "GRAPHICS/TILESET/SET_65/TILES_A.4bpp.mtf"
	.byte 0x94	@ PACKER: left in the buffer by Field_Map151+0x1e7f
