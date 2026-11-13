@ Why assembly: the Thumb entry of Transform_UpdateVertices (world_map_lines.c),
@ copied to RAM with it: callers enter in Thumb and bx pc switches to the ARM
@ code that follows, a mode switch inside a function. The halfword after it is
@ never run.
.syntax unified
	.thumb
	.global Transform_UpdateVerticesCode
Transform_UpdateVerticesCode:
	bx	pc
AlchemyUncredited_0802146e:
	.2byte	0x0200
AlchemyUncreditedEnd_0802146e:
