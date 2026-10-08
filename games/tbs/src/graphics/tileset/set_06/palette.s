	.section .rodata
	.global Tileset_Set06Palette
Tileset_Set06Palette:
	.incbin "graphics/tileset/set_06/palette.gbapal.lz"
	.byte 0x88, 0xd7, 0x3c	@ PACKER: left in the buffer by Field_Map007+0xfd
