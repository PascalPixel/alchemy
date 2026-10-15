	.section .rodata
	.global Tileset_Set66TilesA
Tileset_Set66TilesA:
	.incbin "games/tla/src/graphics/tileset/set_66/tiles_a.4bpp.mtf"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
