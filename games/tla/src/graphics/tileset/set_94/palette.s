	.section .rodata
	.global Tileset_Set94Palette
Tileset_Set94Palette:
	.incbin "games/tla/src/graphics/tileset/set_94/palette.gbapal.lz"
	.byte 0xed	@ PACKER: left in the buffer by Field_Map220+0x193
