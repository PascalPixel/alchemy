	.section .rodata
	.global Tileset_Palette65
Tileset_Palette65:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_65.gbapal.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
