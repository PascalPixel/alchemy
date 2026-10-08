	.section .rodata
	.global Tileset_Palette01
Tileset_Palette01:
	.incbin "graphics/tileset/palettes/pal_01.gbapal.lz"
	.byte 0x94, 0x01, 0x06	@ PACKER: left in the buffer by Field_Map040+0x159
