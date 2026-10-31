@ Why assembly: the Thumb entry of the grid outliner, copied to RAM with it:
@ callers enter in Thumb and bx pc switches to the ARM code of Func_08021828
@ (f021828.c) that follows, a mode switch inside a function. The halfword
@ after it is never run.
.syntax unified
	.thumb
	.global Func_08021824
	.thumb_func
Func_08021824:
	.global Data_08021824
Data_08021824:
	bx	pc
AlchemyUncredited_08021826:
	.2byte	0x0200
AlchemyUncreditedEnd_08021826:
