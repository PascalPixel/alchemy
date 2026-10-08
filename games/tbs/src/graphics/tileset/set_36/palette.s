	.section .rodata
	.global Tileset_Set36Palette
Tileset_Set36Palette:
	.incbin "graphics/tileset/set_36/palette.gbapal.lz"
	.byte 0x34	@ PACKER: left in the buffer by Field_Map046+0x18f
