	.section .rodata
	.global Tileset_Set79Palette
Tileset_Set79Palette:
	.incbin "games/tla/src/graphics/tileset/set_79/palette.gbapal.lz"
	.byte 0x78	@ PACKER: left in the buffer by Field_Map174+0x15f
