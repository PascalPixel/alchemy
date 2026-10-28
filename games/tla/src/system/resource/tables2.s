	.section .rodata
@ Second copy of the color conversion and byte remap tables, matching the run
@ from gResourceSlot_ConversionTables through the end of Data_080209a8.
	.global Data_080c89cc
Data_080c89cc:
	.incbin "games/tla/src/system/resource/colconv0.table"
	.incbin "games/tla/src/system/resource/colconv1.table"
	.incbin "games/tla/src/system/resource/colconv2.table"
	.incbin "games/tla/src/system/resource/colconv3.table"
	.incbin "games/tla/src/system/resource/colconv4.table"
	.incbin "games/tla/src/field/shared/remap0.table"
	.incbin "games/tla/src/field/shared/remap1.table"
	.incbin "games/tla/src/field/shared/remap2.table"
	.incbin "games/tla/src/field/shared/remap3.table"
	.incbin "games/tla/src/field/shared/remap4.table"
