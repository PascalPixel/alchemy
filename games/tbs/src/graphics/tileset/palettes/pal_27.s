	.section .rodata
	.global Tileset_Palette27
Tileset_Palette27:
	.incbin "graphics/tileset/palettes/pal_27.gbapal.lz"
	.byte 0x86, 0xf1	@ PACKER: left in the buffer by Field_Map122+0x18e
