	.section .rodata
	.global Tileset_Set100Palette
Tileset_Set100Palette:
	.incbin "games/tla/src/graphics/tileset/set_100/palette.gbapal.lz"
	.byte 0xfa	@ PACKER: left in the buffer by Field_Map233+0x13f
