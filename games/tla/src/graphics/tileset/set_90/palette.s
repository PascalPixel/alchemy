	.section .rodata
	.global Tileset_Set90Palette
Tileset_Set90Palette:
	.incbin "games/tla/src/graphics/tileset/set_90/palette.gbapal.lz"
	.byte 0xc0, 0x39	@ PACKER: left in the buffer by Field_Map205+0x166
