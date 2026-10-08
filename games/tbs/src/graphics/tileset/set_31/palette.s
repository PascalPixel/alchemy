	.section .rodata
	.global Tileset_Set31Palette
Tileset_Set31Palette:
	.incbin "graphics/tileset/set_31/palette.gbapal.lz"
	.byte 0x9e, 0x99, 0xb7	@ PACKER: left in the buffer by Field_Map039+0x17d
