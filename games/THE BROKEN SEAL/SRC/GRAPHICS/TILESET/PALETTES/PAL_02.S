	.section .rodata
	.global Tileset_Palette02
Tileset_Palette02:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_02.gbapal.lz"
	.byte 0xe3, 0x65	@ PACKER: left in the buffer by Field_Map043+0x166
