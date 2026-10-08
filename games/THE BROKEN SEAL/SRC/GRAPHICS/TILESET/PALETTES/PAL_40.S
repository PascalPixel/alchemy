	.section .rodata
	.global Tileset_Palette40
Tileset_Palette40:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_40.gbapal.lz"
	.byte 0xb5, 0x03	@ PACKER: left in the buffer by Field_Map149+0x172
