	.section .rodata
	.global Tileset_Set22Palette
Tileset_Set22Palette:
	.incbin "games/tla/src/graphics/tileset/set_22/palette.gbapal.lz"
	.byte 0x12, 0x05, 0x08	@ PACKER: left in the buffer by Field_Map039+0x14d
