	.section .rodata
	.global Tileset_Set43Palette
Tileset_Set43Palette:
	.incbin "graphics/tileset/set_43/palette.gbapal.lz"
	.byte 0xc0, 0xcf, 0x02	@ PACKER: left in the buffer by Field_Map054+0x16d
