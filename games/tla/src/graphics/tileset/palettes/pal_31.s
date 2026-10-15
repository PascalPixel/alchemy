	.section .rodata
	.global Tileset_Palette31
Tileset_Palette31:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_31.gbapal.lz"
	.byte 0x30	@ PACKER: left in the buffer by Field_Map173+0x193
