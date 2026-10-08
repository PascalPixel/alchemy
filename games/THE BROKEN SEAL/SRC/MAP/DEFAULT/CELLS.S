	.section .rodata
	.global Field_DefaultMapCells
Field_DefaultMapCells:
	.incbin "MAP/DEFAULT/CELLS.table.lz"
	.byte 0x01	@ PACKER: left in the buffer by Field_PerspectiveDataA+0x39eb
