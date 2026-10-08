	.section .rodata
	.global Runtime_ByteRemapTable
Runtime_ByteRemapTable:
	.incbin "FIELD/COMMON/REMAP0.table"
	.incbin "FIELD/COMMON/REMAP1.table"
	.incbin "FIELD/COMMON/REMAP2.table"
	.incbin "FIELD/COMMON/REMAP3.table"
