	.section .rodata
@ バイト置換表 256 個ずつ。前の四つは The Broken Seal の Runtime_ByteRemapTable と同じ
	.global Runtime_ByteRemapTable
Runtime_ByteRemapTable:
	.incbin "games/common/src/field/common/remap0.table"
	.global Data_080209a8
Data_080209a8:
	.incbin "games/common/src/field/common/remap1.table"
	.incbin "games/common/src/field/common/remap2.table"
	.incbin "games/common/src/field/common/remap3.table"
	.incbin "games/tla/src/field/common/remap4.table"
