	.section .rodata
	.global Tileset_Set67Palette
Tileset_Set67Palette:
	.incbin "games/tla/src/graphics/tileset/set_67/palette.gbapal.lz"
	.byte 0x78, 0x9f	@ PACKER: left in the buffer by Field_Map144+0x156
