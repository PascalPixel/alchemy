	.section .rodata
	.global Tileset_Set62Palette
Tileset_Set62Palette:
	.incbin "games/tla/src/graphics/tileset/set_62/palette.gbapal.lz"
	.byte 0x80, 0x0e	@ PACKER: left in the buffer by Field_Map129+0x13a
