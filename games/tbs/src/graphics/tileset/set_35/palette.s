	.section .rodata
	.global Tileset_Set35Palette
Tileset_Set35Palette:
	.incbin "graphics/tileset/set_35/palette.gbapal.lz"
	.byte 0x05	@ PACKER: left in the buffer by Field_Map045+0x17b
