	.section .rodata
	.global Tileset_Set46Palette
Tileset_Set46Palette:
	.incbin "games/tla/src/graphics/tileset/set_46/palette.gbapal.lz"
	.byte 0x29, 0xc7	@ PACKER: left in the buffer by Field_Map095+0x132
