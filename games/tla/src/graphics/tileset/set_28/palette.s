	.section .rodata
	.global Tileset_Set28Palette
Tileset_Set28Palette:
	.incbin "games/tla/src/graphics/tileset/set_28/palette.gbapal.lz"
	.byte 0x2d, 0x40, 0xff	@ PACKER: left in the buffer by Field_Map045+0x169
