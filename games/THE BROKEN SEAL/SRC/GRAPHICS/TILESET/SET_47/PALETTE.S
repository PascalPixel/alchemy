	.section .rodata
	.global Tileset_Set47Palette
Tileset_Set47Palette:
	.incbin "GRAPHICS/TILESET/SET_47/PALETTE.gbapal.lz"
	.byte 0x98, 0xc9	@ PACKER: left in the buffer by Field_Map073+0x192
