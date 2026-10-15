	.section .rodata
	.global Tileset_Set34Palette
Tileset_Set34Palette:
	.incbin "games/tla/src/graphics/tileset/set_34/palette.gbapal.lz"
	.byte 0xe7, 0x0b	@ PACKER: left in the buffer by Field_Map053+0x186
