	.section .rodata
	.global Resource_Data61A
Resource_Data61A:
	.incbin "games/tla/src/graphics/tileset/blank/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map292+0xb
