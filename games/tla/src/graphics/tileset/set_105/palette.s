	.section .rodata
	.global Tileset_Set105Palette
Tileset_Set105Palette:
	.incbin "games/tla/src/graphics/tileset/set_105/palette.gbapal.lz"
	.byte 0xa3	@ PACKER: left in the buffer by Field_Map251+0x11f
