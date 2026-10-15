	.section .rodata
	.global Tileset_Set110Palette
Tileset_Set110Palette:
	.incbin "games/tla/src/graphics/tileset/set_110/palette.gbapal.lz"
	.byte 0xc7, 0xba	@ PACKER: left in the buffer by Field_Map263+0x16e
