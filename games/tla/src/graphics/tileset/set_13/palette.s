	.section .rodata
	.global Tileset_Set13Palette
Tileset_Set13Palette:
	.incbin "games/tla/src/graphics/tileset/set_13/palette.gbapal.lz"
	.byte 0x85	@ PACKER: left in the buffer by Field_Map030+0x11f
