	.section .rodata
	.global Tileset_Set81Palette
Tileset_Set81Palette:
	.incbin "games/tla/src/graphics/tileset/set_81/palette.gbapal.lz"
	.byte 0x48, 0x1d, 0x3f	@ PACKER: left in the buffer by Field_Map180+0x149
