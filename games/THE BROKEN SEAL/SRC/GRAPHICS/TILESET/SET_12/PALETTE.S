	.section .rodata
	.global Tileset_Set12Palette
Tileset_Set12Palette:
	.incbin "GRAPHICS/TILESET/SET_12/PALETTE.gbapal.lz"
	.byte 0xb1	@ PACKER: left in the buffer by Field_Map014+0x127
