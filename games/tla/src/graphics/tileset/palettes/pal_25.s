	.section .rodata
	.global Tileset_Palette25
Tileset_Palette25:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_25.gbapal.lz"
	.byte 0xf6, 0x1c, 0xf7	@ PACKER: left in the buffer by Field_Map132+0x199
