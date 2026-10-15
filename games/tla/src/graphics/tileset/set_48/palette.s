	.section .rodata
	.global Tileset_Set48Palette
Tileset_Set48Palette:
	.incbin "games/tla/src/graphics/tileset/set_48/palette.gbapal.lz"
	.byte 0xe3, 0xe0, 0x4b	@ PACKER: left in the buffer by Field_Map097+0x121
