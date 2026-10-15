	.section .rodata
	.global Tileset_Palette52
Tileset_Palette52:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_52.gbapal.lz"
	.byte 0xce, 0xe0, 0x07	@ PACKER: left in the buffer by Tileset_Set98TilesD+0x1e5
