@ Tileset_Set32Palette: 14 palettes of 16 colours, one row each, packed by the
@ resource packer.
	.section .rodata
	.global Tileset_Set32Palette
Tileset_Set32Palette:
	.incbin "GRAPHICS/TILESET/SET_32/PALETTE.gbapal.lz"
