.syntax unified
	.thumb
	.global Func_08024ea8
	.thumb_func
Func_08024ea8:
	push {r5, lr}
	adds r5, r0, #0
	movs r2, #4
	ldrsh r3, [r5, r2]
	ldr r2, [r5]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldr r1, [r3, #4]
	bl ObjectDispatch_ApplyArgumentToChildren
	ldrh r3, [r5, #4]
	movs r0, #1
	adds r3, #2
	strh r3, [r5, #4]
	pop {r5, pc}
	.2byte 0x0000
