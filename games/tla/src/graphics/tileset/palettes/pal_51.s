	.section .rodata
	.global Tileset_Palette51
Tileset_Palette51:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_51.gbapal.lz"
	.byte 0x27, 0xef	@ PACKER: left in the buffer by Field_Map225+0x152
