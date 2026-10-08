	.section .rodata
	.global Tileset_Palette09
Tileset_Palette09:
	.incbin "graphics/tileset/palettes/pal_09.gbapal.lz"
	.byte 0xcc	@ PACKER: left in the buffer by Field_Map099+0x1c3
