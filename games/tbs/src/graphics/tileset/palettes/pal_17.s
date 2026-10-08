	.section .rodata
	.global Tileset_Palette17
Tileset_Palette17:
	.incbin "graphics/tileset/palettes/pal_17.gbapal.lz"
	.byte 0x53	@ PACKER: left in the buffer by Field_Map112+0x12b
