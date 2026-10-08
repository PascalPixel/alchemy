	.section .rodata
	.global Tileset_Palette00
Tileset_Palette00:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_00.gbapal.lz"
	.byte 0x41, 0x28, 0x40	@ PACKER: left in the buffer by Field_Map016+0x159
