	.section .rodata
	.global Runtime_ByteRemapTable
Runtime_ByteRemapTable:
	.incbin "field/remap0.table"
	.incbin "field/remap1.table"
	.incbin "field/remap2.table"
	.incbin "field/remap3.table"
