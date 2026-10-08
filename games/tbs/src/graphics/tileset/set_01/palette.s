	.section .rodata
	.global Tileset_Set01Palette
Tileset_Set01Palette:
	.incbin "graphics/tileset/set_01/palette.gbapal.lz"
	.byte 0xb3	@ PACKER: left in the buffer by Field_Map002+0x193
