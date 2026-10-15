	.section .rodata
	.global Tileset_Palette27
Tileset_Palette27:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_27.gbapal.lz"
	.byte 0x01, 0x2f	@ PACKER: left in the buffer by Field_Map141+0x1ae
