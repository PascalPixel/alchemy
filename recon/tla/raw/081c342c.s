.syntax unified
	.thumb
	.global Audio_EmptyCallback
	.thumb_func
Audio_EmptyCallback:
	bx lr
	.2byte 0x0000
