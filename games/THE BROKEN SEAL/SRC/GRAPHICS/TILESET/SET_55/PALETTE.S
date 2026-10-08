	.section .rodata
	.global Tileset_Set55Palette
Tileset_Set55Palette:
	.incbin "GRAPHICS/TILESET/SET_55/PALETTE.gbapal.lz"
	.byte 0x05	@ PACKER: left in the buffer by Field_Map102+0x1bf
