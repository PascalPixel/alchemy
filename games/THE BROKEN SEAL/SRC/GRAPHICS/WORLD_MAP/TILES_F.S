	.section .rodata
	.global WorldMap_TilesF
WorldMap_TilesF:
	.incbin "GRAPHICS/WORLD_MAP/TILES_F.8bpp.lz"
	/* PACKER: left in the packer's buffer by WorldMap_TilesE at +0x159d */
	.byte 0x4a, 0xa8, 0x82
