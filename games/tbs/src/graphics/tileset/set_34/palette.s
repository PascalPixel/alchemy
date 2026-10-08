	.section .rodata
	.global Tileset_Set34Palette
Tileset_Set34Palette:
	.incbin "graphics/tileset/set_34/palette.gbapal.lz"
	.byte 0x6a, 0x7b	@ PACKER: left in the buffer by Field_Map044+0x132
