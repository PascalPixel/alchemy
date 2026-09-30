.syntax unified
	.thumb
	.global Func_0804cda8
	.thumb_func
Func_0804cda8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #192
	lsls r0, r0, #18
	adds r3, r0, #0
	adds r3, #232
	ldr r3, [r3]
	ldr r0, [r0, #124]
	mov r8, r3
	ldr r3, .L_0804d0b0
	ldr r1, .L_0804d0b4
	ldr r3, [r3]
	movs r2, #31
	lsls r3, r3, #1
	ands r3, r2
	lsls r3, r3, #1
	ldrh r7, [r1, r3]
	mov r9, r0
	ldr r0, .L_0804d0b8
	sub sp, #12
	adds r3, r7, r0
	mov r6, r8
	cmp r3, #0
	bge .L_0804cde6
	adds r3, r7, #0
	subs r3, #253
.L_0804cde6:
	movs r1, #152
	asrs r3, r3, #2
	lsls r1, r1, #1
	ldr r4, .L_0804d0bc
	adds r7, r3, r1
	ldr r3, [sp, #4]
	lsls r1, r7, #16
	movs r2, #255
	lsrs r1, r1, #16
	ands r3, r4
	lsls r2, r2, #8
	adds r2, #255
	orrs r3, r1
	ands r3, r2
	lsls r1, r1, #16
	orrs r3, r1
	str r3, [sp, #4]
	add r0, sp, #4
	ldr r3, [r0, #4]
	movs r5, #0
	ands r3, r4
	str r3, [r0, #4]
	bl AffineMatrix_BuildForEffect
	movs r2, #142
	add r2, r8
	mov r11, r0
	movs r0, #0
	ldrsh r3, [r2, r0]
	mov r10, r2
	cmp r5, r3
	bcs .L_0804cee2
	ldr r4, .L_0804d0c0
.L_0804ce28:
	movs r1, #12
	ldrsh r2, [r6, r1]
	cmp r2, #0
	beq .L_0804ced4
	mov r3, r8
	adds r3, #140
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r5, r3
	bne .L_0804cea8
	lsls r3, r7, #3
	subs r3, r3, r7
	cmp r3, #0
	bge .L_0804ce4c
	movs r1, #128
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r3, r1
.L_0804ce4c:
	asrs r3, r3, #9
	adds r3, r2, r3
	movs r2, #14
	ldrsh r1, [r6, r2]
	subs r3, #20
	mov r12, r3
	cmp r1, #0
	beq .L_0804ce70
	lsls r3, r7, #1
	adds r3, r3, r7
	cmp r3, #0
	bge .L_0804ce66
	adds r3, #255
.L_0804ce66:
	asrs r3, r3, #8
	adds r3, r1, r3
	adds r1, r3, #0
	subs r1, #20
	b .L_0804ce84
.L_0804ce70:
	lsls r3, r7, #4
	subs r3, r3, r7
	cmp r3, #0
	bge .L_0804ce7a
	adds r3, #255
.L_0804ce7a:
	asrs r3, r3, #8
	adds r1, r3, #0
	subs r1, #30
	movs r3, #255
	ands r1, r3
.L_0804ce84:
	adds r0, r6, #0
	movs r3, #0
	mov r2, r11
	stmia r0!, {r3}
	lsls r3, r2, #25
	orrs r3, r1
	mov r1, r12
	lsls r2, r1, #16
	orrs r3, r2
	ldr r2, .L_0804d0c4
	movs r1, #246
	orrs r3, r2
	stmia r0!, {r3}
	ldrh r3, [r6, #18]
	lsls r3, r3, #2
	adds r3, r3, r4
	ldrh r3, [r3, #2]
	b .L_0804cec6
.L_0804cea8:
	movs r3, #14
	ldrsh r1, [r6, r3]
	adds r0, r6, #0
	movs r3, #0
	stmia r0!, {r3}
	lsls r3, r2, #16
	orrs r1, r3
	ldr r3, .L_0804d0c8
	orrs r1, r3
	ldrh r3, [r6, #18]
	stmia r0!, {r1}
	lsls r3, r3, #2
	adds r3, r3, r4
	ldrh r3, [r3, #2]
	movs r1, #245
.L_0804cec6:
	lsrs r3, r3, #5
	str r3, [r0]
	adds r0, r6, #0
	str r4, [sp, #0]
	bl Runtime_PushSlotEntry
	ldr r4, [sp, #0]
.L_0804ced4:
	mov r1, r10
	movs r0, #0
	ldrsh r3, [r1, r0]
	adds r5, #1
	adds r6, #20
	cmp r5, r3
	bcc .L_0804ce28
.L_0804cee2:
	mov r2, r9
	cmp r2, #0
	bne .L_0804ceea
	b .L_0804d0a2
.L_0804ceea:
	movs r3, #136
	lsls r3, r3, #3
	adds r3, #255
	add r3, r9
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_0804cefe
	b .L_0804d0a2
.L_0804cefe:
	mov r1, r10
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #0
	bne .L_0804cf0a
	b .L_0804d0a2
.L_0804cf0a:
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #57
	add r3, r9
	ldrb r2, [r3]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #5
	adds r3, r3, r2
	lsls r3, r3, #2
	add r3, r9
	mov r12, r3
	mov r3, r8
	adds r3, #148
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0804cfea
	movs r3, #140
	add r3, r8
	mov r10, r3
	movs r0, #0
	ldrsh r3, [r3, r0]
	lsls r0, r7, #1
	lsls r2, r3, #2
	adds r2, r2, r3
	lsls r2, r2, #2
	adds r1, r2, #0
	adds r1, #12
	mov r2, r8
	ldrsh r4, [r2, r1]
	mov lr, r0
	adds r3, r0, r7
	ldr r0, .L_0804d0cc
	lsls r3, r3, #2
	adds r2, r3, r0
	cmp r2, #0
	bge .L_0804cf5a
	adds r0, #255
	adds r2, r3, r0
.L_0804cf5a:
	asrs r2, r2, #8
	subs r3, r4, r2
	lsls r3, r3, #8
	adds r2, r4, r2
	adds r3, r3, r2
	mov r2, r8
	adds r4, r3, #0
	adds r3, r2, r1
	movs r0, #2
	ldrsh r3, [r3, r0]
	ldr r1, .L_0804d0d0
	lsls r2, r7, #5
	adds r0, r3, #0
	adds r3, r2, r1
	adds r4, #23
	adds r0, #24
	cmp r3, #0
	bge .L_0804cf82
	ldr r1, .L_0804d0d4
	adds r3, r2, r1
.L_0804cf82:
	asrs r3, r3, #9
	adds r3, r0, r3
	mov r1, r12
	adds r0, r3, #1
	movs r5, #24
	adds r1, #102
	cmp r5, r0
	bcs .L_0804cfa6
	movs r6, #255
.L_0804cf94:
	ldrh r2, [r1]
	adds r3, r6, #0
	ands r3, r2
	orrs r3, r4
	adds r5, #1
	strh r3, [r1]
	adds r1, #4
	cmp r5, r0
	bcc .L_0804cf94
.L_0804cfa6:
	mov r3, r8
	mov r1, r10
	movs r2, #12
	ldrsh r4, [r3, r2]
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #0
	bne .L_0804cfcc
	mov r2, lr
	ldr r0, .L_0804d0cc
	adds r3, r2, r7
	lsls r1, r3, #2
	adds r3, r1, r0
	cmp r3, #0
	bge .L_0804cfc8
	ldr r2, .L_0804d0d8
	adds r3, r1, r2
.L_0804cfc8:
	asrs r3, r3, #8
	subs r4, r4, r3
.L_0804cfcc:
	mov r1, r12
	lsls r4, r4, #8
	adds r1, #6
	movs r5, #0
	movs r0, #255
.L_0804cfd6:
	ldrh r2, [r1]
	adds r3, r0, #0
	ands r3, r2
	orrs r3, r4
	adds r5, #1
	strh r3, [r1]
	adds r1, #4
	cmp r5, #23
	bls .L_0804cfd6
	b .L_0804d0a2
.L_0804cfea:
	movs r3, #140
	add r3, r8
	mov lr, r3
	movs r0, #0
	ldrsh r3, [r3, r0]
	lsls r6, r7, #1
	lsls r2, r3, #2
	adds r2, r2, r3
	lsls r2, r2, #2
	adds r1, r2, #0
	adds r1, #12
	mov r2, r8
	ldr r0, .L_0804d0cc
	ldrsh r4, [r2, r1]
	adds r3, r6, r7
	lsls r3, r3, #2
	adds r2, r3, r0
	cmp r2, #0
	bge .L_0804d014
	adds r0, #255
	adds r2, r3, r0
.L_0804d014:
	asrs r2, r2, #8
	subs r3, r4, r2
	lsls r3, r3, #8
	adds r2, r4, r2
	adds r3, r3, r2
	mov r2, r8
	adds r4, r3, #0
	adds r3, r2, r1
	movs r1, #2
	ldrsh r0, [r3, r1]
	ldr r1, .L_0804d0d0
	lsls r2, r7, #5
	adds r3, r2, r1
	adds r4, #23
	cmp r3, #0
	bge .L_0804d038
	ldr r1, .L_0804d0d4
	adds r3, r2, r1
.L_0804d038:
	asrs r3, r3, #9
	subs r3, r0, r3
	subs r0, r3, #1
	lsls r3, r0, #2
	add r3, r12
	adds r5, r0, #0
	adds r1, r3, #6
	cmp r5, #135
	bhi .L_0804d05e
	movs r0, #255
.L_0804d04c:
	ldrh r2, [r1]
	adds r3, r0, #0
	ands r3, r2
	orrs r3, r4
	adds r5, #1
	strh r3, [r1]
	adds r1, #4
	cmp r5, #135
	bls .L_0804d04c
.L_0804d05e:
	mov r3, r8
	mov r1, lr
	movs r2, #12
	ldrsh r4, [r3, r2]
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #0
	bne .L_0804d082
	ldr r2, .L_0804d0cc
	adds r3, r6, r7
	lsls r1, r3, #2
	adds r3, r1, r2
	cmp r3, #0
	bge .L_0804d07e
	ldr r0, .L_0804d0d8
	adds r3, r1, r0
.L_0804d07e:
	asrs r3, r3, #8
	subs r4, r4, r3
.L_0804d082:
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #38
	lsls r4, r4, #8
	add r1, r12
	movs r5, #136
	movs r0, #255
.L_0804d090:
	ldrh r2, [r1]
	adds r3, r0, #0
	ands r3, r2
	orrs r3, r4
	adds r5, #1
	strh r3, [r1]
	adds r1, #4
	cmp r5, #159
	bls .L_0804d090
.L_0804d0a2:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0804d0b0:
	.4byte Data_0300122c
.L_0804d0b4:
	.4byte Data_0805e9c4
.L_0804d0b8:
	.4byte 0xffffff00
.L_0804d0bc:
	.4byte 0xffff0000
.L_0804d0c0:
	.4byte ResourceTableEntries
.L_0804d0c4:
	.4byte 0x80002300
.L_0804d0c8:
	.4byte 0x80002000
.L_0804d0cc:
	.4byte 0xfffff4ff
.L_0804d0d0:
	.4byte 0xffffe0ff
.L_0804d0d4:
	.4byte 0xffffe2fe
.L_0804d0d8:
	.4byte 0xfffff5fe
