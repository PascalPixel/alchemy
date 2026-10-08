	.section .rodata
	.global Tileset_Set26Palette
Tileset_Set26Palette:
	.incbin "GRAPHICS/TILESET/SET_26/PALETTE.gbapal.lz"
	.byte 0x08	@ PACKER: left in the buffer by Field_Map034+0x183
