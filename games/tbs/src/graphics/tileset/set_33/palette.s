	.section .rodata
	.global Tileset_Set33Palette
Tileset_Set33Palette:
	.incbin "graphics/tileset/set_33/palette.gbapal.lz"
	.byte 0xcb, 0x81	@ PACKER: left in the buffer by Field_Map042+0x166
