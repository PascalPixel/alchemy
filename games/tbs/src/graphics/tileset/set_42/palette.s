	.section .rodata
	.global Tileset_Set42Palette
Tileset_Set42Palette:
	.incbin "graphics/tileset/set_42/palette.gbapal.lz"
	.byte 0x1f	@ PACKER: left in the buffer by Field_Map052+0x1c7
