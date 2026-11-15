	.section .rodata
@ Three 8x8 glints that FieldPsynergy_UpdateArrow draws into its 64x64 canvas, the
@ first two in turn; drawn in the basic colour palette and the ramp from
@ blue to white that FieldPsynergy_Arrow writes to OBJ colours 0xe0 to 0xef
	.global gArrow_GlintPixels
gArrow_GlintPixels:
	.incbin "games/tla/src/field/glint.8bpp"
