	.section .rodata
	.global Tileset_Palette05
Tileset_Palette05:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_05.gbapal.lz"
	.byte 0x5d, 0x02, 0x06	@ PACKER: left in the buffer by Field_Map077+0x1b1
