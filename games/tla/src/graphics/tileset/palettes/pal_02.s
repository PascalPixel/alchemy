	.section .rodata
	.global Tileset_Palette02
Tileset_Palette02:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_02.gbapal.lz"
	.byte 0xe3, 0x56	@ PACKER: left in the buffer by Field_Map047+0x176
