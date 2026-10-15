	.section .rodata
	.global Tileset_Set63Palette
Tileset_Set63Palette:
	.incbin "games/tla/src/graphics/tileset/set_63/palette.gbapal.lz"
	.byte 0x04	@ PACKER: left in the buffer by Field_Map131+0x1db
