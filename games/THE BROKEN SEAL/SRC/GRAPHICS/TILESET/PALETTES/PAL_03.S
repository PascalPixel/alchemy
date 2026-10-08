	.section .rodata
	.global Tileset_Palette03
Tileset_Palette03:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_03.gbapal.lz"
	.byte 0x7e, 0x5e	@ PACKER: left in the buffer by Field_Map062+0x156
