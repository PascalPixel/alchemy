	.section .rodata
	.global Tileset_Set80Palette
Tileset_Set80Palette:
	.incbin "games/tla/src/graphics/tileset/set_80/palette.gbapal.lz"
	.byte 0x04, 0x08, 0xf0	@ PACKER: left in the buffer by Field_Map179+0xf1
