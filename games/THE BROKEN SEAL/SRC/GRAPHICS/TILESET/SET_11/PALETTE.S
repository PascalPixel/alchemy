	.section .rodata
	.global Tileset_Set11Palette
Tileset_Set11Palette:
	.incbin "GRAPHICS/TILESET/SET_11/PALETTE.gbapal.lz"
	.byte 0x43	@ PACKER: left in the buffer by Field_Map013+0x113
