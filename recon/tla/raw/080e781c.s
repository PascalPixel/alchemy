.syntax unified
	.thumb
	.global Func_080e781c
	.thumb_func
Func_080e781c:
	push {lr}
	bl Func_080cdf5c
	bl ObjectTable_Get
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #181
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #252
	lsls r3, r3, #8
	adds r3, #137
	strh r3, [r2]
	pop {pc}
