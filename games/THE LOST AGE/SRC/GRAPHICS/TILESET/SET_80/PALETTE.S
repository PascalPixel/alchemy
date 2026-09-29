@ Tileset_Set80Palette: 14 palettes of 16 colours, one row each, packed by the
@ resource packer.
	.section .rodata
	.global Tileset_Set80Palette
Tileset_Set80Palette:
	.incbin "GRAPHICS/TILESET/SET_80/PALETTE.gbapal.lz"
