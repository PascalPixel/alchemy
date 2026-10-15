	.section .rodata
	.global Tileset_Set31Palette
Tileset_Set31Palette:
	.incbin "games/tla/src/graphics/tileset/set_31/palette.gbapal.lz"
	.byte 0x2f, 0x43, 0x0e	@ PACKER: left in the buffer by Field_Map050+0x159
