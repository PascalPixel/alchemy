	.section .rodata
	.global WorldMap_TilesC
WorldMap_TilesC:
	.incbin "GRAPHICS/WORLD_MAP/TILES_C.8bpp.lz"
	/* PACKER: left in the packer's buffer by WorldMap_TilesB at +0xbc9 */
	.byte 0x63, 0xd3, 0xb6
