	.section .rodata
	.global WorldMap_PaletteA
WorldMap_PaletteA:
	.incbin "games/tla/src/graphics/world_map/palette_a.gbapal.plz"
	.byte 0x60, 0xc2, 0x93	@ PACKER: left in the buffer by Field_DefaultMapCells+0x1ed
