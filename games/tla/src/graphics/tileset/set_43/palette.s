	.section .rodata
	.global Tileset_Set43Palette
Tileset_Set43Palette:
	.incbin "games/tla/src/graphics/tileset/set_43/palette.gbapal.lz"
	.byte 0x0d	@ PACKER: left in the buffer by Field_Map091+0x147
