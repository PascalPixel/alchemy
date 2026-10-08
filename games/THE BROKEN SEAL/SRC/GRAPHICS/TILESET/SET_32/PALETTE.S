	.section .rodata
	.global Tileset_Set32Palette
Tileset_Set32Palette:
	.incbin "GRAPHICS/TILESET/SET_32/PALETTE.gbapal.lz"
	.byte 0xb1, 0xc1	@ PACKER: left in the buffer by Field_Map041+0x176
