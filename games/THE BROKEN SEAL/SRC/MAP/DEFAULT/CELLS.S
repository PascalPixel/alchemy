@ Field_DefaultMapCells: the default field map's cells, packed by the
@ resource packer.
	.section .rodata
	.global Field_DefaultMapCells
Field_DefaultMapCells:
	.incbin "MAP/DEFAULT/CELLS.table.lz"
