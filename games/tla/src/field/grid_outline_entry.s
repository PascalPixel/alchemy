@ Why assembly: the Thumb entry of the grid outliner, copied to RAM with it:
@ callers enter in Thumb and bx pc switches to the ARM code of fdGridDrawOutlineArm
@ (grid_outline.c) that follows, a mode switch inside a function. The halfword
@ after it is never run.
.syntax unified
	.thumb
	.global GridDrawOutline
	.thumb_func
GridDrawOutline:
	.global fdGridDrawOutlineCode
fdGridDrawOutlineCode:
	bx	pc
AlchemyUncredited08021826:
	.2byte	0x0200
AlchemyUncreditedEnd08021826:
