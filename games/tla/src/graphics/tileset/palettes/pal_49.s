	.section .rodata
	.global Tileset_Palette49
Tileset_Palette49:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_49.gbapal.lz"
	.byte 0xa7	@ PACKER: left in the buffer by Tileset_Set95TilesD+0x1cb
