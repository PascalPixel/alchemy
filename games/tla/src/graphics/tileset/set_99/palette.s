	.section .rodata
	.global Tileset_Set99Palette
Tileset_Set99Palette:
	.incbin "games/tla/src/graphics/tileset/set_99/palette.gbapal.lz"
	.byte 0x06	@ PACKER: left in the buffer by Field_Map227+0x193
