	.section .rodata
	.global Tileset_BlankPalette
Tileset_BlankPalette:
	.incbin "GRAPHICS/TILESET/SET_68/PALETTE.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map001+0xb
