	.section .rodata
	.global Tileset_Set69Palette
Tileset_Set69Palette:
	.incbin "games/tla/src/graphics/tileset/set_69/palette.gbapal.lz"
	.byte 0x17, 0x03, 0xcc	@ PACKER: left in the buffer by Field_Map146+0x1c5
