.syntax unified
	.thumb
	.global Func_08042690
	.thumb_func
Func_08042690:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	mov r8, r0
	movs r1, #16
	movs r0, #64
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #60]
	movs r3, #0
	mov r10, r3
	movs r3, #1
	adds r5, r0, #0
	strb r3, [r6, #6]
	mov r0, r8
	bl Func_08042630
	ldrh r3, [r5, #10]
	ldrh r1, [r5, #6]
	ldrh r2, [r5, #8]
	movs r4, #6
	ldrh r0, [r5, #4]
	str r4, [sp, #0]
	bl UiWindow_Create
	str r0, [r5]
	mov r0, r8
	bl Func_0804297c
	mov r3, r10
	strb r3, [r6, #6]
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
