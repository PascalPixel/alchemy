	.section .rodata
	.global Tileset_Set25Palette
Tileset_Set25Palette:
	.incbin "games/tla/src/graphics/tileset/set_25/palette.gbapal.lz"
	.byte 0x08	@ PACKER: left in the buffer by Field_Map042+0x10b
