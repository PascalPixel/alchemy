	.section .rodata
	.global Tileset_Set38Palette
Tileset_Set38Palette:
	.incbin "games/tla/src/graphics/tileset/set_38/palette.gbapal.lz"
	.byte 0x46	@ PACKER: left in the buffer by Field_Map074+0x14f
