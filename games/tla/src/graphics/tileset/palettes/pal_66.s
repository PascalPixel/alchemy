	.section .rodata
	.global Tileset_Palette66
Tileset_Palette66:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_66.gbapal.lz"
	.byte 0x17	@ PACKER: left in the buffer by Tileset_Palette65+0x18f
