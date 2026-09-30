.syntax unified
	.thumb
	.global Func_0802509c
	.thumb_func
Func_0802509c:
	push {r5, lr}
	adds r5, r0, #0
	movs r3, #4
	ldrsh r2, [r5, r3]
	ldr r3, [r5]
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r3, #4
	ldmia r3!, {r1}
	ldr r2, [r5, #12]
	ldr r3, [r3]
	bl Object_SetMoveTarget
	ldrh r3, [r5, #4]
	movs r0, #1
	adds r3, #3
	strh r3, [r5, #4]
	pop {r5, pc}
