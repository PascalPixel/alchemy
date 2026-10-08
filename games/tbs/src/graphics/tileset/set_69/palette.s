	.section .rodata
	.global Tileset_Set69Palette
Tileset_Set69Palette:
	.incbin "graphics/tileset/set_68/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map173+0xb
