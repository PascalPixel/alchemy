	.section .rodata
	.global Tileset_Palette23
Tileset_Palette23:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_23.gbapal.lz"
	.byte 0xe0, 0xe4	@ PACKER: left in the buffer by Field_Map128+0x18e
