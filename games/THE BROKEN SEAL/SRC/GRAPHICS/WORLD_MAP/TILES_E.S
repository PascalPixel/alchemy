	.section .rodata
	.global WorldMap_TilesE
WorldMap_TilesE:
	.incbin "GRAPHICS/WORLD_MAP/TILES_E.8bpp.lz"
	/* PACKER: left in the packer's buffer by Field_DefaultMapCells at +0x1ea7 */
	.byte 0x48
