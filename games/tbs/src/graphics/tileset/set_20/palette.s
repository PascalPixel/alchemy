	.section .rodata
	.global Tileset_Set20Palette
Tileset_Set20Palette:
	.incbin "graphics/tileset/set_20/palette.gbapal.lz"
	.byte 0xe2, 0x17	@ PACKER: left in the buffer by Field_Map025+0x19a
