.syntax unified
	.thumb
	.global Func_080e25e8
	.thumb_func
Func_080e25e8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #28
	str r3, [sp, #12]
	ldr r3, [sp, #64]
	str r1, [sp, #20]
	str r2, [sp, #16]
	str r0, [sp, #24]
	lsls r3, r3, #16
	movs r6, #192
	asrs r3, r3, #16
	lsls r6, r6, #18
	ldr r7, [sp, #68]
	mov r11, r3
	ldr r5, [r6, #108]
	bl Func_080cdf5c
	bl Object_GetById
	ldr r3, [r0, #20]
	movs r2, #204
	asrs r3, r3, #20
	str r3, [sp, #8]
	lsls r2, r2, #4
	adds r1, r5, r2
	lsls r3, r7, #3
	ldr r2, [r6, #32]
	subs r3, r3, r7
	lsls r3, r3, #3
	adds r2, r2, r3
	movs r3, #132
	lsls r3, r3, #1
	adds r3, r3, r2
	adds r5, r1, #0
	mov r9, r3
	movs r0, #0
	ldrsh r3, [r5, r0]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	beq .L_080e2704
.L_080e2644:
	movs r2, #0
	ldrsh r3, [r5, r2]
	ldr r4, [sp, #60]
	mov r2, r11
	subs r1, r3, r4
	movs r0, #2
	ldrsh r3, [r5, r0]
	ldr r4, [sp, #8]
	subs r3, r3, r2
	mov r0, r9
	subs r2, r3, r4
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	adds r1, r1, r3
	ldr r3, [r0, #12]
	asrs r3, r3, #20
	adds r2, r2, r3
	cmp r1, #0
	blt .L_080e26f6
	ldr r3, [sp, #16]
	cmp r1, r3
	bge .L_080e26f6
	cmp r2, #0
	blt .L_080e26f6
	ldr r4, [sp, #12]
	cmp r2, r4
	bge .L_080e26f6
	ldr r3, [sp, #20]
	ldr r0, [sp, #24]
	ldr r4, [sp, #60]
	adds r3, r3, r2
	adds r6, r0, r1
	mov r8, r3
	mov r0, r11
	adds r4, r4, r1
	movs r3, #1
	adds r7, r0, r2
	mov r1, r8
	movs r2, #1
	adds r0, r6, #0
	mov r10, r4
	str r4, [sp, #0]
	str r7, [sp, #4]
	bl Func_080201f0
	movs r2, #0
	ldrsh r0, [r5, r2]
	movs r3, #2
	ldrsh r1, [r5, r3]
	movs r2, #2
	lsls r1, r1, #20
	lsls r0, r0, #20
	bl Func_080dbb78
	ldr r1, .L_080e2714
	movs r4, #200
	lsls r4, r4, #5
	movs r3, #0
	adds r4, #76
	strb r3, [r0, #2]
	adds r0, r1, r4
	movs r2, #0
	ldrsb r2, [r0, r2]
	cmp r2, #63
	bgt .L_080e26f6
	movs r4, #184
	lsls r4, r4, #5
	lsls r2, r2, #3
	adds r4, #76
	adds r3, r2, r4
	strb r6, [r1, r3]
	mov r4, r8
	adds r3, r3, r1
	strb r4, [r3, #1]
	mov r4, r10
	strb r4, [r3, #2]
	strb r7, [r3, #3]
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
.L_080e26f6:
	adds r5, #4
	movs r4, #0
	ldrsh r3, [r5, r4]
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_080e2644
.L_080e2704:
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e2714:
	.4byte Data_02001000
