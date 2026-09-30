.syntax unified
	.thumb
	.global Func_080e3060
	.thumb_func
Func_080e3060:
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r0, #6]
	bx lr
