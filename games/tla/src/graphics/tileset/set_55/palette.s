	.section .rodata
	.global Tileset_Set55Palette
Tileset_Set55Palette:
	.incbin "games/tla/src/graphics/tileset/set_55/palette.gbapal.lz"
	.byte 0x9f	@ PACKER: left in the buffer by Field_Map107+0xe3
