	.section .rodata
	.global Tileset_Set45TilesA
Tileset_Set45TilesA:
	.incbin "graphics/tileset/set_45/tiles_a.4bpp.mtf"
	.byte 0x13, 0x00	@ PACKER: left in the buffer by Field_Map060+0x24ee
