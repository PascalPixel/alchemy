	.section .rodata
	.global Tileset_Palette12
Tileset_Palette12:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_12.gbapal.lz"
	.byte 0x19, 0xe9, 0x78	@ PACKER: left in the buffer by Field_Map106+0x1a5
