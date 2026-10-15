	.section .rodata
	.global Tileset_Palette03
Tileset_Palette03:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_03.gbapal.lz"
	.byte 0x0e	@ PACKER: left in the buffer by Field_Map054+0x173
