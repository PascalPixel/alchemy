	.section .rodata
	.global Tileset_Palette48
Tileset_Palette48:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_48.gbapal.lz"
	.byte 0x19	@ PACKER: left in the buffer by Field_Map217+0x167
