.syntax unified
	.thumb
	.global Func_0802cc74
	.thumb_func
Func_0802cc74:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r2, r2, r3
	movs r3, #0
	strh r3, [r2, #34]
	bx lr
