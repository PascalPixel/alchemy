	.section .rodata
	.global Tileset_Set21Palette
Tileset_Set21Palette:
	.incbin "games/tla/src/graphics/tileset/set_21/palette.gbapal.lz"
	.byte 0x3a, 0x16, 0xda	@ PACKER: left in the buffer by Field_Map038+0x105
