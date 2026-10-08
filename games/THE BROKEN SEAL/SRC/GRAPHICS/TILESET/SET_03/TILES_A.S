	.section .rodata
	.global Tileset_Set03TilesA
Tileset_Set03TilesA:
	.incbin "GRAPHICS/TILESET/SET_03/TILES_A.4bpp.mtf"
	.byte 0x1e	@ PACKER: left in the buffer by Field_Map003+0x283f
