	.section .rodata
	.global Tileset_Palette50
Tileset_Palette50:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_50.gbapal.lz"
	.byte 0x0e	@ PACKER: left in the buffer by Field_Map222+0x137
