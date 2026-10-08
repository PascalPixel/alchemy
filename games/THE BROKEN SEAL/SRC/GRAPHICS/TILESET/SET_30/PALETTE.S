	.section .rodata
	.global Tileset_Set30Palette
Tileset_Set30Palette:
	.incbin "GRAPHICS/TILESET/SET_30/PALETTE.gbapal.lz"
	.byte 0x8e, 0x9e, 0x21	@ PACKER: left in the buffer by Field_Map038+0x1a1
