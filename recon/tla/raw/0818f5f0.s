.syntax unified
	.thumb
	.global Func_0818f5f0
	.thumb_func
Func_0818f5f0:
	push {r5, r6, lr}
	adds r6, r0, #0
	ldr r0, [r6, #8]
	bl Func_08118088 + 0x10
	ldr r5, [r0]
	movs r1, #2
	adds r0, r5, #0
	bl Object_SetMode
	adds r0, r5, #0
	movs r1, #28
	bl ObjectDispatch_ApplyValueToChildrenFar
	adds r0, r6, #0
	movs r1, #3
	bl Func_0818f620
	adds r0, r5, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
	pop {r5, r6, pc}
	.2byte 0x0000
