	.section .rodata
	.global Tileset_Set102Palette
Tileset_Set102Palette:
	.incbin "games/tla/src/graphics/tileset/set_102/palette.gbapal.lz"
	.byte 0xd4, 0x99, 0xc0	@ PACKER: left in the buffer by Field_Map236+0x17d
