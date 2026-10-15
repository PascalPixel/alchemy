	.section .rodata
	.global Tileset_Set41Palette
Tileset_Set41Palette:
	.incbin "games/tla/src/graphics/tileset/set_41/palette.gbapal.lz"
	.byte 0xe1	@ PACKER: left in the buffer by Field_Map087+0xbb
