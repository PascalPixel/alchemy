	.section .rodata
	.global Tileset_Set06Palette
Tileset_Set06Palette:
	.incbin "games/tla/src/graphics/tileset/set_06/palette.gbapal.lz"
	.byte 0x81, 0xc2	@ PACKER: left in the buffer by Field_Map008+0x196
