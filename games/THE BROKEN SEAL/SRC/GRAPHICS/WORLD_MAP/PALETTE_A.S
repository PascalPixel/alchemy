	.section .rodata
	.global WorldMap_PaletteA
WorldMap_PaletteA:
	.incbin "GRAPHICS/WORLD_MAP/PALETTE_A.gbapal.plz"
	/* PACKER: left in the packer's buffer by Field_DefaultMetatileAttributes at +0x1f5 */
	.byte 0x11, 0x82, 0x87
