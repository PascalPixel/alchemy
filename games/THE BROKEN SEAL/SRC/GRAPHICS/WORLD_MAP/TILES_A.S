	.section .rodata
	.global WorldMap_TilesA
WorldMap_TilesA:
	.incbin "GRAPHICS/WORLD_MAP/TILES_A.8bpp.lz"
	/* PACKER: left in the packer's buffer by Field_DefaultMapCells at +0x16ba */
	.byte 0x91, 0xb3
