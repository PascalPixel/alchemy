	.section .rodata
	.global Tileset_Palette10
Tileset_Palette10:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_10.gbapal.lz"
	.byte 0x10, 0x08	@ PACKER: left in the buffer by Field_Map088+0x11a
