	.section .rodata
@ Three 8x8 glints that Func_080e1678 draws into its 64x64 canvas, the
@ first two in turn; drawn in the basic colour palette and the ramp from
@ blue to white that Func_080e1f2c writes to OBJ colours 0xe0 to 0xef
	.global Data_080eda0c
Data_080eda0c:
	.incbin "games/tla/src/battle/effect/glint.8bpp"
