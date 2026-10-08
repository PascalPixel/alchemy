	.section .rodata
	.global Tileset_Set25Palette
Tileset_Set25Palette:
	.incbin "graphics/tileset/set_25/palette.gbapal.lz"
	.byte 0xf3	@ PACKER: left in the buffer by Field_Map033+0x15f
