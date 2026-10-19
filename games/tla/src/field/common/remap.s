	.section .rodata
@ 256-entry byte remap tables; the first four match TBS's gRuntime_ByteRemapTable.
	.global gRuntime_ByteRemapTable
gRuntime_ByteRemapTable:
	.incbin "games/tla/src/field/common/remap0.table"
	.global Data_080209a8
Data_080209a8:
	.incbin "games/tla/src/field/common/remap1.table"
	.incbin "games/tla/src/field/common/remap2.table"
	.incbin "games/tla/src/field/common/remap3.table"
	.incbin "games/tla/src/field/common/remap4.table"
