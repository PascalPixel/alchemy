	.section .rodata
	.global Tileset_Set39Palette
Tileset_Set39Palette:
	.incbin "GRAPHICS/TILESET/SET_39/PALETTE.gbapal.lz"
	.byte 0xdc, 0x32	@ PACKER: left in the buffer by Field_Map049+0x18e
