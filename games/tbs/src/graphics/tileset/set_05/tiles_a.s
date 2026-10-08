	.section .rodata
	.global Tileset_Set05TilesA
Tileset_Set05TilesA:
	.incbin "graphics/tileset/set_05/tiles_a.4bpp.mtf"
	.byte 0x86, 0x47	@ PACKER: left in the buffer by Field_Map006+0x1dae
