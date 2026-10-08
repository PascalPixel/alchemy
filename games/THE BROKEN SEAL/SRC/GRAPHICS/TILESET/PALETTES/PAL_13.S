	.section .rodata
	.global Tileset_Palette13
Tileset_Palette13:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_13.gbapal.lz"
	.byte 0x94, 0x4b, 0xc1	@ PACKER: left in the buffer by Field_Map107+0x191
