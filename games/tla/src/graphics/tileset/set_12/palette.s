	.section .rodata
	.global Tileset_Set12Palette
Tileset_Set12Palette:
	.incbin "games/tla/src/graphics/tileset/set_12/palette.gbapal.lz"
	.byte 0x8a, 0x10	@ PACKER: left in the buffer by Field_Map021+0x1ae
