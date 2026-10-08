	.section .rodata
	.global Tileset_Set13Palette
Tileset_Set13Palette:
	.incbin "graphics/tileset/set_13/palette.gbapal.lz"
	.byte 0x27, 0x50	@ PACKER: left in the buffer by Field_Map015+0x10a
