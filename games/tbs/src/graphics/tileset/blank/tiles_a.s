	.section .rodata
	.global Tileset_BlankTilesA
Tileset_BlankTilesA:
	.incbin "graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Map001+0x14e
