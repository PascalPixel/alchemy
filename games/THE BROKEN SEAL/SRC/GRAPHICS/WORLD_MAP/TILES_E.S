	.section .rodata
	.global WorldMap_TilesE
WorldMap_TilesE:
	.incbin "GRAPHICS/WORLD_MAP/TILES_E.8bpp.lz"
	.byte 0x48	@ PACKER: left in the buffer by Field_DefaultMapCells+0x1ea7
