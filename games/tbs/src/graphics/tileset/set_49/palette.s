	.section .rodata
	.global Tileset_Set49Palette
Tileset_Set49Palette:
	.incbin "graphics/tileset/set_49/palette.gbapal.lz"
	.byte 0xb3, 0xec, 0xe0	@ PACKER: left in the buffer by Field_Map076+0x1c9
