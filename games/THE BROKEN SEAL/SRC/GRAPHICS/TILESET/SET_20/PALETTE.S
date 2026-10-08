	.section .rodata
	.global Tileset_Set20Palette
Tileset_Set20Palette:
	.incbin "GRAPHICS/TILESET/SET_20/PALETTE.gbapal.lz"
	.byte 0xe2, 0x17	@ PACKER: left in the buffer by Field_Map025+0x19a
