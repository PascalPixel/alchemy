	.section .rodata
@ The 16 colours FieldPsynergy_UpdateBurst writes to OBJ palettes 14 and 15.
	.global gBurst_SparkPalette
gBurst_SparkPalette:
	.incbin "games/tla/src/field/fxpal.gbapal"
