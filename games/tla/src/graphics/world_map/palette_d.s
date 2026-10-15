	.section .rodata
	.global WorldMap_PaletteD
WorldMap_PaletteD:
	.incbin "games/tla/src/graphics/world_map/palette_d.gbapal.plz"
	.byte 0x40, 0x92	@ PACKER: left in the buffer by WorldMap_TilesC6+0x1da
