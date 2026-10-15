	.section .rodata
	.global Tileset_Palette04
Tileset_Palette04:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_04.gbapal.lz"
	.byte 0x98, 0xc4	@ PACKER: left in the buffer by Field_Map055+0x182
