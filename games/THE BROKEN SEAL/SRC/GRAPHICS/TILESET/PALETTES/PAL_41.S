	.section .rodata
	.global Tileset_Palette41
Tileset_Palette41:
	.incbin "COMMON/GRAPHICS/TILESET/PALETTES/PAL_41.gbapal.lz"
	.byte 0xfc, 0x22, 0x8f	@ PACKER: left in the buffer by Field_Map152+0x171
