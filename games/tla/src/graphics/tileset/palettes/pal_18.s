	.section .rodata
	.global Tileset_Palette18
Tileset_Palette18:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_18.gbapal.lz"
	.byte 0x45	@ PACKER: left in the buffer by Field_Map112+0xff
