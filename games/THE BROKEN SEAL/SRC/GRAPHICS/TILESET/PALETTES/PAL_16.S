	.section .rodata
	.global Tileset_Palette16
Tileset_Palette16:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_16.gbapal.lz"
	.byte 0xc7, 0x0c, 0xb4	@ PACKER: left in the buffer by Field_Map111+0x179
