	.section .rodata
	.global Tileset_Set08Palette
Tileset_Set08Palette:
	.incbin "games/tla/src/graphics/tileset/set_08/palette.gbapal.lz"
	.byte 0x80	@ PACKER: left in the buffer by Field_Map011+0x197
