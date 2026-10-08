	.section .rodata
	.global Tileset_Palette52
Tileset_Palette52:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_52.gbapal.lz"
	.byte 0x06	@ PACKER: left in the buffer by Field_Map168+0x103
