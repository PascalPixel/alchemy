.syntax unified
	.thumb
	.global Func_080e89e4
	.thumb_func
Func_080e89e4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	ldr r1, [r2, #108]
	movs r4, #230
	lsls r4, r4, #1
	adds r3, r1, r4
	ldr r3, [r3]
	ldr r7, [r2, #32]
	mov r8, r3
	adds r3, r7, #0
	adds r3, #228
	movs r4, #2
	ldrsh r5, [r3, r4]
	adds r3, #4
	mov r11, r5
	movs r4, #2
	ldrsh r5, [r3, r4]
	sub sp, #20
	str r5, [sp, #0]
	adds r3, #20
	ldr r3, [r3]
	ldr r6, [r2, #92]
	cmp r3, #0
	beq .L_080e8a32
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	bne .L_080e8a4a
	movs r3, #2
	ands r3, r0
	cmp r3, #0
	bne .L_080e8a62
.L_080e8a32:
	movs r5, #197
	lsls r5, r5, #1
	adds r3, r1, r5
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080e8a72
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_080e8a5a
.L_080e8a4a:
	movs r0, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r2, r2, #9
	movs r1, #0
	bl Func_08020228
	b .L_080e8b30
.L_080e8a5a:
	movs r3, #2
	ands r3, r0
	cmp r3, #0
	beq .L_080e8a72
.L_080e8a62:
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #0
	bl Func_08020228
	b .L_080e8b30
.L_080e8a72:
	ldr r3, .L_080e8b40
	movs r0, #253
	lsls r0, r0, #1
	add r5, sp, #4
	adds r3, r3, r0
	movs r1, #0
	ldrsh r0, [r3, r1]
	adds r1, r5, #0
	bl Func_08020328
	ldr r3, [r5, #4]
	mov r1, r8
	mov lr, r3
	lsls r0, r3, #3
	ldr r3, [r1, #8]
	ldr r2, [r5]
	ldr r4, [r5, #8]
	ldr r5, [r5, #12]
	str r3, [r6]
	mov r12, r2
	ldr r3, [r1, #12]
	lsls r2, r2, #3
	str r3, [r6, #4]
	adds r2, #12
	ldr r3, [r1, #16]
	movs r1, #0
	mov r10, r4
	mov r9, r5
	lsls r4, r4, #3
	lsls r5, r5, #3
	str r3, [r6, #8]
	strh r1, [r6, #28]
	cmp r11, r2
	bge .L_080e8abe
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r6, #28]
.L_080e8abe:
	mov r2, r11
	adds r3, r0, #0
	adds r2, #240
	subs r3, #12
	cmp r2, r3
	blt .L_080e8ace
	movs r3, #1
	strh r3, [r6, #28]
.L_080e8ace:
	strh r1, [r6, #30]
	ldr r2, [sp, #0]
	adds r3, r4, #0
	adds r3, #12
	cmp r2, r3
	bge .L_080e8ae2
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r6, #30]
.L_080e8ae2:
	ldr r2, [sp, #0]
	adds r3, r5, #0
	adds r2, #160
	subs r3, #12
	cmp r2, r3
	blt .L_080e8af2
	movs r3, #1
	strh r3, [r6, #30]
.L_080e8af2:
	adds r4, r7, #0
	adds r4, #236
	ldr r3, [r4]
	adds r0, r7, #0
	str r3, [r6, #12]
	adds r0, #244
	ldr r3, [r0]
	adds r1, r7, #0
	str r3, [r6, #16]
	adds r1, #240
	ldr r3, [r1]
	adds r2, r7, #0
	str r3, [r6, #20]
	adds r2, #248
	ldr r3, [r2]
	mov r5, r12
	str r3, [r6, #24]
	lsls r3, r5, #19
	str r3, [r4]
	mov r4, lr
	lsls r3, r4, #19
	mov r5, r10
	str r3, [r0]
	lsls r3, r5, #19
	mov r0, r9
	str r3, [r1]
	lsls r3, r0, #19
	str r3, [r2]
	mov r0, r8
	bl Object_ResetMotion
.L_080e8b30:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e8b40:
	.4byte gPartyState
