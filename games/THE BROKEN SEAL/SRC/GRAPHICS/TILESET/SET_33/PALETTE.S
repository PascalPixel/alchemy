	.section .rodata
	.global Tileset_Set33Palette
Tileset_Set33Palette:
	.incbin "GRAPHICS/TILESET/SET_33/PALETTE.gbapal.lz"
	.byte 0xcb, 0x81	@ PACKER: left in the buffer by Field_Map042+0x166
