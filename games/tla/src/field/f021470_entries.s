@ Why assembly: the Thumb entry of the routine in f021470.c, copied to RAM with
@ it: callers enter in Thumb and bx pc switches to the ARM code of Func_08021470
@ that follows, a mode switch inside a function. The halfword after it is
@ never run.
.syntax unified
	.thumb
	.global Data_0802146c
Data_0802146c:
	bx	pc
AlchemyUncredited_0802146e:
	.2byte	0x0200
AlchemyUncreditedEnd_0802146e:
