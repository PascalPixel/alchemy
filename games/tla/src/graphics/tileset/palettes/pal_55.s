	.section .rodata
	.global Tileset_Palette55
Tileset_Palette55:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_55.gbapal.lz"
	.byte 0x34, 0x61, 0x91	@ PACKER: left in the buffer by Field_Map249+0x13d
