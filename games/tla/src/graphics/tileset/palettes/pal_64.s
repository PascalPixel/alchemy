	.section .rodata
	.global Tileset_Palette64
Tileset_Palette64:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_64.gbapal.lz"
	.byte 0x00, 0x81	@ PACKER: left in the buffer by Tileset_Palette63+0x176
