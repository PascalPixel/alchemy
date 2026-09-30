.syntax unified
	.thumb
	.global Func_08118d60
	.thumb_func
Func_08118d60:
	movs r3, #128
	lsls r3, r3, #19
	movs r2, #0
	adds r3, #18
	strh r2, [r3]
	bx lr
