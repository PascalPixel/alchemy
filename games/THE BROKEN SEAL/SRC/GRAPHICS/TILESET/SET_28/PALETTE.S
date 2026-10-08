	.section .rodata
	.global Tileset_Set28Palette
Tileset_Set28Palette:
	.incbin "GRAPHICS/TILESET/SET_28/PALETTE.gbapal.lz"
	.byte 0x40, 0x15, 0x08	@ PACKER: left in the buffer by Field_Map036+0x1a1
