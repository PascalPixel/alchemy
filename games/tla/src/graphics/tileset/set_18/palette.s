	.section .rodata
	.global Tileset_Set18Palette
Tileset_Set18Palette:
	.incbin "games/tla/src/graphics/tileset/set_18/palette.gbapal.lz"
	.byte 0x99, 0xac	@ PACKER: left in the buffer by Field_Map035+0x10a
