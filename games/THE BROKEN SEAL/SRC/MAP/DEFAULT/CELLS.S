	.section .rodata
	.global Field_DefaultMapCells
Field_DefaultMapCells:
	.incbin "MAP/DEFAULT/CELLS.table.lz"
	/* PACKER: left in the packer's buffer by Field_PerspectiveDataA at +0x39eb */
	.byte 0x01
