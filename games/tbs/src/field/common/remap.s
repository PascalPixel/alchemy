@ A table of the handwritten routine beside it: it lives in the routine's text.
	.text
	.global Runtime_ByteRemapTable
Runtime_ByteRemapTable:
	.incbin "field/common/remap0.table"
	.incbin "field/common/remap1.table"
	.incbin "field/common/remap2.table"
	.incbin "field/common/remap3.table"
