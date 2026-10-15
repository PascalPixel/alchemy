	.section .rodata
	.global Tileset_Set59TilesA
Tileset_Set59TilesA:
	.incbin "games/tla/src/graphics/tileset/set_59/tiles_a.4bpp.mtf"
	.byte 0x77, 0x46	@ PACKER: left in the buffer by Field_Map099+0x2ec2
