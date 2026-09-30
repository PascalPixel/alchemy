.syntax unified
	.thumb
	.global Func_0803a54c
	.thumb_func
Func_0803a54c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #60]
	adds r3, #148
	ldr r6, [r3]
	movs r3, #2
	strb r3, [r5, #5]
	movs r1, #1
	sub sp, #4
	bl UiText_BuildRenderEntries
	movs r2, #1
	mov r10, r2
	mov r3, r10
	strb r3, [r5, #5]
	movs r2, #244
	lsls r3, r0, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	ldrh r3, [r5, r3]
	movs r7, #0
	mov r8, r0
	cmp r3, #0
	beq .L_0803a5d6
	ldr r0, [r6]
	cmp r0, #0
	bne .L_0803a5b2
	movs r3, #10
	str r3, [sp, #0]
	movs r1, #15
	movs r2, #30
	movs r3, #6
	movs r0, #0
	bl UiWindow_Create
	mov r3, r10
	adds r5, r0, #0
	str r5, [r6]
	movs r0, #0
	str r3, [sp, #0]
	movs r1, #15
	movs r2, #30
	movs r3, #6
	bl Func_0803a2b0
	str r7, [r6, #8]
	b .L_0803a5b4
.L_0803a5b2:
	adds r5, r0, #0
.L_0803a5b4:
	cmp r5, #0
	beq .L_0803a5d6
	ldr r2, [r6, #8]
	adds r0, r5, #0
	mov r1, r8
	bl Func_080395fc
	movs r3, #0
	adds r7, r0, #0
	str r7, [r6, #4]
	str r3, [r6, #8]
	cmp r7, #0
	bne .L_0803a5d6
	adds r0, r5, #0
	movs r1, #1
	bl UiWork_Finalize
.L_0803a5d6:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
