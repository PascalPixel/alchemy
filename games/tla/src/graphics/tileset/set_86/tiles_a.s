	.section .rodata
	.global Tileset_Set86TilesA
Tileset_Set86TilesA:
	.incbin "games/tla/src/graphics/tileset/set_86/tiles_a.4bpp.mtf"
	.byte 0xf8, 0x61	@ PACKER: left in the buffer by Field_Map198+0xfbe
