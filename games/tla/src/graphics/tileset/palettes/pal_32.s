	.section .rodata
	.global Tileset_Palette32
Tileset_Palette32:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_32.gbapal.lz"
	.byte 0x84, 0x03, 0x15	@ PACKER: left in the buffer by Field_Map178+0x13d
