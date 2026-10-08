	.section .rodata
	.global Tileset_Set74Palette
Tileset_Set74Palette:
	.incbin "graphics/tileset/set_68/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map178+0xb
