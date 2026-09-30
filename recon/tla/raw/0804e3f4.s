.syntax unified
	.thumb
	.global Func_0804e3f4
	.thumb_func
Func_0804e3f4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	sub sp, #24
	movs r0, #1
	str r0, [sp, #8]
	movs r1, #0
	movs r2, #10
	mov r11, r3
	movs r0, #0
	movs r3, #5
	bl Func_0803d3c0
	movs r3, #2
	movs r2, #14
	str r0, [sp, #12]
	str r3, [sp, #0]
	movs r1, #10
	movs r3, #3
	movs r0, #10
	bl UiWindow_Create
	adds r7, r0, #0
	ldr r0, .L_0804e574
	movs r5, #0
	ldrsh r3, [r0, r5]
	movs r2, #1
	negs r2, r2
	movs r1, #0
	cmp r3, r2
	beq .L_0804e450
	mov r12, r2
	adds r2, r0, #0
.L_0804e444:
	adds r2, #4
	movs r0, #0
	ldrsh r3, [r2, r0]
	adds r1, #1
	cmp r3, r12
	bne .L_0804e444
.L_0804e450:
	ldr r0, .L_0804e578
	mov r8, r1
	movs r1, #0
	ldrsh r3, [r0, r1]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	beq .L_0804e470
	mov r12, r2
	adds r2, r0, #0
.L_0804e464:
	adds r2, #4
	movs r0, #0
	ldrsh r3, [r2, r0]
	adds r1, #1
	cmp r3, r12
	bne .L_0804e464
.L_0804e470:
	add r1, r8
	ldr r6, .L_0804e57c
	mov r10, r1
	movs r1, #2
	mov r9, r1
.L_0804e47a:
	ldr r3, [r6, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_0804e48a
	movs r2, #1
	str r2, [sp, #8]
	subs r5, #1
.L_0804e48a:
	ldr r3, [r6, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_0804e49a
	movs r3, #1
	str r3, [sp, #8]
	adds r5, #1
.L_0804e49a:
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0804e4ac
	movs r0, #1
	str r0, [sp, #8]
	subs r5, #10
.L_0804e4ac:
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0804e4be
	movs r1, #1
	str r1, [sp, #8]
	adds r5, #10
.L_0804e4be:
	ldr r3, [r6, #12]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_0804e54c
	ldr r3, [r6, #12]
	mov r2, r9
	ands r3, r2
	cmp r3, #0
	bne .L_0804e54c
	ldr r3, [sp, #8]
	cmp r3, #0
	beq .L_0804e544
	movs r0, #0
	mov r1, r10
	str r0, [sp, #8]
	adds r0, r5, r1
	bl __modsi3
	adds r5, r0, #0
	adds r0, r7, #0
	bl RenderOutput_PrepareForRedraw
	cmp r5, r8
	bge .L_0804e4fa
	ldr r2, .L_0804e574
	lsls r3, r5, #2
	adds r3, #2
	ldrsh r0, [r2, r3]
	b .L_0804e50a
.L_0804e4fa:
	mov r0, r8
	subs r2, r5, r0
	ldr r3, .L_0804e578
	lsls r2, r2, #2
	adds r2, #2
	ldrsh r3, [r3, r2]
	adds r0, r3, #0
	adds r0, #128
.L_0804e50a:
	movs r1, #152
	lsls r1, r1, #5
	mov r2, r11
	adds r1, #130
	ldrh r3, [r2, r1]
	movs r2, #15
	str r3, [sp, #20]
	movs r3, #1
	str r2, [sp, #0]
	str r3, [sp, #4]
	add r2, sp, #20
	add r3, sp, #16
	movs r1, #0
	bl Func_0803dab0
	movs r3, #0
	adds r0, r5, #0
	movs r1, #2
	adds r2, r7, #0
	str r3, [sp, #0]
	bl UiText_DrawNumberInWindow
	ldr r0, .L_0804e580
	adds r1, r7, #0
	adds r0, r5, r0
	movs r2, #24
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
.L_0804e544:
	movs r0, #1
	bl WaitFrames
	b .L_0804e47a
.L_0804e54c:
	adds r0, r7, #0
	movs r1, #2
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #12]
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	movs r0, #0
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0804e574:
	.4byte Data_0805eb58
.L_0804e578:
	.4byte Data_0805eb7c
.L_0804e57c:
	.4byte gInput
.L_0804e580:
	.4byte 0x00001342
