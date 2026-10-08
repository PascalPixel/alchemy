	.section .rodata
	.global Tileset_Set60Palette
Tileset_Set60Palette:
	.incbin "graphics/tileset/set_60/palette.gbapal.lz"
	.byte 0x31, 0x9d, 0x0f	@ PACKER: left in the buffer by Field_Map127+0x14d
