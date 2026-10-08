	.section .rodata
	.global Tileset_Set55Palette
Tileset_Set55Palette:
	.incbin "graphics/tileset/set_55/palette.gbapal.lz"
	.byte 0x05	@ PACKER: left in the buffer by Field_Map102+0x1bf
