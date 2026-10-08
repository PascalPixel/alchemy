	.section .rodata
	.global Tileset_Palette30
Tileset_Palette30:
	.incbin "graphics/tileset/palettes/pal_30.gbapal.lz"
	.byte 0x7f, 0x45, 0xd8	@ PACKER: left in the buffer by Field_Map126+0x18d
