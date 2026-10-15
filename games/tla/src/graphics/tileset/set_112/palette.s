	.section .rodata
	.global Tileset_Set112Palette
Tileset_Set112Palette:
	.incbin "games/tla/src/graphics/tileset/set_112/palette.gbapal.lz"
	.byte 0x25	@ PACKER: left in the buffer by Field_Map277+0x117
