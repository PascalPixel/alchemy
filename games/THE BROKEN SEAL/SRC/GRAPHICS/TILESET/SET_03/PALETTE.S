	.section .rodata
	.global Tileset_Set03Palette
Tileset_Set03Palette:
	.incbin "GRAPHICS/TILESET/SET_03/PALETTE.gbapal.lz"
	.byte 0xc1, 0xac, 0xce	@ PACKER: left in the buffer by Field_Map004+0x191
