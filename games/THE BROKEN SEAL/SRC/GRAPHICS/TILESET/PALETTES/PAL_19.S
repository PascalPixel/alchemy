	.section .rodata
	.global Tileset_Palette19
Tileset_Palette19:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_19.gbapal.lz"
	.byte 0xb3, 0x90, 0x80	@ PACKER: left in the buffer by Field_Map113+0x185
