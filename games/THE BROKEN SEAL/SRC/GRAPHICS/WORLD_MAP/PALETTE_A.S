	.section .rodata
	.global WorldMap_PaletteA
WorldMap_PaletteA:
	.incbin "GRAPHICS/WORLD_MAP/PALETTE_A.gbapal.plz"
	.byte 0x11, 0x82, 0x87	@ PACKER: left in the buffer by Field_DefaultMetatileAttributes+0x1f5
