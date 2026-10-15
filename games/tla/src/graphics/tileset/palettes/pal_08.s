	.section .rodata
	.global Tileset_Palette08
Tileset_Palette08:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_08.gbapal.lz"
	.byte 0xf3, 0xff	@ PACKER: left in the buffer by Field_Map059+0x1c2
