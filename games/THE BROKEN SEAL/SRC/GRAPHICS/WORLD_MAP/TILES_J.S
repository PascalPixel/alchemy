	.section .rodata
	.global WorldMap_TilesJ
WorldMap_TilesJ:
	.incbin "GRAPHICS/WORLD_MAP/TILES_J.8bpp.lz"
	/* PACKER: left in the packer's buffer by WorldMap_TilesF at +0x153f */
	.byte 0x52
