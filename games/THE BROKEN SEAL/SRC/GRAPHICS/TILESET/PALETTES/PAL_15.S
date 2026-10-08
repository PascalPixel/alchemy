	.section .rodata
	.global Tileset_Palette15
Tileset_Palette15:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_15.gbapal.lz"
	.byte 0x10, 0x2f, 0x79	@ PACKER: left in the buffer by Field_Map110+0x181
