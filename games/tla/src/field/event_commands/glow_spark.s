	.section .rodata
@ An 8x8 dot of 256 colours, shaded from its rim (colour 0xe9) to its centre
@ (0xef), that Func_080e572c loads into VRAM for its 32 particles. Its
@ callback Func_080e4d9c writes OBJ colours 0xe0 to 0xff, computed from the
@ sine table, each frame; the picture carries the shared OBJ palette and
@ those colours of the first frame.
	.global Data_080f3984
Data_080f3984:
	.incbin "games/tla/src/field/event_commands/glow_spark.8bpp"
