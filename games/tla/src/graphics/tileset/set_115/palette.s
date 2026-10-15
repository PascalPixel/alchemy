	.section .rodata
	.global Tileset_Set115Palette
Tileset_Set115Palette:
	.incbin "games/tla/src/graphics/tileset/set_115/palette.gbapal.lz"
	.byte 0x83, 0xf0	@ PACKER: left in the buffer by Field_Map280+0x152
