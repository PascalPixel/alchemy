	.section .rodata
	.global Tileset_Palette31
Tileset_Palette31:
	.incbin "graphics/tileset/palettes/pal_31.gbapal.lz"
	.byte 0xc8, 0x1f, 0x81	@ PACKER: left in the buffer by Field_Map130+0x159
