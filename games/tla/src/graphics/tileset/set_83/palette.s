	.section .rodata
	.global Tileset_Set83Palette
Tileset_Set83Palette:
	.incbin "games/tla/src/graphics/tileset/set_83/palette.gbapal.lz"
	.byte 0xa4, 0x02, 0x52	@ PACKER: left in the buffer by Field_Map191+0x1c9
