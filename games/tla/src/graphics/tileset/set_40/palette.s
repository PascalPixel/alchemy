	.section .rodata
	.global Tileset_Set40Palette
Tileset_Set40Palette:
	.incbin "games/tla/src/graphics/tileset/set_40/palette.gbapal.lz"
	.byte 0x94, 0x75	@ PACKER: left in the buffer by Field_Map078+0x14a
