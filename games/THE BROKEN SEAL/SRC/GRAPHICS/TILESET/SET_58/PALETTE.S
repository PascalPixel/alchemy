	.section .rodata
	.global Tileset_Set58Palette
Tileset_Set58Palette:
	.incbin "GRAPHICS/TILESET/SET_58/PALETTE.gbapal.lz"
	.byte 0xbe, 0xee	@ PACKER: left in the buffer by Field_Map108+0x186
