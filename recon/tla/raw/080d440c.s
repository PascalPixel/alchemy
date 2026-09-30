.syntax unified
	.thumb
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r10, r1
	movs r1, #213
	sub sp, #16
	lsls r1, r1, #4
	adds r6, r0, #0
	movs r0, #108
	str r3, [sp, #12]
	adds r7, r2, #0
	bl Runtime_AllocateBlock
	movs r1, #230
	str r0, [sp, #8]
	lsls r1, r1, #1
	adds r3, r0, r1
	ldr r5, [r3]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	movs r2, #240
	adds r3, r1, #0
	adds r3, #236
	ldr r3, [r3]
	lsls r2, r2, #15
	adds r2, r3, r2
	str r2, [sp, #4]
	adds r3, r1, #0
	adds r3, #240
	ldr r2, [r5, #12]
	ldr r3, [r3]
	movs r0, #192
	adds r3, r3, r2
	lsls r0, r0, #15
	adds r0, r3, r0
	str r0, [sp, #0]
	adds r3, r1, #0
	adds r3, #244
	ldr r3, [r3]
	ldr r0, .L_080d4504
	adds r0, r0, r3
	adds r3, r1, #0
	adds r3, #248
	ldr r3, [r3]
	mov r11, r0
	adds r3, r3, r2
	ldr r2, .L_080d4508
	adds r0, r5, #0
	adds r2, r2, r3
	movs r3, #8
	adds r3, r3, r5
	str r3, [r1]
	mov r8, r3
	mov r9, r2
	bl Object_ResetMotion
	movs r3, #1
	negs r3, r3
	cmp r6, r3
	bne .L_080d4492
	mov r0, r8
	ldr r6, [r0]
.L_080d4492:
	cmp r10, r3
	bne .L_080d449a
	ldr r1, [r5, #12]
	mov r10, r1
.L_080d449a:
	cmp r7, r3
	bne .L_080d44a0
	ldr r7, [r5, #16]
.L_080d44a0:
	ldr r2, [sp, #4]
	cmp r6, r2
	bge .L_080d44a8
	adds r6, r2, #0
.L_080d44a8:
	ldr r3, [sp, #0]
	cmp r7, r3
	bge .L_080d44b0
	adds r7, r3, #0
.L_080d44b0:
	cmp r6, r11
	ble .L_080d44b6
	mov r6, r11
.L_080d44b6:
	cmp r7, r9
	ble .L_080d44bc
	mov r7, r9
.L_080d44bc:
	ldr r0, [sp, #12]
	cmp r0, #0
	bne .L_080d44ea
	mov r1, r8
	mov r2, r10
	str r6, [r1]
	movs r0, #1
	str r2, [r5, #12]
	str r7, [r5, #16]
	bl WaitFrames
	ldr r0, [sp, #8]
	movs r1, #197
	lsls r1, r1, #1
	adds r3, r0, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	beq .L_080d44f6
	bl Func_08020120
	b .L_080d44f6
.L_080d44ea:
	adds r0, r5, #0
	adds r1, r6, #0
	mov r2, r10
	adds r3, r7, #0
	bl Object_SetPosition
.L_080d44f6:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080d4504:
	.4byte 0xff880000
.L_080d4508:
	.4byte 0xffc00000
