	.section .rodata
	.global Tileset_Set64Palette
Tileset_Set64Palette:
	.incbin "games/tla/src/graphics/tileset/set_64/palette.gbapal.lz"
	.byte 0x18, 0x8c	@ PACKER: left in the buffer by Field_Map133+0x116
