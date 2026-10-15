	.section .rodata
	.global Tileset_Palette19
Tileset_Palette19:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_19.gbapal.lz"
	.byte 0xcf, 0x39, 0x99	@ PACKER: left in the buffer by Field_Map114+0xc5
