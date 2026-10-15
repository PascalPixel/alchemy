	.section .rodata
	.global Tileset_Set05Palette
Tileset_Set05Palette:
	.incbin "games/tla/src/graphics/tileset/set_05/palette.gbapal.lz"
	.byte 0xaa, 0x2e	@ PACKER: left in the buffer by Field_Map007+0xfe
