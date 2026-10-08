	.section .rodata
	.global Tileset_Palette14
Tileset_Palette14:
	.incbin "graphics/tileset/palettes/pal_14.gbapal.lz"
	.byte 0xf8, 0xe4	@ PACKER: left in the buffer by Field_Map109+0x17e
