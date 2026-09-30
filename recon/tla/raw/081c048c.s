.syntax unified
	.thumb
	.global Audio_NoopStubWorkCopy
	.thumb_func
Audio_NoopStubWorkCopy:
	bx lr
	.2byte 0x0000
