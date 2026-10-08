	.section .rodata
	.global Tileset_Palette51
Tileset_Palette51:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_51.gbapal.lz"
	.byte 0xf0, 0x40, 0xef	@ PACKER: left in the buffer by Field_Map164+0x159
