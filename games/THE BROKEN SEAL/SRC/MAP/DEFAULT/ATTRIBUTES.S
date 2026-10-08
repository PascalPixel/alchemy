	.section .rodata
	.global Field_DefaultMetatileAttributes
Field_DefaultMetatileAttributes:
	.incbin "MAP/DEFAULT/ATTRIBUTES.table.lz"
	/* PACKER: left in the packer's buffer by Field_DefaultMapCells at +0x48e */
	.byte 0xfa, 0x60
