	.section .rodata
@ A copy of The Broken Seal's gResourceSlot_ConversionTables
	.global gResourceSlot_ConversionTables
gResourceSlot_ConversionTables:
	.incbin "games/tla/src/field/sprite_color_tables_0.table"
	.incbin "games/tla/src/field/sprite_color_tables_1.table"
	.incbin "games/tla/src/field/sprite_color_tables_2.table"
	.incbin "games/tla/src/field/sprite_color_tables_3.table"
	.incbin "games/tla/src/field/sprite_color_tables_4.table"
@ 256-entry byte remap tables; the first four match The Broken Seal's gSpriteColorRemapTables.
	.global gSpriteColorRemapTables
gSpriteColorRemapTables:
	.incbin "games/tla/src/field/remap0.table"
	.global Data_080209a8
Data_080209a8:
	.incbin "games/tla/src/field/remap1.table"
	.incbin "games/tla/src/field/remap2.table"
	.incbin "games/tla/src/field/remap3.table"
	.incbin "games/tla/src/field/remap4.table"
