.syntax unified
	.thumb
	.global Func_080250c0
	.thumb_func
Func_080250c0:
	push {r5, lr}
	adds r5, r0, #0
	movs r3, #4
	ldrsh r2, [r5, r3]
	ldr r3, [r5]
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r3, #4
	ldmia r3!, {r2}
	ldr r1, [r5, #8]
	ldmia r3!, {r0}
	adds r1, r1, r2
	ldr r4, [r3]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r2, r2, r0
	adds r3, r3, r4
	adds r0, r5, #0
	bl Object_SetMoveTarget
	ldrh r3, [r5, #4]
	movs r0, #1
	adds r3, #4
	strh r3, [r5, #4]
	pop {r5, pc}
	.2byte 0x0000
