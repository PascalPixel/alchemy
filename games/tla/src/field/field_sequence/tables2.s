	.section .rodata
@ Second copy of the color conversion and byte remap tables, matching the run
@ from gResourceSlot_ConversionTables through the end of Data_080209a8.
	.global Data_080c89cc
Data_080c89cc:
	.incbin "games/tla/src/field/sprite_color_tables_0.table"
	.incbin "games/tla/src/field/sprite_color_tables_1.table"
	.incbin "games/tla/src/field/sprite_color_tables_2.table"
	.incbin "games/tla/src/field/sprite_color_tables_3.table"
	.incbin "games/tla/src/field/sprite_color_tables_4.table"
	.incbin "games/tla/src/field/remap0.table"
	.incbin "games/tla/src/field/remap1.table"
	.incbin "games/tla/src/field/remap2.table"
	.incbin "games/tla/src/field/remap3.table"
	.incbin "games/tla/src/field/remap4.table"
