@ Why assembly: the Thumb entry of the text canvas renderer, copied to RAM
@ with it: callers enter in Thumb and bx pc switches to the ARM code of
@ Glyph_DrawArm (glyph_arm.c) that follows, a mode switch inside a function.
@ The halfword after it is never run.
.syntax unified
	.thumb
	.global Glyph_DrawCode
Glyph_DrawCode:
	bx	pc
AlchemyUncredited_080385e2:
	.2byte	0x0200
AlchemyUncreditedEnd_080385e2:
