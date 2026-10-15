	.section .rodata
	.global Tileset_Set24Palette
Tileset_Set24Palette:
	.incbin "games/tla/src/graphics/tileset/set_24/palette.gbapal.lz"
	.byte 0xc7	@ PACKER: left in the buffer by Field_Map041+0x177
