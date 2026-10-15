	.section .rodata
	.global Tileset_Set54Palette
Tileset_Set54Palette:
	.incbin "games/tla/src/graphics/tileset/set_54/palette.gbapal.lz"
	.byte 0x3c, 0x78, 0xf0	@ PACKER: left in the buffer by Field_Map105+0x165
