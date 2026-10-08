	.section .rodata
	.global Tileset_Set54Palette
Tileset_Set54Palette:
	.incbin "GRAPHICS/TILESET/SET_54/PALETTE.gbapal.lz"
	.byte 0x2a, 0x91, 0x1b	@ PACKER: left in the buffer by Field_Map101+0x1a5
