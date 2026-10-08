	.section .rodata
	.global Tileset_Palette49
Tileset_Palette49:
	.incbin "COMMON/GRAPHICS/TILESET/PALETTES/PAL_49.gbapal.lz"
	.byte 0x1e, 0x33	@ PACKER: left in the buffer by Field_VinasuHeya+0x106
