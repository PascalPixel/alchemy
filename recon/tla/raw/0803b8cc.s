.syntax unified
	.thumb
	.global Func_0803b8cc
	.thumb_func
Func_0803b8cc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r3
	movs r3, #192
	lsls r3, r3, #18
	adds r6, r1, #0
	movs r1, #0
	ldr r5, [r3, #60]
	adds r7, r2, #0
	sub sp, #12
	bl UiText_BuildRenderEntries
	movs r2, #244
	lsls r3, r0, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	ldrh r3, [r5, r3]
	cmp r3, #0
	bne .L_0803b8f8
	movs r0, #0
	b .L_0803b910
.L_0803b8f8:
	ldr r3, [sp, #32]
	adds r1, r6, #0
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #1
	str r3, [sp, #8]
	adds r2, r7, #0
	mov r3, r8
	bl UiWindow_FitOnScreen
	movs r0, #1
.L_0803b910:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
