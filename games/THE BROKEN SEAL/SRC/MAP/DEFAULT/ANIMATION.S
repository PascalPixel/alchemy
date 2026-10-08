	.section .rodata
	.global Field_DefaultMapAnimation
Field_DefaultMapAnimation:
	.incbin "MAP/DEFAULT/ANIMATION.table.lz"
	/* PACKER: left in the packer's buffer by Field_DefaultMapCells at +0x151 */
	.byte 0x6b, 0x4e, 0xad
