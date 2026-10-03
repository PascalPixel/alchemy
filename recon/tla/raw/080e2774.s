.syntax unified
	.thumb
	.global Func_080e2774
	.thumb_func
Func_080e2774:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #44
	str r1, [sp, #36]
	ldr r1, [sp, #76]
	lsls r3, r3, #16
	lsls r1, r1, #16
	asrs r3, r3, #16
	asrs r1, r1, #16
	str r3, [sp, #32]
	str r1, [sp, #28]
	str r0, [sp, #40]
	movs r5, #192
	lsls r2, r2, #16
	lsls r5, r5, #18
	asrs r6, r2, #16
	ldr r7, [r5, #108]
	bl EventRuntime_GetControlledOwner
	bl Object_GetById
	ldr r3, [r5, #32]
	movs r2, #132
	lsls r2, r2, #1
	adds r1, r3, r2
	movs r3, #204
	lsls r3, r3, #4
	adds r4, r7, r3
	ldr r3, [r1, #8]
	asrs r5, r3, #20
	str r5, [sp, #20]
	ldr r2, [r1, #12]
	asrs r5, r2, #20
	str r5, [sp, #16]
	cmp r6, #2
	bgt .L_080e27d2
	lsls r3, r6, #3
	subs r3, r3, r6
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r3, [r1, #8]
	ldr r2, [r1, #12]
.L_080e27d2:
	ldr r1, [sp, #40]
	asrs r3, r3, #20
	subs r3, r1, r3
	str r3, [sp, #12]
	ldr r3, [sp, #36]
	asrs r2, r2, #20
	subs r2, r3, r2
	str r2, [sp, #8]
	movs r5, #204
	ldr r3, [r0, #20]
	lsls r5, r5, #4
	asrs r3, r3, #20
	negs r3, r3
	str r3, [sp, #24]
	adds r3, r7, r5
	movs r0, #0
	ldrsh r3, [r3, r0]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	beq .L_080e28c0
	adds r5, r4, #0
.L_080e27fe:
	movs r3, #2
	ldrsh r6, [r5, r3]
	ldr r4, [sp, #24]
	ldr r2, [sp, #20]
	movs r1, #0
	ldrsh r7, [r5, r1]
	ldr r0, [sp, #16]
	adds r3, r6, r4
	subs r1, r7, r2
	subs r2, r3, r0
	ldr r3, [sp, #12]
	cmp r1, r3
	blt .L_080e28b2
	ldr r4, [sp, #32]
	adds r3, r3, r4
	cmp r1, r3
	bge .L_080e28b2
	ldr r0, [sp, #8]
	cmp r2, r0
	blt .L_080e28b2
	ldr r4, [sp, #28]
	adds r3, r0, r4
	cmp r2, r3
	bge .L_080e28b2
	ldr r0, [sp, #12]
	ldr r4, [sp, #80]
	subs r3, r1, r0
	ldr r1, [sp, #8]
	ldr r0, [sp, #84]
	subs r2, r2, r1
	ldr r1, [sp, #40]
	adds r4, r4, r3
	adds r1, r1, r3
	ldr r3, [sp, #36]
	adds r0, r0, r2
	mov r8, r4
	adds r3, r3, r2
	mov r9, r0
	str r1, [sp, #0]
	str r3, [sp, #4]
	mov r0, r8
	movs r2, #1
	mov r11, r1
	mov r10, r3
	mov r1, r9
	movs r3, #1
	bl Func_080201f0
	lsls r1, r6, #20
	movs r2, #2
	lsls r0, r7, #20
	bl Func_080dbb78
	ldr r1, .L_080e28d0
	movs r4, #200
	lsls r4, r4, #5
	movs r3, #0
	adds r4, #76
	strb r3, [r0, #2]
	adds r0, r1, r4
	movs r2, #0
	ldrsb r2, [r0, r2]
	cmp r2, #63
	bgt .L_080e28b2
	movs r4, #184
	lsls r4, r4, #5
	lsls r2, r2, #3
	adds r4, #76
	adds r3, r2, r4
	mov r4, r8
	strb r4, [r1, r3]
	adds r3, r3, r1
	mov r4, r9
	strb r4, [r3, #1]
	mov r4, r11
	strb r4, [r3, #2]
	mov r4, r10
	strb r4, [r3, #3]
	movs r3, #184
	lsls r3, r3, #5
	adds r3, #80
	adds r2, r2, r3
	ldrh r3, [r5]
	strb r3, [r1, r2]
	adds r2, r2, r1
	ldrh r3, [r5, #2]
	strb r3, [r2, #1]
	ldrb r3, [r0]
	adds r3, #1
	strb r3, [r0]
.L_080e28b2:
	adds r5, #4
	movs r4, #0
	ldrsh r3, [r5, r4]
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_080e27fe
.L_080e28c0:
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e28d0:
	.4byte Data_02001000
