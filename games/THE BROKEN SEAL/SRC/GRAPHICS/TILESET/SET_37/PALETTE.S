	.section .rodata
	.global Tileset_Set37Palette
Tileset_Set37Palette:
	.incbin "GRAPHICS/TILESET/SET_37/PALETTE.gbapal.lz"
	.byte 0x9b, 0x46	@ PACKER: left in the buffer by Field_Map047+0x16a
