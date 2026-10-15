	.section .rodata
	.global Tileset_Set74Palette
Tileset_Set74Palette:
	.incbin "games/tla/src/graphics/tileset/set_74/palette.gbapal.lz"
	.byte 0xcc, 0x0c, 0x26	@ PACKER: left in the buffer by Field_Map161+0x1b1
