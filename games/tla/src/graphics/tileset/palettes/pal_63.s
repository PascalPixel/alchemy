	.section .rodata
	.global Tileset_Palette63
Tileset_Palette63:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_63.gbapal.lz"
	.byte 0x81	@ PACKER: left in the buffer by Field_Map276+0x177
