	.section .rodata
	.global Field_DefaultMapAnimation
Field_DefaultMapAnimation:
	.incbin "map/default/animation.table.lz"
	.byte 0x6b, 0x4e, 0xad	@ PACKER: left in the buffer by Field_DefaultMapCells+0x151
