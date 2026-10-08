	.section .rodata
	.global Tileset_Palette27
Tileset_Palette27:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_27.gbapal.lz"
	.byte 0x86, 0xf1	@ PACKER: left in the buffer by Field_Map122+0x18e
