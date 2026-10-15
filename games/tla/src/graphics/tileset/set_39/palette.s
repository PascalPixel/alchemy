	.section .rodata
	.global Tileset_Set39Palette
Tileset_Set39Palette:
	.incbin "games/tla/src/graphics/tileset/set_39/palette.gbapal.lz"
	.byte 0x98, 0xb7, 0xe6	@ PACKER: left in the buffer by Field_Map075+0x191
