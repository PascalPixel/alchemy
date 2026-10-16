	.section .rodata
@ Second copy of the color conversion and byte remap tables, matching the sequence
@ from gResourceSlot_ConversionTables through the end of Data_080209a8.
	.global Data_080c89cc
Data_080c89cc:
	.incbin "games/common/src/system/resource/colconv0.table"
	.incbin "games/common/src/system/resource/colconv1.table"
	.incbin "games/common/src/system/resource/colconv2.table"
	.incbin "games/common/src/system/resource/colconv3.table"
	.incbin "games/common/src/system/resource/colconv4.table"
	.incbin "games/common/src/field/common/remap0.table"
	.incbin "games/common/src/field/common/remap1.table"
	.incbin "games/common/src/field/common/remap2.table"
	.incbin "games/common/src/field/common/remap3.table"
	.incbin "games/tla/src/field/common/remap4.table"
