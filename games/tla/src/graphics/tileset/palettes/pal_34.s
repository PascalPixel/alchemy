	.section .rodata
	.global Tileset_Palette34
Tileset_Palette34:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_34.gbapal.lz"
	.byte 0x58, 0xf9	@ PACKER: left in the buffer by Field_Map187+0x102
