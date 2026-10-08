	.section .rodata
	.global Tileset_Palette26
Tileset_Palette26:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_26.gbapal.lz"
	.byte 0x9c, 0x00	@ PACKER: left in the buffer by Field_Map120+0x1de
