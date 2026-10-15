	.section .rodata
	.global Tileset_Palette11
Tileset_Palette11:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_11.gbapal.lz"
	.byte 0x5d	@ PACKER: left in the buffer by Field_Map094+0x19b
