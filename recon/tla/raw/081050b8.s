.syntax unified
	.thumb
	.global Func_081050b8
	.thumb_func
Func_081050b8:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #10
	adds r3, r3, r2
	strb r0, [r3]
	bx lr
