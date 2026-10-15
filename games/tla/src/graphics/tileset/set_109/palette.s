	.section .rodata
	.global Tileset_Set109Palette
Tileset_Set109Palette:
	.incbin "games/tla/src/graphics/tileset/set_109/palette.gbapal.lz"
	.byte 0x14, 0x8f	@ PACKER: left in the buffer by Field_Map259+0x19a
