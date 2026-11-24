@ Why assembly: the Thumb entry of the grid outliner, copied to RAM with it:
@ callers enter in Thumb and bx pc switches to the ARM code of Grid_DrawOutlineArm
@ (grid_outline.c) that follows, a mode switch inside a function. The halfword
@ after it is never run.
.syntax unified
	.thumb
	.global Grid_DrawOutline
	.thumb_func
Grid_DrawOutline:
	.global Grid_DrawOutlineCode
Grid_DrawOutlineCode:
	bx	pc
AlchemyUncredited_08021826:
	.2byte	0x0200
AlchemyUncreditedEnd_08021826:
