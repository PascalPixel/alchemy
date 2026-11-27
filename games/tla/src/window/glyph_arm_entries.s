@ Why assembly: the Thumb entry of the text canvas renderer, copied to RAM
@ with it: callers enter in Thumb and bx pc switches to the ARM code of
@ mnGlyphDrawArm (glyph_arm.c) that follows, a mode switch inside a function.
@ The halfword after it is never run.
.syntax unified
	.thumb
	.global mnGlyphDrawCode
mnGlyphDrawCode:
	bx	pc
AlchemyUncredited080385e2:
	.2byte	0x0200
AlchemyUncreditedEnd080385e2:
