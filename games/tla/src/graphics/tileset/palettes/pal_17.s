	.section .rodata
	.global Tileset_Palette17
Tileset_Palette17:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_17.gbapal.lz"
	.byte 0x29	@ PACKER: left in the buffer by Field_Map111+0xe3
