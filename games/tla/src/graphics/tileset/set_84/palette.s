	.section .rodata
	.global Tileset_Set84Palette
Tileset_Set84Palette:
	.incbin "games/tla/src/graphics/tileset/set_84/palette.gbapal.lz"
	.byte 0x9d, 0xf8	@ PACKER: left in the buffer by Field_Map192+0x1d2
