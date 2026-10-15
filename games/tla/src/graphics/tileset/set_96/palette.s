	.section .rodata
	.global Tileset_Set96Palette
Tileset_Set96Palette:
	.incbin "games/tla/src/graphics/tileset/set_96/palette.gbapal.lz"
	.byte 0x92, 0x44, 0x80	@ PACKER: left in the buffer by Field_Map223+0x135
