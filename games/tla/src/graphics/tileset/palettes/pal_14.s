	.section .rodata
	.global Tileset_Palette14
Tileset_Palette14:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_14.gbapal.lz"
	.byte 0xb5	@ PACKER: left in the buffer by Field_Map108+0xe3
