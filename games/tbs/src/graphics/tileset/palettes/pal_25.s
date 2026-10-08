	.section .rodata
	.global Tileset_Palette25
Tileset_Palette25:
	.incbin "graphics/tileset/palettes/pal_25.gbapal.lz"
	.byte 0x5b, 0x24, 0xd1	@ PACKER: left in the buffer by Field_Map119+0x18d
