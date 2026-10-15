	.section .rodata
	.global Tileset_Set77Palette
Tileset_Set77Palette:
	.incbin "games/tla/src/graphics/tileset/set_77/palette.gbapal.lz"
	.byte 0x18	@ PACKER: left in the buffer by Field_Map167+0x133
