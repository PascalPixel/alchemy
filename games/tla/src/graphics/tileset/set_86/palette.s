	.section .rodata
	.global Tileset_Set86Palette
Tileset_Set86Palette:
	.incbin "games/tla/src/graphics/tileset/set_86/palette.gbapal.lz"
	.byte 0xe6	@ PACKER: left in the buffer by Field_Map199+0x16b
