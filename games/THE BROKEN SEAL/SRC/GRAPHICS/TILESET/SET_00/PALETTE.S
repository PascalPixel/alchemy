	.section .rodata
	.global Tileset_Set00Palette
Tileset_Set00Palette:
	.incbin "GRAPHICS/TILESET/SET_00/PALETTE.gbapal.lz"
	.byte 0x3f, 0x55, 0x78	@ PACKER: left in the buffer by Field_Map000+0x18d
