	.section .rodata
	.global Tileset_Set107Palette
Tileset_Set107Palette:
	.incbin "games/tla/src/graphics/tileset/set_107/palette.gbapal.lz"
	.byte 0x18	@ PACKER: left in the buffer by Field_Map254+0x15f
