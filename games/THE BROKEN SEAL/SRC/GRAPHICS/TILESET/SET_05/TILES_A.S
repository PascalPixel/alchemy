	.section .rodata
	.global Tileset_Set05TilesA
Tileset_Set05TilesA:
	.incbin "GRAPHICS/TILESET/SET_05/TILES_A.4bpp.mtf"
	.byte 0x86, 0x47	@ PACKER: left in the buffer by Field_Map006+0x1dae
