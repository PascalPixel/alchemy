	.section .rodata
	.global Tileset_Palette48
Tileset_Palette48:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_48.gbapal.lz"
	.byte 0x38, 0x16	@ PACKER: left in the buffer by Field_Map160+0x1e6
