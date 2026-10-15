	.section .rodata
	.global Tileset_Set14Palette
Tileset_Set14Palette:
	.incbin "games/tla/src/graphics/tileset/set_14/palette.gbapal.lz"
	.byte 0x27, 0x1f	@ PACKER: left in the buffer by Field_Map031+0x16e
