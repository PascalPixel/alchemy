	.section .rodata
	.global Field_DefaultMapAnimation
Field_DefaultMapAnimation:
	.incbin "MAP/DEFAULT/ANIMATION.table.lz"
	.byte 0x6b, 0x4e, 0xad	@ PACKER: left in the buffer by Field_DefaultMapCells+0x151
