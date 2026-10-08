	.section .rodata
	.global Tileset_Set42Palette
Tileset_Set42Palette:
	.incbin "GRAPHICS/TILESET/SET_42/PALETTE.gbapal.lz"
	.byte 0x1f	@ PACKER: left in the buffer by Field_Map052+0x1c7
