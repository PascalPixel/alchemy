	.section .rodata
	.global Tileset_Set65Palette
Tileset_Set65Palette:
	.incbin "games/tla/src/graphics/tileset/set_65/palette.gbapal.lz"
	.byte 0x27, 0x95	@ PACKER: left in the buffer by Field_Map134+0x10a
