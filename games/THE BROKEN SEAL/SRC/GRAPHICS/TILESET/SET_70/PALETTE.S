	.section .rodata
	.global Tileset_Set70Palette
Tileset_Set70Palette:
	.incbin "GRAPHICS/TILESET/SET_68/PALETTE.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map174+0xb
