	.section .rodata
	.global WorldMap_TilesG
WorldMap_TilesG:
	.incbin "GRAPHICS/WORLD_MAP/TILES_G.8bpp.lz"
	.byte 0x20, 0x75	@ PACKER: left in the buffer by WorldMap_TilesF+0x14ae
