	.section .rodata
	.global Tileset_Set06Palette
Tileset_Set06Palette:
	.incbin "GRAPHICS/TILESET/SET_06/PALETTE.gbapal.lz"
	.byte 0x88, 0xd7, 0x3c	@ PACKER: left in the buffer by Field_Map007+0xfd
