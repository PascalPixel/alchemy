@ Why assembly: the Thumb entry of fdWorldMapUpdateAffineLines (world_map_lines.c),
@ copied to RAM with it: callers enter in Thumb and bx pc switches to the ARM
@ code that follows, a mode switch inside a function. The halfword after it is
@ never run.
.syntax unified
	.thumb
	.global fdWorldMapUpdateAffineLinesCode
fdWorldMapUpdateAffineLinesCode:
	bx	pc
AlchemyUncredited0802146e:
	.2byte	0x0200
AlchemyUncreditedEnd0802146e:
