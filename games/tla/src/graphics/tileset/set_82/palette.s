	.section .rodata
	.global Tileset_Set82Palette
Tileset_Set82Palette:
	.incbin "games/tla/src/graphics/tileset/set_82/palette.gbapal.lz"
	.byte 0x64, 0x05	@ PACKER: left in the buffer by Field_Map189+0x1a6
