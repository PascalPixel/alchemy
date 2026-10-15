	.section .rodata
	.global Tileset_Set56Palette
Tileset_Set56Palette:
	.incbin "games/tla/src/graphics/tileset/set_56/palette.gbapal.lz"
	.byte 0x11, 0x04	@ PACKER: left in the buffer by Field_Map113+0xfa
