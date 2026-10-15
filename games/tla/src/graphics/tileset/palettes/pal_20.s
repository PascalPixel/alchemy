	.section .rodata
	.global Tileset_Palette20
Tileset_Palette20:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_20.gbapal.lz"
	.byte 0x5c	@ PACKER: left in the buffer by Field_Map116+0xd3
