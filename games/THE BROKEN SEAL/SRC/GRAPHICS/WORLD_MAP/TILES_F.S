	.section .rodata
	.global WorldMap_TilesF
WorldMap_TilesF:
	.incbin "GRAPHICS/WORLD_MAP/TILES_F.8bpp.lz"
	.byte 0x4a, 0xa8, 0x82	@ PACKER: left in the buffer by WorldMap_TilesE+0x159d
