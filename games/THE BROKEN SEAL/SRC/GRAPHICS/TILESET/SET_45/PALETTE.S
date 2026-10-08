	.section .rodata
	.global Tileset_Set45Palette
Tileset_Set45Palette:
	.incbin "GRAPHICS/TILESET/SET_45/PALETTE.gbapal.lz"
	.byte 0xa8, 0x80, 0x60	@ PACKER: left in the buffer by Field_Map061+0x1d9
