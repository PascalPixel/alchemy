	.section .rodata
	.global WorldMap_TilesB
WorldMap_TilesB:
	.incbin "GRAPHICS/WORLD_MAP/TILES_B.8bpp.lz"
	.byte 0x10, 0x08, 0xc4	@ PACKER: left in the buffer by WorldMap_TilesA+0x151d
