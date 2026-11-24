@ Why assembly: the Thumb entry of the cell tiles builder: callers enter in
@ Thumb and bx pc switches to the ARM code of ScanlineWindow_BuildSpans
@ (window_spans.c) that follows, a mode switch inside a function.
@ The halfword after it is never run.
.syntax unified
	.thumb
	.global ScanlineWindow_BuildSpansCode
ScanlineWindow_BuildSpansCode:
	bx	pc
AlchemyUncredited_080388fe:
	.2byte	0x0200
AlchemyUncreditedEnd_080388fe:
