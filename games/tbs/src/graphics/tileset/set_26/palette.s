	.section .rodata
	.global Tileset_Set26Palette
Tileset_Set26Palette:
	.incbin "graphics/tileset/set_26/palette.gbapal.lz"
	.byte 0x08	@ PACKER: left in the buffer by Field_Map034+0x183
