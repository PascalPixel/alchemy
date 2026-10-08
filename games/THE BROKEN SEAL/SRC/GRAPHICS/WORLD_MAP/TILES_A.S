	.section .rodata
	.global WorldMap_TilesA
WorldMap_TilesA:
	.incbin "GRAPHICS/WORLD_MAP/TILES_A.8bpp.lz"
	.byte 0x91, 0xb3	@ PACKER: left in the buffer by Field_DefaultMapCells+0x16ba
