	.section .rodata
	.global Field_DefaultMetatileAttributes
Field_DefaultMetatileAttributes:
	.incbin "MAP/DEFAULT/ATTRIBUTES.table.lz"
	.byte 0xfa, 0x60	@ PACKER: left in the buffer by Field_DefaultMapCells+0x48e
