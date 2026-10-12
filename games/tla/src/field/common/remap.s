	.section .rodata
@ バイト置換表 256 個ずつ。前の四つは The Broken Seal の Runtime_ByteRemapTable と同じ
	.global Runtime_ByteRemapTable
Runtime_ByteRemapTable:
	.incbin "common/field/common/remap0.table"
	.global Data_080209a8
Data_080209a8:
	.incbin "common/field/common/remap1.table"
	.incbin "common/field/common/remap2.table"
	.incbin "common/field/common/remap3.table"
	.incbin "field/common/remap4.table"
