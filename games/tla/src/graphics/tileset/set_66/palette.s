	.section .rodata
	.global Tileset_Set66Palette
Tileset_Set66Palette:
	.incbin "games/tla/src/graphics/tileset/set_66/palette.gbapal.lz"
	.byte 0xe1, 0x40, 0x88	@ PACKER: left in the buffer by Field_Map135+0x16d
