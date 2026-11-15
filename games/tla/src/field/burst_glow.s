	.section .rodata
@ An 8x8 dot of 256 colours, shaded from its rim (colour 0xe9) to its centre
@ (0xef), that FieldPsynergy_Burst loads into VRAM for its 32 particles. Its
@ callback FieldPsynergy_UpdateBurst writes OBJ colours 0xe0 to 0xff, computed from the
@ sine table, each frame; the picture carries the shared OBJ palette and
@ those colours of the first frame.
	.global gBurst_GlowTiles
gBurst_GlowTiles:
	.incbin "games/tla/src/field/glow_spark.8bpp"
