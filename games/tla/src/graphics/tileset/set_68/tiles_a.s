	.section .rodata
	.global Tileset_Set68TilesA
Tileset_Set68TilesA:
	.incbin "games/tla/src/graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0x0d, 0x09	@ PACKER: left in the buffer by Field_Map144+0x1e3a
