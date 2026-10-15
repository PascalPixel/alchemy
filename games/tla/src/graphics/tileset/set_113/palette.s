	.section .rodata
	.global Tileset_Set113Palette
Tileset_Set113Palette:
	.incbin "games/tla/src/graphics/tileset/set_113/palette.gbapal.lz"
	.byte 0x9e, 0x4d	@ PACKER: left in the buffer by Field_Map278+0x142
