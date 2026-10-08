	.section .rodata
	.global Tileset_Palette28
Tileset_Palette28:
	.incbin "graphics/tileset/palettes/pal_28.gbapal.lz"
	.byte 0x15, 0x30	@ PACKER: left in the buffer by Field_Map123+0x186
