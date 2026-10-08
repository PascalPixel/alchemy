	.section .rodata
	.global WorldMap_TilesD
WorldMap_TilesD:
	.incbin "GRAPHICS/WORLD_MAP/TILES_D.8bpp.lz"
	/* PACKER: left in the packer's buffer by WorldMap_TilesC at +0xb72 */
	.byte 0x12, 0x9f
