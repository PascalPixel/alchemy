@ Tileset_Set64Palette: 14 palettes of 16 colours, one row each, packed by the
@ resource packer.
	.section .rodata
	.global Tileset_Set64Palette
Tileset_Set64Palette:
	.incbin "GRAPHICS/TILESET/SET_64/PALETTE.gbapal.lz"
