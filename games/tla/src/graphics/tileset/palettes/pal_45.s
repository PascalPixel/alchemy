	.section .rodata
	.global Tileset_Palette45
Tileset_Palette45:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_45.gbapal.lz"
	.byte 0x38, 0x10	@ PACKER: left in the buffer by Field_Map203+0x14e
