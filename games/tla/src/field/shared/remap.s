	.section .rodata
@ 256-entry byte remap tables; the first four match The Broken Seal's gSpriteColorRemapTables.
	.global gSpriteColorRemapTables
gSpriteColorRemapTables:
	.incbin "games/tla/src/field/shared/remap0.table"
	.global Data_080209a8
Data_080209a8:
	.incbin "games/tla/src/field/shared/remap1.table"
	.incbin "games/tla/src/field/shared/remap2.table"
	.incbin "games/tla/src/field/shared/remap3.table"
	.incbin "games/tla/src/field/shared/remap4.table"
