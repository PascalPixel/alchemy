	.section .rodata
	.global Tileset_Palette43
Tileset_Palette43:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_43.gbapal.lz"
	.byte 0xec, 0x5c	@ PACKER: left in the buffer by Field_Map155+0x17e
