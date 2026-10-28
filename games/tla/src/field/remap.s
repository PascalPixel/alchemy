	.section .rodata
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
