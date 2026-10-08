	.section .rodata
	.global Tileset_Set36Palette
Tileset_Set36Palette:
	.incbin "GRAPHICS/TILESET/SET_36/PALETTE.gbapal.lz"
	.byte 0x34	@ PACKER: left in the buffer by Field_Map046+0x18f
