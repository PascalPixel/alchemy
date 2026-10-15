	.section .rodata
	.global Tileset_Set98Palette
Tileset_Set98Palette:
	.incbin "games/tla/src/graphics/tileset/set_98/palette.gbapal.lz"
	.byte 0xf7, 0x99, 0x1a	@ PACKER: left in the buffer by Field_Map226+0x1d9
