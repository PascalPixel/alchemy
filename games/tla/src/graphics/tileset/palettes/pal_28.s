	.section .rodata
	.global Tileset_Palette28
Tileset_Palette28:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_28.gbapal.lz"
	.byte 0xb3, 0x00, 0x0d	@ PACKER: left in the buffer by Field_Map159+0x125
