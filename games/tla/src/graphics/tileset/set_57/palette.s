	.section .rodata
	.global Tileset_Set57Palette
Tileset_Set57Palette:
	.incbin "games/tla/src/graphics/tileset/set_57/palette.gbapal.lz"
	.byte 0xf0, 0x0c, 0xa6	@ PACKER: left in the buffer by Field_Map115+0xc1
