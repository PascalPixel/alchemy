	.section .rodata
	.global Tileset_Palette24
Tileset_Palette24:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_24.gbapal.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x5c	@ PACKER: left in the buffer by Field_Map118+0x1a7
	.else
	.byte 0xc8	@ PACKER: left in the buffer by Field_Map118+0x1a7
	.endif
