	.section .rodata
	.global Tileset_Set27Palette
Tileset_Set27Palette:
	.incbin "GRAPHICS/TILESET/SET_27/PALETTE.gbapal.lz"
	.byte 0x8c, 0xda, 0xd6	@ PACKER: left in the buffer by Field_Map035+0x1a9
