	.section .rodata
	.global Field_DefaultMapAnimation
Field_DefaultMapAnimation:
	.incbin "games/tla/src/map/default/animation.table.lz"
	.byte 0x67, 0x57, 0x6f	@ PACKER: left in the buffer by WorldMap_PaletteA+0x151
