	.section .rodata
	.global Tileset_Palette18
Tileset_Palette18:
	.incbin "graphics/tileset/palettes/pal_18.gbapal.lz"
	.byte 0x00, 0x00, 0x38	@ PACKER: left in the buffer; the stream that wrote them is unknown
