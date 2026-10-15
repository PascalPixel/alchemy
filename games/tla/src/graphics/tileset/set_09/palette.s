	.section .rodata
	.global Tileset_Set09Palette
Tileset_Set09Palette:
	.incbin "games/tla/src/graphics/tileset/set_09/palette.gbapal.lz"
	.byte 0xce, 0xcf	@ PACKER: left in the buffer by Field_Map012+0x1a6
