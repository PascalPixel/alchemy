	.section .rodata
	.global Tileset_Set72Palette
Tileset_Set72Palette:
	.incbin "graphics/tileset/set_68/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map176+0xb
