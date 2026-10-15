	.section .rodata
	.global Tileset_Set114Palette
Tileset_Set114Palette:
	.incbin "games/tla/src/graphics/tileset/set_114/palette.gbapal.lz"
	.byte 0x17, 0x3c	@ PACKER: left in the buffer by Field_Map279+0x152
