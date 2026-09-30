.syntax unified
	.thumb
	.global Func_081054cc
	.thumb_func
Func_081054cc:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #18
	adds r3, r3, r2
	strb r0, [r3]
	bx lr
