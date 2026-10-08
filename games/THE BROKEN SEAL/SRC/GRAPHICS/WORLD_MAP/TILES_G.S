	.section .rodata
	.global WorldMap_TilesG
WorldMap_TilesG:
	.incbin "GRAPHICS/WORLD_MAP/TILES_G.8bpp.lz"
	/* PACKER: left in the packer's buffer by WorldMap_TilesF at +0x14ae */
	.byte 0x20, 0x75
