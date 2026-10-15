	.section .rodata
	.global Tileset_Set29Palette
Tileset_Set29Palette:
	.incbin "games/tla/src/graphics/tileset/set_29/palette.gbapal.lz"
	.byte 0x70, 0x28	@ PACKER: left in the buffer by Field_Map048+0x16a
