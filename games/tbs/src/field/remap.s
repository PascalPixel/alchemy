	.section .rodata
	.global Runtime_ByteRemapTable
Runtime_ByteRemapTable:
	.incbin "games/tbs/src/field/remap0.table"
	.incbin "games/tbs/src/field/remap1.table"
	.incbin "games/tbs/src/field/remap2.table"
	.incbin "games/tbs/src/field/remap3.table"
