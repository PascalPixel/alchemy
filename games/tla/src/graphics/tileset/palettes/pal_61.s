	.section .rodata
	.global Tileset_Palette61
Tileset_Palette61:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_61.gbapal.lz"
	.byte 0xb4	@ PACKER: left in the buffer by Tileset_Palette60+0x187
