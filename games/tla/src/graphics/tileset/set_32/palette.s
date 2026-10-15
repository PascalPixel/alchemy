	.section .rodata
	.global Tileset_Set32Palette
Tileset_Set32Palette:
	.incbin "games/tla/src/graphics/tileset/set_32/palette.gbapal.lz"
	.byte 0x74, 0x78, 0x1e	@ PACKER: left in the buffer by Field_Map051+0xe9
