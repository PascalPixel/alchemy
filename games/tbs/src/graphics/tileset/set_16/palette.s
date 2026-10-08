	.section .rodata
	.global Tileset_Set16Palette
Tileset_Set16Palette:
	.incbin "graphics/tileset/set_16/palette.gbapal.lz"
	.byte 0x3f	@ PACKER: left in the buffer by Field_Map019+0xf3
