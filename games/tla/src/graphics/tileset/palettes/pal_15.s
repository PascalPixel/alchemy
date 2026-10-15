	.section .rodata
	.global Tileset_Palette15
Tileset_Palette15:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_15.gbapal.lz"
	.byte 0x08	@ PACKER: left in the buffer by Field_Map109+0xe3
