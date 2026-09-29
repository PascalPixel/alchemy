@ Tileset_Set16Palette: 14 palettes of 16 colours, one row each, packed by the
@ resource packer.
	.section .rodata
	.global Tileset_Set16Palette
Tileset_Set16Palette:
	.incbin "GRAPHICS/TILESET/SET_16/PALETTE.gbapal.lz"
