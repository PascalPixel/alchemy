	.section .rodata
	.global Tileset_Set51Palette
Tileset_Set51Palette:
	.incbin "games/tla/src/graphics/tileset/set_51/palette.gbapal.lz"
	.byte 0x5f, 0x52	@ PACKER: left in the buffer by Field_Map102+0x156
