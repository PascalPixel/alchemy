.syntax unified
	.thumb
	.global Func_080d3be8
	.thumb_func
Func_080d3be8:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #226
	lsls r2, r2, #1
	adds r3, r3, r2
	strh r0, [r3]
	bx lr
