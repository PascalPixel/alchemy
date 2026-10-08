	.section .rodata
	.global Tileset_Set46Palette
Tileset_Set46Palette:
	.incbin "graphics/tileset/set_46/palette.gbapal.lz"
	.byte 0xcc	@ PACKER: left in the buffer by Field_Map068+0x16b
