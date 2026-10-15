	.section .rodata
	.global Tileset_Set10Palette
Tileset_Set10Palette:
	.incbin "games/tla/src/graphics/tileset/set_10/palette.gbapal.lz"
	.byte 0x19, 0xc6, 0x8f	@ PACKER: left in the buffer by Field_Map019+0x179
