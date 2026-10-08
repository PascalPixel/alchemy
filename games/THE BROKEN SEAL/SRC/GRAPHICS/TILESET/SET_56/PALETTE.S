	.section .rodata
	.global Tileset_Set56Palette
Tileset_Set56Palette:
	.incbin "GRAPHICS/TILESET/SET_56/PALETTE.gbapal.lz"
	.byte 0xc5, 0x3e, 0x03	@ PACKER: left in the buffer by Field_Map103+0x1d1
