.syntax unified
	.thumb
	.global Func_080d0a28
	.thumb_func
Func_080d0a28:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #124]
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #60
	adds r4, r6, r1
	movs r2, #0
	ldrsb r2, [r4, r2]
	cmp r2, #0
	beq .L_080d0aba
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #61
	adds r1, r6, r3
	movs r3, #0
	ldrsb r3, [r1, r3]
	ldrb r0, [r1]
	cmp r3, r2
	blt .L_080d0a7e
	movs r3, #0
	strb r3, [r4]
	ldr r0, .L_080d0b68
	bl Scheduler_RemoveCallback
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #176
	ldrh r1, [r2, #10]
	movs r3, #197
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r1
	strh r3, [r2, #10]
	movs r3, #254
	ldrh r1, [r2, #10]
	lsls r3, r3, #7
	adds r3, #255
	ands r3, r1
	strh r3, [r2, #10]
	ldrh r3, [r2, #10]
	b .L_080d0b64
.L_080d0a7e:
	movs r7, #160
	lsls r7, r7, #3
	adds r7, #59
	adds r3, r6, r7
	movs r2, #0
	ldrsb r2, [r3, r2]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #58
	adds r5, r6, r3
	movs r3, #0
	ldrsb r3, [r5, r3]
	subs r7, #17
	subs r2, r2, r3
	adds r3, r0, #1
	strb r3, [r1]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r0, r3, #0
	muls r0, r2
	movs r1, #0
	ldrsb r1, [r4, r1]
	ldr r3, .L_080d0b6c
	mov lr, r3
	.2byte 0xf800
	movs r3, #0
	ldrsb r3, [r5, r3]
	adds r2, r6, r7
	adds r3, r3, r0
	strh r3, [r2]
.L_080d0aba:
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #42
	adds r3, r6, r1
	movs r2, #160
	ldrh r3, [r3]
	lsls r2, r2, #3
	adds r2, #57
	adds r1, r6, r2
	subs r0, r3, #1
	ldrb r3, [r1]
	movs r2, #1
	eors r3, r2
	strb r3, [r1]
	movs r3, #32
	ands r3, r0
	movs r5, #0
	cmp r3, #0
	beq .L_080d0ae2
	movs r5, #15
.L_080d0ae2:
	movs r3, #31
	ands r3, r0
	lsls r0, r3, #1
	ldr r3, .L_080d0b70
	movs r7, #63
	movs r4, #0
	mov r12, r3
	mov lr, r7
.L_080d0af2:
	mov r1, lr
	adds r3, r0, #0
	ands r3, r1
	mov r7, r12
	ldrb r2, [r7, r3]
	movs r7, #161
	lsrs r3, r2, #1
	adds r3, r6, r3
	lsls r7, r7, #3
	adds r1, r3, r7
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080d0b1c
	ldrb r3, [r1]
	movs r2, #15
	ands r2, r3
	lsls r3, r5, #4
	orrs r2, r3
	strb r2, [r1]
	b .L_080d0b26
.L_080d0b1c:
	ldrb r2, [r1]
	movs r3, #240
	ands r3, r2
	orrs r3, r5
	strb r3, [r1]
.L_080d0b26:
	adds r4, #1
	adds r0, #1
	cmp r4, #1
	bls .L_080d0af2
	ldr r1, .L_080d0b74
	ldr r0, .L_080d0b78
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_080d0b62
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	strh r2, [r1]
	movs r1, #161
	lsls r1, r1, #3
	adds r3, #4
	adds r2, r6, r1
	stmia r3!, {r2}
	movs r2, #192
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_080d0b62:
	strh r4, [r0]
.L_080d0b64:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d0b68:
	.4byte Func_080d0a28
.L_080d0b6c:
	.4byte IwramSignedDivide
.L_080d0b70:
	.4byte Data_080f0206
.L_080d0b74:
	.4byte Data_020038e0
.L_080d0b78:
	.4byte 0x04000208
