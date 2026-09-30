.syntax unified
	.thumb
	.global UiText_OpenMessageWindow
	.thumb_func
UiText_OpenMessageWindow:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r5, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #36
	ldr r3, [r3, #60]
	str r2, [sp, #20]
	movs r2, #152
	lsls r2, r2, #5
	mov r8, r3
	adds r2, #132
	lsls r3, r5, #4
	add r2, r8
	lsrs r3, r3, #20
	str r1, [sp, #24]
	strh r3, [r2]
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #134
	add r3, r8
	movs r6, #0
	strh r6, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	movs r1, #1
	ands r5, r3
	bl UiText_BuildRenderEntries
	lsls r3, r0, #1
	mov r10, r0
	movs r0, #244
	lsls r0, r0, #4
	adds r3, r3, r0
	mov r2, r8
	ldrh r3, [r2, r3]
	movs r7, #0
	movs r0, #0
	cmp r3, #0
	beq .L_0803a79e
	add r0, sp, #12
	str r0, [sp, #0]
	add r0, sp, #28
	add r2, sp, #20
	str r0, [sp, #4]
	add r1, sp, #24
	add r3, sp, #16
	mov r9, r0
	mov r0, r10
	str r6, [sp, #8]
	bl UiWindow_FitOnScreen
	ldr r2, [sp, #16]
	cmp r2, #0
	bne .L_0803a71c
	ldr r4, [sp, #12]
	cmp r4, #0
	bne .L_0803a71e
	movs r0, #0
	b .L_0803a79e
.L_0803a71c:
	ldr r4, [sp, #12]
.L_0803a71e:
	movs r3, #1
	ands r3, r5
	cmp r3, #0
	bne .L_0803a72a
	movs r3, #2
	orrs r7, r3
.L_0803a72a:
	movs r1, #8
	adds r3, r5, #0
	ands r3, r1
	cmp r3, #0
	beq .L_0803a736
	orrs r7, r1
.L_0803a736:
	movs r3, #16
	ands r3, r5
	cmp r3, #0
	beq .L_0803a742
	movs r3, #128
	orrs r7, r3
.L_0803a742:
	movs r3, #32
	ands r3, r5
	cmp r3, #0
	beq .L_0803a750
	movs r3, #128
	lsls r3, r3, #1
	orrs r7, r3
.L_0803a750:
	ldr r0, [sp, #24]
	ldr r1, [sp, #20]
	adds r3, r4, #0
	str r7, [sp, #0]
	bl UiWindow_Create
	adds r6, r0, #0
	movs r0, #0
	cmp r6, #0
	beq .L_0803a79e
	mov r2, r9
	str r2, [sp, #0]
	movs r5, #0
	adds r0, r6, #0
	mov r1, r10
	movs r2, #0
	movs r3, #0
	str r5, [sp, #4]
	bl UiText_QueueRenderEntries
	cmp r0, #0
	bne .L_0803a788
	adds r0, r6, #0
	movs r1, #1
	bl UiWork_Finalize
	movs r0, #0
	b .L_0803a79e
.L_0803a788:
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #138
	add r3, r8
	strb r5, [r3]
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #139
	add r3, r8
	strb r5, [r3]
	adds r0, r6, #0
.L_0803a79e:
	add sp, #36
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
