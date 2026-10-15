	.section .rodata
	.global Tileset_Palette38
Tileset_Palette38:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_38.gbapal.lz"
	.byte 0x2d, 0x03, 0x3c	@ PACKER: left in the buffer by Field_Map190+0x155
