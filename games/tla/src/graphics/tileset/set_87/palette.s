	.section .rodata
	.global Tileset_Set87Palette
Tileset_Set87Palette:
	.incbin "games/tla/src/graphics/tileset/set_87/palette.gbapal.lz"
	.byte 0x09, 0x12, 0xe0	@ PACKER: left in the buffer by Field_Map201+0x14d
