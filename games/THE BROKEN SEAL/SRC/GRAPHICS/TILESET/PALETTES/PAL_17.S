	.section .rodata
	.global Tileset_Palette17
Tileset_Palette17:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_17.gbapal.lz"
	.byte 0x53	@ PACKER: left in the buffer by Field_Map112+0x12b
