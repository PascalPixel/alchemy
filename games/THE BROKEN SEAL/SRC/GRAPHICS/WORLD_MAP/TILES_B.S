	.section .rodata
	.global WorldMap_TilesB
WorldMap_TilesB:
	.incbin "GRAPHICS/WORLD_MAP/TILES_B.8bpp.lz"
	/* PACKER: left in the packer's buffer by WorldMap_TilesA at +0x151d */
	.byte 0x10, 0x08, 0xc4
