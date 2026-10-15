	.section .rodata
	.global Tileset_Set75Palette
Tileset_Set75Palette:
	.incbin "games/tla/src/graphics/tileset/set_75/palette.gbapal.lz"
	.byte 0xc1	@ PACKER: left in the buffer by Field_Map162+0x123
