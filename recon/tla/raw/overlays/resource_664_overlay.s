.syntax unified
	.thumb
	.section .text.x02008216,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008266,"ax",%progbits
	.2byte 0x0000
	.section .text.x020082f2,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200868e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008908,"ax",%progbits
	.global Func_02000908
	.thumb_func
Func_02000908:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_02008980
	adds r7, r0, #0
.L_0200891c:
	ldrh r0, [r7]
	bl Actor_GetById
	movs r3, #4
	ldrsh r2, [r7, r3]
	movs r1, #0
	mov r8, r2
	adds r6, r0, #0
	movs r3, #2
	ldrsh r5, [r7, r3]
	bl Actor_SetSingleChildFlags
	mov r2, r8
	lsls r0, r2, #16
	lsrs r0, r0, #16
	bl GameFlag_Test
	lsls r5, r5, #16
	lsrs r5, r5, #16
	lsls r3, r5, #1
	adds r5, r5, r3
	adds r5, r5, r0
	adds r1, r5, #0
	adds r0, r6, #0
	bl Func_02005d5c
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r1, r6, #0
	adds r1, #89
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	adds r3, r6, #0
	adds r3, #100
	mov r2, r8
	strh r2, [r3]
	adds r0, r6, #0
	adds r7, #6
	bl Func_020006cc
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200891c
.L_02008980:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008d48,"ax",%progbits
	.global Func_02000d48
	.thumb_func
Func_02000d48:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r2, .L_02008e38
	movs r3, #133
	mov r10, r2
	lsls r3, r3, #2
	add r3, r10
	adds r6, r0, #0
	ldr r0, [r3]
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r7, r0, #0
	mov r8, r3
	movs r3, #179
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008db6
	movs r3, #173
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008db6
	movs r3, #175
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008db6
	movs r3, #180
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r5, [r3, r2]
	cmp r5, #0
	bne .L_02008db6
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r10, r3
	mov r2, r10
	ldrb r3, [r2]
	cmp r3, #5
	bne .L_02008dc0
.L_02008db6:
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetAnimationSpeed
	b .L_02008f24
.L_02008dc0:
	adds r0, r6, #0
	movs r1, #16
	bl Actor_SetAnimationSpeed
	ldr r2, [r6, #16]
	ldr r3, [r6, #12]
	ldr r1, [r6, #8]
	subs r2, r2, r3
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #2
	bl Func_02000690
	asrs r0, r0, #8
	cmp r0, #212
	bne .L_02008e0c
	adds r3, r6, #0
	movs r1, #142
	adds r3, #99
	lsls r1, r1, #1
	strb r5, [r3]
	adds r0, r6, #0
	adds r1, #255
	str r5, [r6, #108]
	bl Func_02005f8c
	ldrh r3, [r6, #6]
	movs r2, #192
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_02008e02
	ldr r1, .L_02008e3c
	b .L_02008e04
.L_02008e02:
	ldr r1, .L_02008e40
.L_02008e04:
	adds r0, r6, #0
	bl Func_02005d6c
	b .L_02008f24
.L_02008e0c:
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_02008eb6
	mov r2, r10
	ldrb r3, [r2]
	adds r0, r7, #0
	adds r1, r6, #0
	movs r5, #0
	adds r0, #8
	adds r1, #8
	cmp r3, #2
	bne .L_02008e44
	bl Func_02000d18
	cmp r0, #16
	bgt .L_02008e5a
	ldr r2, [r7, #16]
	ldr r3, [r6, #16]
	b .L_02008e50
.L_02008e38:
	.4byte gMainRoutineWork
.L_02008e3c:
	.4byte gData_020061ac
.L_02008e40:
	.4byte gData_02006194
.L_02008e44:
	bl Func_02000cec
	cmp r0, #8
	bgt .L_02008e5a
	ldr r2, [r7, #12]
	ldr r3, [r6, #12]
.L_02008e50:
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02008e5a
	movs r5, #1
.L_02008e5a:
	cmp r5, #0
	beq .L_02008eb6
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008eb6
	ldrh r3, [r6, #6]
	str r6, [r7, #104]
	strh r3, [r7, #6]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	ldr r0, [r7, #80]
	ldr r3, [r6, #80]
	ldrb r1, [r0, #9]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	movs r2, #181
	lsls r2, r2, #1
	add r2, r8
	strb r3, [r0, #9]
	movs r3, #200
	strh r3, [r2]
	ldr r3, .L_02008edc
	movs r2, #128
	ldr r4, .L_02008ed8
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	strb r4, [r3]
	mov r2, r9
	movs r3, #1
	strb r3, [r2]
	adds r2, r6, #0
	adds r2, #99
	strb r3, [r2]
.L_02008eb6:
	ldrh r3, [r6, #6]
	movs r2, #192
	lsls r2, r2, #8
	adds r0, r3, #0
	cmp r3, r2
	bne .L_02008efc
	bl Math_Cosine
	movs r1, #192
	lsls r1, r1, #9
	ldr r5, .L_02008ee0
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r3, r3, r0
	b .L_02008ee4
	.2byte 0x0000
.L_02008ed8:
	.4byte 0x00000000
.L_02008edc:
	.4byte gMainRoutineWork
.L_02008ee0:
	.4byte IwramMulQ16
.L_02008ee4:
	ldrh r0, [r6, #6]
	str r3, [r6, #8]
	bl Math_Sine
	movs r1, #192
	lsls r1, r1, #9
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #12]
	subs r3, r3, r0
	str r3, [r6, #12]
	b .L_02008f24
.L_02008efc:
	bl Math_Cosine
	movs r1, #192
	lsls r1, r1, #9
	ldr r5, .L_02008f30
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r3, r3, r0
	ldrh r0, [r6, #6]
	str r3, [r6, #8]
	bl Math_Sine
	movs r1, #192
	lsls r1, r1, #9
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #16]
	adds r3, r3, r0
	str r3, [r6, #16]
.L_02008f24:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008f30:
	.4byte IwramMulQ16
	.section .text.x02008f34,"ax",%progbits
	.global Func_02000f34
	.thumb_func
Func_02000f34:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	sub sp, #68
	adds r7, r0, #0
	cmp r3, #0
	beq .L_02008f56
	b .L_0200906c
.L_02008f56:
	movs r1, #173
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_02008f66
	b .L_0200906c
.L_02008f66:
	movs r1, #175
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_02008f76
	b .L_0200906c
.L_02008f76:
	movs r1, #180
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200906c
	movs r3, #100
	adds r3, r3, r7
	mov r10, r3
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #240
	bne .L_02009062
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_Test
	add r2, sp, #16
	add r6, sp, #56
	mov r8, r2
	cmp r0, #0
	bne .L_02008fac
	adds r0, r7, #0
	movs r1, #202
	bl Func_02005f8c
.L_02008fac:
	ldrh r3, [r7, #6]
	movs r1, #192
	lsls r1, r1, #8
	cmp r3, r1
	bne .L_02008fbe
	ldr r3, [r7, #8]
	str r3, [r6]
	ldr r3, [r7, #16]
	b .L_02008fea
.L_02008fbe:
	ldrh r0, [r7, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r5, .L_02009078
	lsls r0, r0, #12
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r7, #8]
	adds r3, r3, r0
	str r3, [r6]
	ldrh r0, [r7, #6]
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #12
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r7, #16]
	adds r3, r3, r0
.L_02008fea:
	str r3, [r6, #8]
	add r6, sp, #56
	movs r0, #140
	ldr r2, [r7, #12]
	ldr r3, [r6, #8]
	ldr r1, [r6]
	lsls r0, r0, #1
	bl Import_ActorCreate
	movs r1, #2
	adds r5, r0, #0
	bl Func_02005d5c
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	adds r3, r5, #0
	movs r4, #0
	adds r3, #85
	strb r4, [r3]
	adds r3, #13
	strb r4, [r3]
	adds r3, #1
	strb r4, [r3]
	ldrh r3, [r7, #6]
	mov r1, r8
	strh r3, [r5, #6]
	ldr r3, .L_0200907c
	mov r2, r10
	str r3, [r5, #108]
	movs r3, #1
	str r7, [r5, #104]
	strh r4, [r2]
	str r3, [r1]
	movs r3, #7
	str r3, [r1, #4]
	ldr r3, .L_02009080
	ldr r2, [r6, #8]
	ldr r0, [r6]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	ldr r1, [r7, #12]
	str r3, [sp, #8]
	mov r3, r8
	str r3, [sp, #12]
	movs r3, #0
	str r4, [sp, #0]
	str r4, [sp, #4]
	bl Func_020002f4
.L_02009062:
	adds r2, r7, #0
	adds r2, #100
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_0200906c:
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009078:
	.4byte IwramMulQ16
.L_0200907c:
	.4byte Func_02000d48
.L_02009080:
	.4byte 0xfffa0000
	.section .text.x02009290,"ax",%progbits
	.global Func_02001290
	.thumb_func
Func_02001290:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	sub sp, #68
	mov r8, r0
	cmp r3, #0
	beq .L_020092b2
	b .L_02009408
.L_020092b2:
	movs r1, #173
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_020092c2
	b .L_02009408
.L_020092c2:
	movs r1, #175
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_020092d2
	b .L_02009408
.L_020092d2:
	movs r1, #180
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_020092e2
	b .L_02009408
.L_020092e2:
	movs r5, #0
.L_020092e4:
	adds r0, r5, #0
	adds r0, #13
	adds r5, #1
	bl Actor_GetById
	cmp r5, #3
	bne .L_020092e4
	movs r3, #100
	add r3, r8
	mov r10, r3
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #240
	beq .L_02009302
	b .L_020093fe
.L_02009302:
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_Test
	movs r7, #0
	add r6, sp, #56
	cmp r0, #0
	bne .L_0200931a
	mov r0, r8
	movs r1, #202
	bl Func_02005f8c
.L_0200931a:
	mov r2, r8
	ldrh r3, [r2, #6]
	movs r1, #192
	lsls r1, r1, #8
	cmp r3, r1
	bne .L_0200932e
	ldr r3, [r2, #8]
	str r3, [r6]
	ldr r3, [r2, #16]
	b .L_02009360
.L_0200932e:
	mov r2, r8
	ldrh r0, [r2, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r5, .L_02009414
	lsls r0, r0, #12
	mov lr, r5
	.2byte 0xf800
	mov r1, r8
	ldr r3, [r1, #8]
	adds r3, r3, r0
	str r3, [r6]
	ldrh r0, [r1, #6]
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #12
	mov lr, r5
	.2byte 0xf800
	mov r2, r8
	ldr r3, [r2, #16]
	adds r3, r3, r0
.L_02009360:
	str r3, [r6, #8]
	movs r5, #0
	b .L_02009368
.L_02009366:
	adds r5, #1
.L_02009368:
	cmp r5, #3
	beq .L_0200937c
	adds r0, r5, #0
	adds r0, #13
	bl Actor_GetById
	adds r7, r0, #0
	ldr r3, [r7, #108]
	cmp r3, #0
	bne .L_02009366
.L_0200937c:
	add r6, sp, #56
	ldr r3, [r6]
	mov r1, r8
	str r3, [r7, #8]
	adds r0, r7, #0
	ldr r3, [r1, #12]
	movs r1, #2
	str r3, [r7, #12]
	movs r5, #0
	ldr r3, [r6, #8]
	str r3, [r7, #16]
	bl Func_02005d5c
	adds r0, r7, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r1, [r7, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #32
	orrs r3, r2
	strb r3, [r1]
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	adds r3, #13
	strb r5, [r3]
	adds r3, #1
	strb r5, [r3]
	mov r2, r8
	ldrh r3, [r2, #6]
	add r4, sp, #16
	strh r3, [r7, #6]
	ldr r3, .L_02009418
	str r2, [r7, #104]
	str r3, [r7, #108]
	mov r3, r10
	strh r5, [r3]
	movs r3, #1
	str r3, [r4]
	movs r3, #7
	str r3, [r4, #4]
	ldr r3, .L_0200941c
	ldr r1, [r2, #12]
	ldr r2, [r6, #8]
	ldr r0, [r6]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #8]
	movs r3, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	str r4, [sp, #12]
	bl Func_020002f4
.L_020093fe:
	mov r2, r8
	adds r2, #100
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02009408:
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009414:
	.4byte IwramMulQ16
.L_02009418:
	.4byte Func_020010b4
.L_0200941c:
	.4byte 0xfffa0000
	.section .text.x02009458,"ax",%progbits
	.global Func_02001458
	.thumb_func
Func_02001458:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_020095fc
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r5, #192
	lsls r5, r5, #18
	adds r6, r0, #0
	ldr r0, [r3]
	ldr r7, [r5, #108]
	bl Actor_GetById
	adds r5, #128
	ldr r3, [r5]
	movs r2, #168
	lsls r2, r2, #6
	adds r2, #1
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r8, r0
	cmp r3, #0
	beq .L_02009492
	b .L_020095f0
.L_02009492:
	movs r1, #217
	lsls r1, r1, #1
	adds r3, r7, r1
	ldrh r3, [r3]
	cmp r3, #0
	bne .L_020094a0
	b .L_020095f0
.L_020094a0:
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_020094d2
	subs r2, #12
	adds r3, r7, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_020094d2
	adds r2, #4
	adds r3, r7, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_020094d2
	adds r2, #10
	adds r3, r7, r2
	movs r1, #0
	ldrsh r5, [r3, r1]
	cmp r5, #0
	beq .L_020094dc
.L_020094d2:
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetAnimationSpeed
	b .L_020095f0
.L_020094dc:
	movs r1, #16
	adds r0, r6, #0
	bl Actor_SetAnimationSpeed
	movs r2, #100
	adds r2, r2, r6
	movs r1, #0
	ldrsh r3, [r2, r1]
	mov r9, r2
	cmp r3, #0
	bne .L_02009506
	ldr r3, [r6, #8]
	cmp r3, #0
	bne .L_02009500
	adds r0, r6, #0
	bl Func_02005d7c
	b .L_020095f0
.L_02009500:
	str r5, [r6, #16]
	str r5, [r6, #8]
	b .L_020095f0
.L_02009506:
	movs r2, #8
	adds r2, r2, r6
	mov r0, r8
	mov r10, r2
	adds r0, #8
	mov r1, r10
	bl Func_02000cec
	cmp r0, #8
	bgt .L_02009546
	mov r1, r8
	movs r3, #14
	ldrsh r2, [r1, r3]
	movs r1, #14
	ldrsh r3, [r6, r1]
	cmp r2, r3
	bne .L_02009546
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #201
	strh r3, [r2]
	str r5, [r6, #76]
	ldr r3, [r6, #48]
	mov r1, r8
	negs r3, r3
	asrs r3, r3, #1
	str r3, [r6, #48]
	ldr r2, .L_02009600
	ldr r3, [r1, #16]
	adds r3, r3, r2
	str r3, [r6, #16]
.L_02009546:
	ldr r3, .L_020095fc
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009604
	cmp r2, r3
	bne .L_02009582
	movs r0, #8
	bl Actor_GetById
	mov r1, r10
	adds r5, r0, #0
	adds r0, #8
	bl Func_02000cec
	cmp r0, #8
	bgt .L_020095be
	ldr r3, [r6, #48]
	ldr r2, .L_02009600
	negs r3, r3
	asrs r3, r3, #1
	str r3, [r6, #48]
	movs r3, #0
	str r3, [r6, #76]
	ldr r3, [r5, #16]
	adds r3, r3, r2
	str r3, [r6, #16]
	b .L_020095be
.L_02009582:
	ldr r3, .L_02009608
	cmp r2, r3
	bne .L_020095be
	movs r5, #0
.L_0200958a:
	adds r0, r5, #0
	adds r0, #12
	bl Actor_GetById
	mov r1, r10
	adds r7, r0, #0
	adds r0, #8
	bl Func_02000cec
	cmp r0, #8
	bgt .L_020095b4
	ldr r3, [r6, #48]
	ldr r1, .L_02009600
	negs r3, r3
	asrs r3, r3, #1
	str r3, [r6, #48]
	movs r3, #0
	str r3, [r6, #76]
	ldr r3, [r7, #16]
	adds r3, r3, r1
	str r3, [r6, #16]
.L_020095b4:
	adds r3, r5, #1
	lsls r3, r3, #24
	lsrs r5, r3, #24
	cmp r5, #3
	bne .L_0200958a
.L_020095be:
	mov r2, r9
	ldrh r3, [r2]
	mov r1, r9
	subs r3, #1
	strh r3, [r1]
	ldr r1, [r6, #48]
	ldr r3, [r6, #16]
	ldr r2, [r6, #52]
	adds r3, r3, r1
	str r3, [r6, #16]
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #8]
	ldr r3, [r6, #76]
	movs r2, #192
	subs r1, r1, r3
	ldr r3, [r6, #24]
	lsls r2, r2, #4
	adds r2, #204
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	str r1, [r6, #48]
	adds r3, r3, r2
	str r3, [r6, #28]
.L_020095f0:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020095fc:
	.4byte gMainRoutineWork
.L_02009600:
	.4byte 0xfff80000
.L_02009604:
	.4byte 0x00000057
.L_02009608:
	.4byte 0x0000005c
	.section .text.x02009690,"ax",%progbits
	.balign 4
	.global Func_02001690
	.thumb_func
Func_02001690:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	beq .L_020096a8
	b .L_020097da
.L_020096a8:
	subs r1, #12
	adds r3, r2, r1
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	beq .L_020096b6
	b .L_020097da
.L_020096b6:
	adds r1, #4
	adds r3, r2, r1
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	beq .L_020096c4
	b .L_020097da
.L_020096c4:
	adds r1, #10
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_020096d2
	b .L_020097da
.L_020096d2:
	ldr r3, .L_020097dc
	ldr r0, [r3]
	movs r3, #6
	ands r3, r0
	cmp r3, #0
	bne .L_020097da
	ldr r2, .L_020097e0
	movs r4, #240
	lsls r4, r4, #1
	adds r3, r2, r4
	movs r4, #0
	ldrsh r1, [r3, r4]
	ldr r3, .L_020097e4
	cmp r1, r3
	bne .L_02009716
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_0200970a
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #155
	bl GameFlag_Test
	movs r3, #188
	lsls r3, r3, #17
	cmp r0, #0
	beq .L_0200970e
.L_0200970a:
	movs r3, #140
	lsls r3, r3, #17
.L_0200970e:
	movs r2, #154
	lsls r2, r2, #18
	adds r0, r3, #0
	b .L_020097b6
.L_02009716:
	ldr r3, .L_020097e8
	cmp r1, r3
	bne .L_020097da
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_02009776
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r2, r1
	ldr r0, [r5]
	bl Actor_GetById
	ldr r3, [r0, #16]
	asrs r3, r3, #19
	cmp r3, #41
	ble .L_02009766
	ldr r0, [r5]
	bl Actor_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #19
	cmp r3, #44
	bgt .L_0200974e
.L_02009746:
	movs r0, #168
	lsls r0, r0, #16
	ldr r2, .L_020097ec
	b .L_020097b6
.L_0200974e:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009798
	movs r0, #252
	movs r2, #228
	lsls r0, r0, #17
	lsls r2, r2, #17
	b .L_020097b6
.L_02009766:
	movs r0, #154
	movs r2, #136
	lsls r0, r0, #18
	ldr r1, .L_020097f0
	lsls r2, r2, #16
	bl Func_0200160c
	b .L_020097da
.L_02009776:
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r2, r3
	ldr r0, [r5]
	bl Actor_GetById
	ldr r3, [r0, #16]
	asrs r3, r3, #19
	cmp r3, #41
	ble .L_020097be
	ldr r0, [r5]
	bl Actor_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #19
	cmp r3, #49
	ble .L_020097a2
.L_02009798:
	movs r0, #138
	movs r2, #228
	lsls r0, r0, #18
	lsls r2, r2, #17
	b .L_020097b6
.L_020097a2:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009746
	movs r0, #132
	movs r2, #150
	lsls r0, r0, #17
	lsls r2, r2, #18
.L_020097b6:
	movs r1, #0
	bl Func_0200160c
	b .L_020097da
.L_020097be:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009766
	movs r0, #162
	movs r2, #136
	lsls r0, r0, #18
	ldr r1, .L_020097f0
	lsls r2, r2, #16
	bl Func_0200160c
.L_020097da:
	pop {r5, pc}
.L_020097dc:
	.4byte gFrameCount
.L_020097e0:
	.4byte gMainRoutineWork
.L_020097e4:
	.4byte 0x00000057
.L_020097e8:
	.4byte 0x0000005c
.L_020097ec:
	.4byte 0x025b0000
.L_020097f0:
	.4byte 0xffe00000
	.section .text.x02009828,"ax",%progbits
	.global Func_02001828
	.thumb_func
Func_02001828:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009bdc
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #132
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r1, #0
	str r1, [sp, #16]
	mov r8, r3
	mov r10, r0
	mov r9, r1
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	movs r0, #207
	bl Func_02005f94
	mov r4, r10
	mov r2, r10
	movs r0, #140
	ldr r3, [r4, #16]
	ldr r1, [r2, #8]
	lsls r0, r0, #1
	ldr r2, [r2, #12]
	bl Import_ActorCreate
	movs r1, #2
	adds r7, r0, #0
	bl Func_02005d5c
	adds r0, r7, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r3, .L_02009be0
	add r5, sp, #16
	str r3, [r7, #24]
	movs r3, #204
	lsls r3, r3, #6
	ldrb r5, [r5]
	adds r3, #51
	str r3, [r7, #28]
	adds r3, r7, #0
	adds r3, #85
	mov r1, r8
	strb r5, [r3]
	ldr r0, [r1, #20]
	ldr r4, [r7, #80]
	ldr r3, [r0, #80]
	ldrb r1, [r4, #9]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r4, #9]
	ldr r4, .L_02009be4
	ldr r3, [r0, #16]
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	adds r3, r3, r4
	adds r0, r7, #0
	bl Func_02005d8c
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r5, #166
	ldr r3, [r7, #28]
	lsls r5, r5, #9
	adds r5, #203
	str r0, [r7, #20]
	cmp r3, r5
	bgt .L_020098fe
.L_020098e2:
	movs r0, #200
	lsls r0, r0, #5
	adds r0, #153
	adds r3, r3, r0
	str r3, [r7, #28]
	movs r0, #1
	bl FieldEvent_Wait
	movs r1, #166
	ldr r3, [r7, #28]
	lsls r1, r1, #9
	adds r1, #203
	cmp r3, r1
	ble .L_020098e2
.L_020098fe:
	adds r0, r7, #0
	bl Func_02005d94
	mov r2, r8
	ldr r3, [r2, #20]
	ldr r4, .L_02009be4
	ldr r3, [r3, #16]
	movs r5, #230
	adds r3, r3, r4
	str r3, [r7, #16]
	b .L_020099d2
.L_02009914:
	ldr r0, .L_02009be8
	movs r3, #3
	ldr r6, [r0]
	mov r11, r0
	ands r6, r3
	cmp r6, #0
	bne .L_02009996
	mov r1, r8
	ldr r3, [r1, #20]
	movs r0, #168
	ldr r1, [r3, #8]
	ldr r2, [r3, #12]
	lsls r0, r0, #2
	ldr r3, [r3, #16]
	bl Import_ActorCreate
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	movs r0, #0
	ldr r2, [r5, #16]
	ldr r1, [r5, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #8]
	str r0, [r5, #20]
	str r3, [r5, #68]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r5, #72]
	ldr r3, [r5, #16]
	str r3, [r5, #76]
	bl Random16Far
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r0
	str r3, [r5, #48]
	adds r0, r5, #0
	movs r1, #7
	bl Func_02005d5c
	adds r0, r5, #0
	ldr r1, .L_02009bec
	bl Func_02005d6c
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #2
	bl Actor_SetPartAttribute
	ldr r3, .L_02009bf0
	str r3, [r5, #108]
.L_02009996:
	mov r2, r11
	ldr r3, [r2]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_020099c0
	ldr r3, [r7, #24]
	movs r2, #200
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	str r3, [r7, #24]
	ldr r3, [r7, #28]
	movs r1, #15
	adds r3, r3, r2
	str r3, [r7, #28]
	mov r3, r8
	ldr r0, [r3, #20]
	bl Actor_SetPartAttribute
	b .L_020099ca
.L_020099c0:
	mov r4, r8
	ldr r0, [r4, #20]
	movs r1, #4
	bl Actor_SetPartAttribute
.L_020099ca:
	movs r0, #1
	bl FieldEvent_Wait
	movs r5, #230
.L_020099d2:
	ldr r3, [r7, #28]
	lsls r5, r5, #9
	adds r5, #203
	cmp r3, r5
	ble .L_02009914
	mov r1, r8
	ldr r0, [r1, #20]
	movs r1, #0
	bl Actor_SetPartAttribute
	movs r0, #136
	bl Func_02005f94
	mov r3, r8
	ldr r2, [r3, #20]
	mov r4, r10
	ldr r3, [r2, #8]
	asrs r3, r3, #20
	str r3, [sp, #24]
	ldr r3, [r2, #16]
	ldr r5, [sp, #24]
	asrs r3, r3, #20
	str r3, [sp, #28]
	ldr r3, [r4, #8]
	asrs r3, r3, #20
	cmp r3, r5
	bge .L_02009a12
	movs r0, #1
	movs r1, #0
	str r0, [sp, #32]
	str r1, [sp, #36]
	b .L_02009a40
.L_02009a12:
	ldr r2, [sp, #24]
	cmp r3, r2
	ble .L_02009a20
	movs r3, #1
	negs r3, r3
	movs r4, #0
	b .L_02009a3c
.L_02009a20:
	mov r5, r10
	ldr r3, [r5, #16]
	ldr r0, [sp, #28]
	asrs r3, r3, #20
	cmp r3, r0
	bge .L_02009a36
	movs r1, #0
	movs r2, #1
	str r1, [sp, #32]
	str r2, [sp, #36]
	b .L_02009a40
.L_02009a36:
	movs r4, #1
	movs r3, #0
	negs r4, r4
.L_02009a3c:
	str r3, [sp, #32]
	str r4, [sp, #36]
.L_02009a40:
	ldr r1, [sp, #24]
	ldr r2, [sp, #28]
	movs r0, #2
	bl Func_02000690
	ldr r3, [sp, #28]
	adds r5, r0, #0
	ldr r0, [sp, #24]
	lsls r2, r3, #20
	lsls r1, r0, #20
	asrs r5, r5, #8
	movs r0, #2
	str r5, [sp, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #20]
	cmp r0, r3
	beq .L_02009a68
	movs r4, #0
	str r4, [sp, #16]
.L_02009a68:
	ldr r5, [sp, #24]
	ldr r0, [sp, #32]
	ldr r3, [sp, #28]
	ldr r4, [sp, #36]
	adds r1, r5, r0
	ldr r0, [sp, #16]
	adds r2, r3, r4
	movs r5, #2
	str r1, [sp, #24]
	str r2, [sp, #28]
	add r9, r5
	cmp r0, #0
	beq .L_02009a40
	movs r0, #2
	bl Func_02000690
	mov r3, r8
	ldr r5, [sp, #32]
	movs r1, #2
	ldr r2, [r3, #20]
	negs r1, r1
	add r9, r1
	mov r3, r9
	muls r3, r5
	movs r4, #10
	ldrsh r6, [r2, r4]
	ldr r1, [sp, #36]
	lsls r3, r3, #3
	adds r6, r6, r3
	mov r3, r9
	muls r3, r1
	ldr r1, [r7, #80]
	movs r0, #18
	ldrsh r5, [r2, r0]
	lsls r3, r3, #3
	ldrb r2, [r1, #9]
	adds r5, r5, r3
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	movs r3, #128
	adds r4, r5, #0
	lsls r3, r3, #11
	adds r2, r6, #0
	lsls r1, r2, #16
	str r3, [r7, #48]
	ldr r2, [r7, #12]
	str r3, [r7, #52]
	adds r0, r7, #0
	lsls r3, r4, #16
	str r6, [sp, #24]
	str r5, [sp, #28]
	bl Func_02005d8c
	movs r0, #160
	movs r1, #160
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl Func_02005ed4
	lsls r5, r5, #16
	lsls r6, r6, #16
	movs r1, #1
	adds r2, r5, #0
	adds r0, r6, #0
	negs r1, r1
	movs r3, #1
	bl Camera_MoveWithinMapBounds
	movs r5, #0
	mov r9, r5
	b .L_02009cea
.L_02009afe:
	ldr r3, .L_02009bdc
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009bf4
	cmp r2, r3
	bne .L_02009b6e
	movs r0, #8
	bl Actor_GetById
	adds r5, r0, #0
	ldr r2, [r7, #8]
	ldr r3, [r5, #8]
	asrs r2, r2, #19
	asrs r3, r3, #19
	cmp r2, r3
	beq .L_02009b26
	b .L_02009c6e
.L_02009b26:
	mov r2, r9
	cmp r2, #0
	beq .L_02009b2e
	b .L_02009c6e
.L_02009b2e:
	ldr r3, .L_02009bf8
	str r7, [r5, #104]
	str r3, [r5, #108]
	adds r3, r5, #0
	adds r3, #85
	strb r2, [r3]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r3, [r7, #80]
	ldr r0, [r5, #80]
	ldrb r3, [r3, #9]
	movs r1, #12
	ands r1, r3
	movs r4, #13
	ldrb r3, [r0, #9]
	negs r4, r4
	adds r2, r4, #0
	str r5, [r7, #104]
	movs r5, #156
	ands r3, r2
	lsls r5, r5, #14
	orrs r3, r1
	adds r5, #37
	strb r3, [r0, #9]
	b .L_02009c6c
.L_02009b6e:
	ldr r3, .L_02009bfc
	cmp r2, r3
	bne .L_02009c00
	movs r0, #9
	bl Actor_GetById
	adds r5, r0, #0
	ldr r2, [r7, #8]
	ldr r3, [r5, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02009c6e
	ldr r2, [r7, #16]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02009c6e
	mov r0, r9
	cmp r0, #0
	bne .L_02009c6e
	ldr r3, .L_02009bf8
	str r7, [r5, #104]
	str r3, [r5, #108]
	adds r3, r5, #0
	adds r3, #85
	strb r0, [r3]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r3, [r7, #80]
	ldr r0, [r5, #80]
	ldrb r3, [r3, #9]
	movs r1, #12
	ands r1, r3
	movs r4, #13
	ldrb r3, [r0, #9]
	negs r4, r4
	adds r2, r4, #0
	str r5, [r7, #104]
	movs r5, #164
	ands r3, r2
	lsls r5, r5, #14
	orrs r3, r1
	adds r5, #105
	strb r3, [r0, #9]
	b .L_02009c6c
.L_02009bdc:
	.4byte gMainRoutineWork
.L_02009be0:
	.4byte 0x0001b333
.L_02009be4:
	.4byte 0xffff0000
.L_02009be8:
	.4byte gFrameCount
.L_02009bec:
	.4byte gData_02005fd8
.L_02009bf0:
	.4byte Func_02002bd0
.L_02009bf4:
	.4byte 0x0000005a
.L_02009bf8:
	.4byte Func_020017f4
.L_02009bfc:
	.4byte 0x0000005c
.L_02009c00:
	ldr r3, .L_02009f68
	cmp r2, r3
	bne .L_02009c6e
	movs r0, #10
	bl Actor_GetById
	adds r5, r0, #0
	ldr r2, [r7, #8]
	ldr r3, [r5, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02009c6e
	ldr r2, [r7, #16]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02009c6e
	mov r0, r9
	cmp r0, #0
	bne .L_02009c6e
	ldr r3, .L_02009f6c
	str r7, [r5, #104]
	str r3, [r5, #108]
	adds r3, r5, #0
	adds r3, #85
	strb r0, [r3]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r3, [r7, #80]
	ldr r0, [r5, #80]
	ldrb r3, [r3, #9]
	movs r1, #12
	ands r1, r3
	movs r4, #13
	ldrb r3, [r0, #9]
	negs r4, r4
	adds r2, r4, #0
	ands r3, r2
	orrs r3, r1
	str r5, [r7, #104]
	movs r5, #240
	lsls r5, r5, #12
	strb r3, [r0, #9]
	adds r5, #51
.L_02009c6c:
	mov r9, r5
.L_02009c6e:
	ldr r0, .L_02009f70
	movs r3, #1
	mov r11, r0
	ldr r0, [r0]
	mov r10, r0
	mov r1, r10
	ands r1, r3
	mov r10, r1
	cmp r1, #0
	bne .L_02009ce4
	add r6, sp, #80
	str r3, [r6]
	movs r2, #128
	movs r3, #128
	lsls r3, r3, #2
	lsls r2, r2, #1
	adds r2, #255
	adds r3, #162
	ands r0, r2
	strh r3, [r6, #24]
	movs r3, #2
	str r3, [r6, #4]
	lsls r0, r0, #12
	mov r8, r2
	bl Math_Cosine
	add r5, sp, #120
	lsls r0, r0, #1
	str r0, [r5]
	bl Random16Far
	ldr r3, [r7, #12]
	movs r2, #31
	ands r2, r0
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5, #4]
	mov r3, r11
	ldr r0, [r3]
	mov r4, r8
	ands r0, r4
	lsls r0, r0, #12
	bl Math_Sine
	str r0, [r5, #8]
	ldr r4, [r7, #8]
	ldr r1, [r5, #4]
	ldr r3, [r5]
	ldr r2, [r7, #16]
	str r0, [sp, #4]
	movs r0, #152
	lsls r0, r0, #13
	mov r5, r10
	str r0, [sp, #8]
	adds r0, r4, #0
	str r5, [sp, #0]
	str r6, [sp, #12]
	bl Func_020002f4
.L_02009ce4:
	movs r0, #1
	bl FieldEvent_Wait
.L_02009cea:
	adds r0, r7, #0
	bl Func_02005dec
	cmp r0, #0
	bne .L_02009cf6
	b .L_02009afe
.L_02009cf6:
	mov r0, r9
	cmp r0, #0
	bne .L_02009cfe
	b .L_02009e0a
.L_02009cfe:
	ldr r5, [r7, #104]
	asrs r3, r0, #16
	lsls r3, r3, #19
	str r3, [r5, #8]
	lsls r3, r0, #19
	str r3, [r5, #16]
	movs r3, #128
	lsls r3, r3, #12
	movs r1, #85
	str r3, [r5, #40]
	adds r1, r1, r5
	movs r3, #2
	movs r6, #0
	str r6, [r5, #108]
	strb r3, [r1]
	mov r8, r1
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	movs r2, #249
	ands r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	movs r1, #1
	bl Actor_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #2
	bl Scene_SetActorSpritePriority
	ldr r3, .L_02009f74
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldr r3, .L_02009f78
	cmp r2, r3
	bne .L_02009d74
	ldr r3, .L_02009f7c
	movs r2, #18
	str r3, [r5, #20]
	movs r3, #19
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #19
	movs r1, #16
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r0, #244
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02009e0a
.L_02009d74:
	ldr r3, .L_02009f80
	cmp r2, r3
	bne .L_02009dba
	movs r3, #20
	movs r2, #38
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #38
	movs r2, #1
	movs r3, #1
	movs r0, #19
	bl Func_02005dac
	adds r0, r5, #0
	bl OverlayObject_WaitUntilIdle
	movs r0, #1
	bl wait
	adds r0, r5, #0
	bl OverlayObject_WaitUntilIdle
	mov r0, r8
	strb r6, [r0]
	movs r1, #208
	movs r0, #160
	str r6, [r5, #20]
	str r6, [r5, #12]
	lsls r0, r0, #17
	lsls r1, r1, #18
	movs r2, #0
	movs r3, #4
	bl Func_02005df4
	b .L_02009e0a
.L_02009dba:
	ldr r3, .L_02009f68
	cmp r2, r3
	bne .L_02009e0a
	adds r0, r5, #0
	bl OverlayObject_WaitUntilIdle
	movs r0, #1
	bl wait
	adds r0, r5, #0
	bl OverlayObject_WaitUntilIdle
	adds r0, r5, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	mov r1, r8
	strb r6, [r1]
	movs r0, #240
	movs r1, #204
	lsls r0, r0, #15
	str r6, [r5, #20]
	str r6, [r5, #12]
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #4
	bl Func_02005df4
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009e0a
	adds r0, r5, #0
	movs r1, #60
	movs r2, #0
	bl Func_02001084
.L_02009e0a:
	ldr r2, [sp, #16]
	cmp r2, #255
	bne .L_02009e18
	movs r0, #136
	bl Func_02005f94
	b .L_0200a08c
.L_02009e18:
	ldr r4, [sp, #32]
	movs r3, #10
	ldrsh r2, [r7, r3]
	lsls r3, r4, #1
	ldr r0, [sp, #36]
	adds r3, r3, r4
	lsls r3, r3, #4
	adds r3, r2, r3
	str r3, [sp, #24]
	movs r5, #18
	ldrsh r2, [r7, r5]
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	adds r3, r2, r3
	str r3, [sp, #28]
	ldr r2, [sp, #24]
	ldr r4, [sp, #28]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #48]
	str r3, [r7, #52]
	lsls r1, r2, #16
	adds r0, r7, #0
	lsls r3, r4, #16
	ldr r2, [r7, #12]
	bl Func_02005d8c
	ldr r5, [sp, #16]
	movs r0, #0
	subs r5, #30
	str r5, [sp, #16]
	str r0, [sp, #20]
	b .L_0200a078
.L_02009e5c:
	ldr r1, [sp, #16]
	cmp r1, #4
	ble .L_02009e64
	b .L_0200a08c
.L_02009e64:
	ldr r2, [sp, #20]
	cmp r2, #0
	bne .L_02009e74
	movs r0, #235
	bl Func_02005f94
	movs r3, #1
	str r3, [sp, #20]
.L_02009e74:
	ldr r4, .L_02009f70
	movs r5, #1
	ldr r3, [r4]
	mov r11, r4
	ands r3, r5
	mov r9, r5
	cmp r3, #0
	bne .L_02009eec
	add r0, sp, #40
	str r5, [r0]
	mov r8, r0
	bl Random16Far
	movs r3, #255
	lsls r3, r3, #8
	movs r1, #128
	lsls r1, r1, #9
	adds r3, #255
	mov r10, r1
	ands r3, r0
	mov r2, r8
	add r3, r10
	str r3, [r2, #12]
	str r3, [r2, #8]
	mov r3, r11
	ldr r0, [r3]
	movs r6, #128
	lsls r6, r6, #1
	adds r6, #255
	ands r0, r6
	lsls r0, r0, #12
	bl Math_Cosine
	mov r4, r10
	add r5, sp, #120
	lsls r0, r0, #1
	str r4, [r5, #4]
	str r0, [r5]
	mov r1, r11
	ldr r0, [r1]
	ands r0, r6
	lsls r0, r0, #12
	bl Math_Sine
	str r0, [r5, #8]
	ldr r6, [r7, #8]
	ldr r4, [r5, #4]
	ldr r1, [r7, #12]
	ldr r2, [r7, #16]
	ldr r3, [r5]
	str r0, [sp, #4]
	movs r0, #160
	lsls r0, r0, #12
	str r4, [sp, #0]
	str r0, [sp, #8]
	mov r4, r8
	adds r0, r6, #0
	str r4, [sp, #12]
	bl Func_020002f4
.L_02009eec:
	mov r5, r11
	ldr r3, [r5]
	movs r5, #3
	ands r3, r5
	cmp r3, #0
	beq .L_02009efa
	b .L_0200a072
.L_02009efa:
	ldr r3, .L_02009f74
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009f84
	cmp r2, r3
	bne .L_02009f88
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_SetBit
	ldr r3, [r7, #8]
	mov r4, r9
	asrs r3, r3, #20
	adds r3, #64
	str r3, [sp, #24]
	ldr r2, [sp, #24]
	ldr r3, [r7, #16]
	adds r2, #1
	asrs r3, r3, #20
	movs r0, #90
	movs r1, #25
	str r3, [sp, #28]
	str r5, [sp, #4]
	str r4, [sp, #0]
	bl Func_02005d9c
	movs r3, #2
	str r3, [sp, #4]
	mov r5, r9
	movs r0, #21
	movs r1, #88
	movs r2, #24
	movs r3, #89
	str r5, [sp, #0]
	bl Func_02005d9c
	movs r3, #89
	movs r5, #23
	str r3, [sp, #4]
	movs r0, #23
	movs r1, #92
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
	movs r3, #25
	str r3, [sp, #4]
	movs r0, #18
	movs r1, #33
	movs r2, #2
	b .L_0200a01a
.L_02009f68:
	.4byte 0x0000005d
.L_02009f6c:
	.4byte Func_020017f4
.L_02009f70:
	.4byte gFrameCount
.L_02009f74:
	.4byte gMainRoutineWork
.L_02009f78:
	.4byte 0x0000005a
.L_02009f7c:
	.4byte 0xffe00000
.L_02009f80:
	.4byte 0x0000005c
.L_02009f84:
	.4byte 0x00000058
.L_02009f88:
	ldr r3, .L_0200a274
	cmp r2, r3
	bne .L_0200a024
	ldr r3, [r7, #8]
	ldr r2, [r7, #12]
	asrs r6, r3, #20
	ldr r3, [r7, #16]
	ldr r0, [sp, #16]
	subs r3, r3, r2
	asrs r3, r3, #20
	adds r5, r3, #0
	adds r5, #64
	str r6, [sp, #24]
	str r5, [sp, #28]
	cmp r0, #0
	bne .L_02009fe0
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_SetBit
	mov r1, r9
	str r1, [sp, #0]
	str r1, [sp, #4]
	adds r3, r5, #0
	movs r0, #39
	movs r1, #82
	adds r2, r6, #0
	bl Func_02005d9c
	movs r3, #82
	movs r5, #38
	str r3, [sp, #4]
	movs r0, #39
	movs r1, #82
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
	movs r3, #18
	str r3, [sp, #4]
	movs r0, #38
	movs r1, #19
	b .L_0200a018
.L_02009fe0:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_SetBit
	mov r2, r9
	str r2, [sp, #0]
	str r2, [sp, #4]
	adds r3, r5, #0
	movs r0, #39
	movs r1, #82
	adds r2, r6, #0
	bl Func_02005d9c
	movs r3, #99
	movs r5, #38
	str r3, [sp, #4]
	movs r0, #38
	movs r1, #98
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
	movs r3, #37
	str r3, [sp, #4]
	movs r0, #38
	movs r1, #35
.L_0200a018:
	movs r2, #1
.L_0200a01a:
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
	b .L_0200a06e
.L_0200a024:
	ldr r3, .L_0200a278
	cmp r2, r3
	bne .L_0200a06e
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_SetBit
	ldr r2, [r7, #8]
	mov r3, r9
	asrs r2, r2, #20
	str r3, [sp, #0]
	movs r0, #15
	movs r1, #80
	movs r3, #77
	str r2, [sp, #24]
	str r5, [sp, #4]
	bl Func_02005d9c
	movs r3, #79
	movs r5, #15
	str r3, [sp, #4]
	movs r0, #15
	movs r1, #80
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
	movs r0, #17
	movs r1, #15
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005dac
.L_0200a06e:
	bl Func_02005d84
.L_0200a072:
	movs r0, #1
	bl FieldEvent_Wait
.L_0200a078:
	adds r0, r7, #0
	bl Func_02005dec
	cmp r0, #0
	bne .L_0200a084
	b .L_02009e5c
.L_0200a084:
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02005f94
.L_0200a08c:
	movs r3, #0
	str r3, [r7, #52]
	str r3, [r7, #48]
	str r3, [r7, #64]
	str r3, [r7, #60]
	str r3, [r7, #56]
	str r3, [sp, #20]
.L_0200a09a:
	ldr r3, [r7, #24]
	ldr r4, .L_0200a27c
	ldr r5, .L_0200a280
	adds r3, r3, r4
	str r3, [r7, #24]
	ldr r3, [r7, #28]
	movs r0, #224
	adds r3, r3, r5
	str r3, [r7, #28]
	ldr r3, [r7, #12]
	lsls r0, r0, #10
	adds r3, r3, r0
	str r3, [r7, #12]
	movs r0, #1
	bl FieldEvent_Wait
	ldr r1, [sp, #20]
	adds r1, #1
	str r1, [sp, #20]
	cmp r1, #8
	bne .L_0200a09a
	adds r0, r7, #0
	bl Func_02005d7c
	ldr r3, .L_0200a284
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldr r3, .L_0200a288
	cmp r2, r3
	beq .L_0200a0de
	b .L_0200a28c
.L_0200a0de:
	ldr r5, [sp, #16]
	cmp r5, #5
	bne .L_0200a1a8
	movs r0, #157
	bl Func_02005f94
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02005dcc
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02005dcc
	ldr r0, [sp, #16]
	movs r3, #85
	str r0, [sp, #0]
	movs r1, #64
	movs r2, #29
	movs r5, #7
	movs r0, #79
	str r5, [sp, #4]
	bl Func_02005d9c
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #157
	bl Func_02005f94
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02005dcc
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02005dcc
	ldr r1, [sp, #16]
	movs r0, #69
	str r1, [sp, #0]
	movs r2, #29
	movs r1, #64
	movs r3, #85
	str r5, [sp, #4]
	bl Func_02005d9c
	movs r3, #29
	movs r2, #21
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #20
	movs r2, #5
	movs r3, #5
	bl Func_02005dac
	movs r3, #31
	movs r2, #89
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #89
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r3, #33
	movs r2, #87
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #50
	movs r1, #86
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_SetBit
	b .L_0200a590
.L_0200a1a8:
	ldr r2, [sp, #16]
	cmp r2, #6
	beq .L_0200a1b0
	b .L_0200a590
.L_0200a1b0:
	movs r0, #157
	bl Func_02005f94
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02005dcc
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02005dcc
	movs r5, #5
	movs r3, #85
	movs r1, #64
	movs r2, #29
	movs r6, #7
	movs r0, #74
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005d9c
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #157
	bl Func_02005f94
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02005dcc
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02005dcc
	movs r0, #64
	movs r1, #64
	movs r2, #29
	movs r3, #85
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005d9c
	movs r3, #29
	movs r2, #21
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #52
	movs r1, #20
	movs r2, #5
	movs r3, #5
	bl Func_02005dac
	movs r3, #33
	movs r2, #87
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #33
	movs r1, #88
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r3, #31
	movs r2, #89
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #88
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_ClearBit
	b .L_0200a590
	.2byte 0x0000
.L_0200a274:
	.4byte 0x0000005b
.L_0200a278:
	.4byte 0x0000005d
.L_0200a27c:
	.4byte 0xffffd99a
.L_0200a280:
	.4byte 0xffffc000
.L_0200a284:
	.4byte gMainRoutineWork
.L_0200a288:
	.4byte 0x00000058
.L_0200a28c:
	ldr r3, .L_0200a5bc
	cmp r2, r3
	beq .L_0200a294
	b .L_0200a590
.L_0200a294:
	ldr r3, [sp, #16]
	cmp r3, #5
	beq .L_0200a29c
	b .L_0200a43e
.L_0200a29c:
	movs r0, #157
	bl Func_02005f94
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02005dcc
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02005dcc
	ldr r4, [sp, #16]
	movs r3, #79
	movs r0, #79
	movs r1, #64
	movs r2, #32
	movs r5, #7
	str r4, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005d9c
	movs r0, #8
	bl Actor_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #19
	cmp r3, #65
	bne .L_0200a3b6
	movs r0, #8
	bl Actor_GetById
	ldr r5, .L_0200a5c0
	ldr r3, [r0, #8]
	add r7, sp, #120
	adds r3, r3, r5
	str r3, [r7]
	movs r0, #8
	bl Actor_GetById
	ldr r3, [r0, #12]
	movs r0, #8
	str r3, [r7, #4]
	bl Actor_GetById
	ldr r3, [r0, #16]
	movs r0, #128
	lsls r0, r0, #12
	adds r3, r3, r0
	movs r1, #2
	str r3, [r7, #8]
	movs r0, #8
	bl Actor_SetSpritePriority
	movs r1, #0
	str r1, [sp, #20]
.L_0200a31c:
	add r6, sp, #40
	movs r3, #1
	str r3, [r6]
	bl Random16Far
	ldr r5, [r7]
	movs r3, #15
	ands r3, r0
	lsls r3, r3, #16
	adds r5, r5, r3
	bl Random16Far
	ldr r1, [r7, #4]
	movs r3, #31
	ands r3, r0
	lsls r3, r3, #16
	adds r1, r1, r3
	movs r3, #0
	ldr r2, [r7, #8]
	str r3, [sp, #0]
	ldr r3, .L_0200a5c4
	adds r0, r5, #0
	str r3, [sp, #4]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [sp, #8]
	movs r3, #0
	str r6, [sp, #12]
	bl Func_020002f4
	ldr r2, [sp, #20]
	adds r2, #1
	str r2, [sp, #20]
	cmp r2, #8
	bne .L_0200a31c
	movs r0, #30
	bl wait
	movs r0, #157
	bl Func_02005f94
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02005dcc
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02005dcc
	movs r3, #5
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #79
	movs r2, #32
	movs r0, #64
	movs r1, #64
	bl Func_02005d9c
	movs r0, #8
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	b .L_0200a590
.L_0200a3b6:
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #157
	bl Func_02005f94
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02005dcc
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02005dcc
	ldr r3, [sp, #16]
	movs r0, #69
	str r3, [sp, #0]
	movs r1, #64
	movs r2, #32
	movs r3, #79
	str r5, [sp, #4]
	bl Func_02005d9c
	movs r3, #32
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #15
	movs r2, #5
	movs r3, #5
	bl Func_02005dac
	movs r3, #34
	movs r2, #82
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #33
	movs r1, #82
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r3, #36
	movs r2, #81
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #52
	movs r1, #81
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
	b .L_0200a590
.L_0200a43e:
	ldr r4, [sp, #16]
	cmp r4, #6
	beq .L_0200a446
	b .L_0200a590
.L_0200a446:
	movs r0, #157
	bl Func_02005f94
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02005dcc
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02005dcc
	movs r0, #8
	bl Actor_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #19
	cmp r3, #65
	bne .L_0200a4f2
	movs r0, #8
	bl Actor_GetById
	ldr r5, .L_0200a5c0
	ldr r3, [r0, #8]
	add r7, sp, #120
	adds r3, r3, r5
	str r3, [r7]
	movs r0, #8
	bl Actor_GetById
	ldr r3, [r0, #12]
	movs r0, #8
	str r3, [r7, #4]
	bl Actor_GetById
	ldr r3, [r0, #16]
	movs r0, #0
	str r3, [r7, #8]
	str r0, [sp, #20]
.L_0200a4a4:
	add r6, sp, #40
	movs r3, #1
	str r3, [r6]
	bl Random16Far
	ldr r5, [r7]
	movs r3, #15
	ands r3, r0
	lsls r3, r3, #16
	adds r5, r5, r3
	bl Random16Far
	ldr r2, [r7, #8]
	movs r3, #31
	ands r3, r0
	lsls r3, r3, #16
	subs r2, r2, r3
	movs r3, #0
	ldr r1, [r7, #4]
	str r3, [sp, #0]
	ldr r3, .L_0200a5c4
	adds r0, r5, #0
	str r3, [sp, #4]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [sp, #8]
	movs r3, #0
	str r6, [sp, #12]
	bl Func_020002f4
	ldr r1, [sp, #20]
	adds r1, #1
	str r1, [sp, #20]
	cmp r1, #8
	bne .L_0200a4a4
	movs r0, #20
	bl wait
	b .L_0200a590
.L_0200a4f2:
	movs r3, #79
	movs r1, #64
	movs r2, #32
	movs r5, #5
	movs r6, #7
	movs r0, #79
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005d9c
	movs r0, #157
	bl Func_02005f94
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #157
	bl Func_02005f94
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02005dcc
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02005dcc
	movs r0, #64
	movs r1, #64
	movs r2, #32
	movs r3, #79
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005d9c
	movs r3, #32
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #21
	movs r2, #5
	movs r3, #5
	bl Func_02005dac
	movs r3, #36
	movs r2, #81
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #81
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r3, #34
	movs r2, #82
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #50
	movs r1, #82
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
.L_0200a590:
	movs r0, #30
	bl FieldEvent_Wait
	ldr r3, .L_0200a5c8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Func_02005eec
	bl Func_02005ee4
	bl Func_02005e1c
	add sp, #132
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a5bc:
	.4byte 0x0000005a
.L_0200a5c0:
	.4byte 0xfff80000
.L_0200a5c4:
	.4byte 0xffff8000
.L_0200a5c8:
	.4byte gMainRoutineWork
	.section .text.x0200a6c0,"ax",%progbits
	.balign 4
	.global Func_020026c0
	.thumb_func
Func_020026c0:
	push {r5, r6, r7, lr}
	sub sp, #8
	bl Func_02005f84
	bl Actor_GetById
	ldr r3, [r0, #8]
	asrs r6, r3, #20
	ldr r3, [r0, #16]
	asrs r5, r3, #20
	lsls r3, r6, #16
	adds r1, r3, r5
	ldr r3, [r0, #36]
	cmp r3, #0
	bne .L_0200a750
	ldr r3, [r0, #44]
	cmp r3, #0
	bne .L_0200a750
	ldr r2, .L_0200a754
	movs r0, #1
	ldr r3, [r2]
	negs r0, r0
	cmp r3, r0
	beq .L_0200a750
	cmp r1, r3
	beq .L_0200a750
	str r0, [r2]
	ldr r1, .L_0200a758
	movs r5, #255
	lsls r5, r5, #8
	adds r5, #255
	ldr r0, [r1]
	asrs r6, r3, #16
	ldr r1, .L_0200a75c
	ands r5, r3
	ldr r3, .L_0200a760
	ldr r1, [r1]
	ldr r2, [r3]
	ldr r3, .L_0200a764
	ldr r3, [r3]
	str r0, [sp, #0]
	str r1, [sp, #4]
	movs r0, #73
	movs r1, #72
	bl Func_02005db4
	ldr r3, .L_0200a768
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200a76c
	cmp r2, r3
	bne .L_0200a750
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #2
	adds r1, r6, #0
	adds r2, r5, #0
	ldr r7, [r3, #108]
	bl Func_02000690
	asrs r0, r0, #8
	cmp r0, #0
	beq .L_0200a750
	movs r1, #181
	adds r3, r0, #0
	lsls r1, r1, #1
	adds r3, #200
	adds r2, r7, r1
	strh r3, [r2]
.L_0200a750:
	add sp, #8
	pop {r5, r6, r7, pc}
.L_0200a754:
	.4byte gData_02006c0c
.L_0200a758:
	.4byte gData_020073e4
.L_0200a75c:
	.4byte gData_020073e8
.L_0200a760:
	.4byte gData_020073dc
.L_0200a764:
	.4byte gData_020073e0
.L_0200a768:
	.4byte gMainRoutineWork
.L_0200a76c:
	.4byte 0x00000059
	.section .text.x0200a790,"ax",%progbits
	.balign 4
	.global Func_02002790
	.thumb_func
Func_02002790:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	sub sp, #8
	bl Func_02005f84
	bl Actor_GetById
	ldr r3, [r0, #8]
	ldr r1, .L_0200a864
	asrs r4, r3, #20
	ldr r3, [r0, #12]
	asrs r5, r3, #20
	ldr r3, [r0, #16]
	asrs r0, r3, #20
	lsls r3, r4, #16
	adds r2, r3, r0
	ldr r3, [r1]
	cmp r2, r3
	beq .L_0200a856
	str r2, [r1]
	ldr r2, .L_0200a868
	movs r3, #1
	str r3, [r2]
	ldr r2, .L_0200a86c
	movs r3, #2
	str r3, [r2]
	ldr r3, .L_0200a870
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200a874
	ldr r1, .L_0200a878
	cmp r2, r3
	beq .L_0200a7ec
	ldr r3, .L_0200a87c
	cmp r2, r3
	beq .L_0200a7ec
	ldr r3, .L_0200a880
	cmp r2, r3
	bne .L_0200a7f6
.L_0200a7ec:
	ldr r2, .L_0200a884
	subs r3, r0, r5
	adds r3, #64
	str r4, [r1]
	b .L_0200a818
.L_0200a7f6:
	ldr r3, .L_0200a888
	cmp r2, r3
	beq .L_0200a80e
	ldr r3, .L_0200a88c
	cmp r2, r3
	beq .L_0200a80e
	ldr r3, .L_0200a890
	cmp r2, r3
	beq .L_0200a80e
	ldr r3, .L_0200a894
	cmp r2, r3
	bne .L_0200a81a
.L_0200a80e:
	adds r3, r4, #0
	ldr r2, .L_0200a884
	adds r3, #64
	str r3, [r1]
	subs r3, r0, r5
.L_0200a818:
	str r3, [r2]
.L_0200a81a:
	ldr r2, .L_0200a878
	ldr r3, .L_0200a884
	ldr r4, .L_0200a868
	ldr r6, .L_0200a86c
	ldr r0, [r2]
	ldr r1, [r3]
	mov r9, r2
	mov r10, r3
	ldr r2, [r4]
	ldr r3, [r6]
	mov r8, r4
	movs r5, #72
	movs r4, #73
	str r4, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005db4
	mov r4, r9
	ldr r1, [r4]
	mov r0, r8
	mov r4, r10
	ldr r2, [r0]
	ldr r0, [r4]
	ldr r3, [r6]
	str r1, [sp, #0]
	str r0, [sp, #4]
	movs r1, #72
	movs r0, #70
	bl Func_02005db4
.L_0200a856:
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a864:
	.4byte gData_02006c0c
.L_0200a868:
	.4byte gData_020073dc
.L_0200a86c:
	.4byte gData_020073e0
.L_0200a870:
	.4byte gMainRoutineWork
.L_0200a874:
	.4byte 0x00000055
.L_0200a878:
	.4byte gData_020073e4
.L_0200a87c:
	.4byte 0x00000056
.L_0200a880:
	.4byte 0x00000057
.L_0200a884:
	.4byte gData_020073e8
.L_0200a888:
	.4byte 0x00000059
.L_0200a88c:
	.4byte 0x0000005a
.L_0200a890:
	.4byte 0x0000005b
.L_0200a894:
	.4byte 0x0000005d
	.section .text.x0200a898,"ax",%progbits
	.global Func_02002898
	.thumb_func
Func_02002898:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	sub sp, #8
	bl Func_02005f84
	bl Actor_GetById
	ldr r3, [r0, #8]
	ldr r1, .L_0200a958
	asrs r4, r3, #20
	ldr r3, [r0, #12]
	asrs r5, r3, #20
	ldr r3, [r0, #16]
	asrs r0, r3, #20
	lsls r3, r4, #16
	adds r2, r3, r0
	ldr r3, [r1]
	cmp r2, r3
	beq .L_0200a94c
	str r2, [r1]
	ldr r2, .L_0200a95c
	movs r3, #1
	str r3, [r2]
	ldr r2, .L_0200a960
	movs r3, #2
	str r3, [r2]
	ldr r3, .L_0200a964
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200a968
	ldr r1, .L_0200a96c
	cmp r2, r3
	beq .L_0200a8ee
	ldr r3, .L_0200a970
	cmp r2, r3
	bne .L_0200a8f8
.L_0200a8ee:
	ldr r2, .L_0200a974
	subs r3, r0, r5
	adds r3, #64
	str r4, [r1]
	b .L_0200a90e
.L_0200a8f8:
	ldr r3, .L_0200a978
	cmp r2, r3
	beq .L_0200a904
	ldr r3, .L_0200a97c
	cmp r2, r3
	bne .L_0200a910
.L_0200a904:
	adds r3, r4, #0
	ldr r2, .L_0200a974
	adds r3, #64
	str r3, [r1]
	subs r3, r0, r5
.L_0200a90e:
	str r3, [r2]
.L_0200a910:
	ldr r2, .L_0200a96c
	ldr r3, .L_0200a974
	ldr r4, .L_0200a95c
	ldr r6, .L_0200a960
	ldr r0, [r2]
	ldr r1, [r3]
	mov r9, r2
	mov r10, r3
	ldr r2, [r4]
	ldr r3, [r6]
	mov r8, r4
	movs r5, #72
	movs r4, #73
	str r4, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005db4
	mov r4, r9
	ldr r1, [r4]
	mov r0, r8
	mov r4, r10
	ldr r2, [r0]
	ldr r0, [r4]
	ldr r3, [r6]
	str r1, [sp, #0]
	str r0, [sp, #4]
	movs r1, #72
	movs r0, #71
	bl Func_02005db4
.L_0200a94c:
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_0200a958:
	.4byte gData_02006c0c
.L_0200a95c:
	.4byte gData_020073dc
.L_0200a960:
	.4byte gData_020073e0
.L_0200a964:
	.4byte gMainRoutineWork
.L_0200a968:
	.4byte 0x00000055
.L_0200a96c:
	.4byte gData_020073e4
.L_0200a970:
	.4byte 0x00000056
.L_0200a974:
	.4byte gData_020073e8
.L_0200a978:
	.4byte 0x00000059
.L_0200a97c:
	.4byte 0x0000005a
	.section .text.x0200bf94,"ax",%progbits
	.global Func_02003f94
	.thumb_func
Func_02003f94:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r0, #102
	adds r0, r0, r7
	mov r8, r0
	adds r5, r7, #0
	adds r5, #100
	mov r1, r8
	ldrh r3, [r1]
	ldrh r0, [r5]
	adds r0, r0, r3
	strh r0, [r5]
	lsls r0, r0, #16
	asrs r0, r0, #16
	bl Math_Cosine
	ldr r1, [r7, #76]
	ldr r6, .L_0200c030
	mov lr, r6
	.2byte 0xf800
	str r0, [r7, #8]
	movs r2, #0
	ldrsh r0, [r5, r2]
	bl Math_Sine
	ldr r1, [r7, #76]
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r7, #8]
	ldr r2, [r7, #68]
	asrs r0, r0, #1
	adds r3, r3, r2
	str r3, [r7, #8]
	ldr r3, [r7, #72]
	subs r5, #1
	adds r0, r0, r3
	str r0, [r7, #16]
	ldrb r3, [r5]
	cmp r3, #141
	beq .L_0200c00c
	ldr r3, .L_0200c034
	movs r2, #1
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200c00c
	adds r3, r7, #0
	adds r3, #98
	ldrb r0, [r3]
	lsls r0, r0, #10
	bl Math_Sine
	ldrb r3, [r5]
	muls r3, r0
	str r3, [r7, #76]
	ldrb r3, [r5]
	adds r3, #10
	strb r3, [r5]
.L_0200c00c:
	mov r3, r8
	ldrh r2, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	movs r0, #0
	cmp r3, #0
	beq .L_0200c02a
	ldrb r3, [r5]
	cmp r3, #141
	bne .L_0200c028
	adds r3, r2, #0
	subs r3, #128
	mov r1, r8
	strh r3, [r1]
.L_0200c028:
	movs r0, #1
.L_0200c02a:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200c030:
	.4byte IwramMulQ16
.L_0200c034:
	.4byte gFrameCount
	.section .text.x0200c0b0,"ax",%progbits
	.global Func_020040b0
	.thumb_func
Func_020040b0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200c134
	sub sp, #68
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_0200c12a
	add r2, sp, #28
	str r3, [r2, #4]
	movs r3, #209
	lsls r3, r3, #1
	adds r3, #255
	strh r3, [r2, #24]
	movs r3, #1
	str r3, [r2]
	mov r8, r2
	bl Random16Far
	movs r6, #31
	mov r2, r10
	ldr r3, [r2, #8]
	ands r0, r6
	subs r0, #16
	lsls r0, r0, #16
	add r5, sp, #16
	adds r3, r3, r0
	str r3, [r5]
	bl Random16Far
	mov r3, r10
	ldr r1, [r3, #12]
	ands r0, r6
	lsls r0, r0, #16
	movs r2, #128
	adds r1, r1, r0
	lsls r2, r2, #12
	adds r1, r1, r2
	str r1, [r5, #4]
	ldr r0, [r5]
	ldr r2, [r3, #16]
	movs r3, #128
	lsls r3, r3, #11
	str r2, [r5, #8]
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #8
	str r3, [sp, #0]
	movs r3, #152
	lsls r3, r3, #13
	str r3, [sp, #8]
	mov r3, r8
	str r3, [sp, #12]
	movs r3, #0
	str r7, [sp, #4]
	bl Func_020002f4
.L_0200c12a:
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c134:
	.4byte gFrameCount
	.section .text.x0200ccc8,"ax",%progbits
	.balign 4
	.ifdef TLA_EDITION_JA
	.global Func_02004cc8
	.thumb_func
Func_02004cc8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_ja_0200ce88
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Actor_GetById
	adds r7, r0, #0
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	movs r0, #216
	movs r2, #136
	movs r3, #143
	lsls r3, r3, #1
	ldr r1, .L_ja_0200ce8c
	lsls r2, r2, #18
	lsls r0, r0, #16
	bl OverlayObject_PrepareObject
	movs r2, #85
	adds r2, r2, r7
	movs r5, #0
	strb r5, [r2]
	mov r11, r0
	movs r1, #0
	adds r0, r7, #0
	mov r10, r2
	bl Actor_SetSingleChildFlags
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r7, #48]
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	ldr r3, .L_ja_0200ce90
	movs r0, #140
	adds r2, r2, r3
	ldr r1, [r7, #8]
	ldr r3, [r7, #16]
	lsls r0, r0, #1
	bl Import_ActorCreate
	movs r1, #2
	adds r6, r0, #0
	bl Func_02005d5c
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r3, r6, #0
	adds r3, #85
	strb r5, [r3]
	movs r3, #179
	lsls r3, r3, #9
	adds r3, #102
	str r3, [r6, #48]
	str r3, [r6, #52]
	ldr r2, [r7, #12]
	movs r3, #160
	lsls r3, r3, #15
	ldr r1, [r7, #8]
	adds r2, r2, r3
	adds r0, r7, #0
	ldr r3, [r7, #16]
	bl Func_02005d8c
	ldr r2, [r6, #12]
	movs r3, #144
	lsls r3, r3, #15
	adds r2, r2, r3
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	adds r0, r6, #0
	bl Func_02005d8c
	movs r0, #137
	bl Func_02005f94
	bl Event_StartScreenTransition
	b .L_ja_0200cda0
.L_ja_0200cd80:
	ldr r3, .L_ja_0200ce94
	mov r1, r8
	ldr r2, [r3]
	ldrb r3, [r3]
	movs r0, #1
	lsls r3, r3, #12
	strh r3, [r7, #6]
	movs r3, #1
	ands r2, r3
	movs r3, #2
	lsls r3, r2
	ldrb r2, [r1]
	eors r3, r2
	strb r3, [r1]
	bl wait
.L_ja_0200cda0:
	adds r0, r7, #0
	bl Func_02005dec
	movs r2, #35
	adds r2, r2, r7
	mov r8, r2
	cmp r0, #0
	beq .L_ja_0200cd80
	movs r0, #144
	lsls r0, r0, #1
	bl Func_02005f94
	movs r0, #10
	bl FieldEvent_Wait
	ldr r5, .L_ja_0200ce88
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	ldr r3, .L_ja_0200ce98
	ldr r2, [r6, #16]
	mov r9, r3
	add r2, r9
	ldr r1, [r6, #8]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r1, #6
	str r0, [r7, #12]
	str r0, [r7, #20]
	adds r0, r7, #0
	bl Func_02005d5c
	movs r0, #6
	bl wait
	movs r0, #152
	bl Func_02005f94
	adds r0, r7, #0
	movs r1, #7
	bl Func_02005d5c
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r7, #40]
	mov r1, r10
	movs r3, #2
	strb r3, [r1]
	mov r2, r8
	movs r3, #1
	strb r3, [r2]
	ldr r3, [r7, #16]
	ldr r2, [r7, #12]
	add r3, r9
	ldr r1, [r7, #8]
	adds r0, r7, #0
	bl Func_02005d8c
	adds r0, r7, #0
	bl OverlayObject_WaitUntilIdle
	adds r0, r7, #0
	bl Func_02005d94
	ldr r0, [r5]
	movs r1, #1
	bl Actor_SetModeById
	movs r1, #5
	adds r0, r6, #0
	bl Func_02005d5c
	movs r0, #12
	bl wait
	adds r0, r6, #0
	bl Func_02005d7c
	mov r1, r10
	movs r3, #3
	strb r3, [r1]
	ldr r3, [r7, #12]
	movs r1, #1
	str r3, [r7, #20]
	adds r0, r7, #0
	bl Actor_SetSingleChildFlags
	mov r0, r11
	bl Func_02005d7c
	movs r0, #1
	bl wait
	bl Event_WaitTransitionFrames
	bl Func_02005e1c
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_ja_0200ce88:
	.4byte gMainRoutineWork
.L_ja_0200ce8c:
	.4byte 0xffe00000
.L_ja_0200ce90:
	.4byte 0xfff80000
.L_ja_0200ce94:
	.4byte gFrameCount
.L_ja_0200ce98:
	.4byte 0xfff00000
	.endif
	.section .text.x0200ceb0,"ax",%progbits
	.global Func_02004eb0
	.thumb_func
Func_02004eb0:
	push {r5, r6, r7, lr}
	movs r6, #0
.L_0200ceb4:
	movs r0, #168
	movs r1, #0
	movs r2, #0
	movs r3, #0
	lsls r0, r0, #2
	bl Import_ActorCreate
	adds r5, r0, #0
	ldr r1, [r5, #80]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r2, r5, #0
	strb r3, [r1, #9]
	adds r2, #99
	lsls r3, r6, #4
	strb r3, [r2]
	adds r3, r5, #0
	movs r7, #0
	adds r3, #85
	strb r7, [r3]
	movs r2, #128
	subs r3, #50
	strb r7, [r3]
	lsls r2, r2, #8
	lsls r3, r6, #12
	adds r3, r3, r2
	str r3, [r5, #48]
	str r3, [r5, #52]
	ldr r3, .L_0200cf38
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #2
	bl Actor_SetPartAttribute
	movs r2, #1
	ands r2, r6
	adds r1, r5, #0
	adds r0, r5, #0
	adds r1, #100
	adds r0, #102
	adds r6, #1
	cmp r2, #0
	beq .L_0200cf24
	movs r3, #212
	strh r3, [r1]
	strh r7, [r0]
	b .L_0200cf2a
.L_0200cf24:
	movs r3, #116
	strh r3, [r1]
	strh r2, [r0]
.L_0200cf2a:
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	cmp r6, #8
	bne .L_0200ceb4
	pop {r5, r6, r7, pc}
.L_0200cf38:
	.4byte Func_02000550
	.section .text.x0200d05a,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200d1f8,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_020051f8
	.thumb_func
Func_020051f8:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	ldr r5, .L_ja_0200d2b4
	lsls r2, r2, #2
	str r2, [r3]
	adds r2, #16
	adds r6, r5, r2
	ldr r0, [r6]
	sub sp, #8
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #244
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_ja_0200d2b8
	cmp r2, r3
	beq .L_ja_0200d234
	b .L_ja_0200d3fc
.L_ja_0200d234:
	bl Func_02002770
	movs r2, #241
	lsls r2, r2, #1
	adds r5, r5, r2
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #16
	bne .L_ja_0200d262
	ldr r0, [r6]
	bl Actor_GetById
	movs r3, #1
	adds r0, #34
	strb r3, [r0]
	movs r0, #8
	bl wait
	bl Func_02005d84
	movs r0, #1
	bl wait
.L_ja_0200d262:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #11
	ble .L_ja_0200d26e
	cmp r3, #16
	bne .L_ja_0200d28c
.L_ja_0200d26e:
	movs r1, #144
	ldr r0, .L_ja_0200d2bc
	lsls r1, r1, #3
	bl Task_Add
	ldr r3, .L_ja_0200d2ac
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_ja_0200d2b0
	subs r2, #2
	strh r3, [r2]
	bl Func_02004eb0
.L_ja_0200d28c:
	ldr r3, .L_ja_0200d2b4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #13
	ble .L_ja_0200d2d4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #142
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d2ce
	b .L_ja_0200d2c0
.L_ja_0200d2ac:
	.4byte 0x00001008
.L_ja_0200d2b0:
	.4byte 0x00003f10
.L_ja_0200d2b4:
	.4byte gMainRoutineWork
.L_ja_0200d2b8:
	.4byte 0x00000055
.L_ja_0200d2bc:
	.4byte Func_02004f3c
.L_ja_0200d2c0:
	movs r1, #206
	movs r2, #164
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02005e6c
.L_ja_0200d2ce:
	ldr r0, .L_ja_0200d3f4
	bl Func_02005f6c
.L_ja_0200d2d4:
	movs r0, #10
	bl Actor_GetById
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #10
	bl Actor_GetById
	str r5, [r0, #12]
	movs r0, #10
	bl Actor_GetById
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r1, #3
	movs r0, #10
	bl Actor_SetSpritePriority
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #154
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200d318
	movs r0, #10
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	b .L_ja_0200d346
.L_ja_0200d318:
	movs r0, #10
	bl Actor_GetById
	movs r2, #0
	movs r1, #8
	bl Func_02001084
	movs r0, #11
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r3, #3
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #78
	movs r2, #18
	movs r3, #105
	bl Func_02005d9c
.L_ja_0200d346:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200d37a
	movs r0, #8
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #9
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #12
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	b .L_ja_0200d3a4
.L_ja_0200d37a:
	movs r0, #8
	bl Actor_GetById
	movs r1, #180
	movs r2, #0
	bl Func_02001084
	movs r0, #9
	bl Actor_GetById
	movs r1, #60
	movs r2, #0
	bl Func_02001084
	movs r0, #12
	bl Actor_GetById
	movs r1, #60
	movs r2, #0
	bl Func_02001420
.L_ja_0200d3a4:
	movs r0, #8
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #9
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #12
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r3, .L_ja_0200d3f8
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #11
	beq .L_ja_0200d3dc
	.2byte 0xf000, 0xfc50 @ bl 0x0200dc7c
.L_ja_0200d3dc:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d3ec
	.2byte 0xf000, 0xfc48 @ bl 0x0200dc7c
.L_ja_0200d3ec:
	bl Func_0200505c
	.2byte 0xf000, 0xfc44 @ bl 0x0200dc7c
.L_ja_0200d3f4:
	.4byte gData_02006050
.L_ja_0200d3f8:
	.4byte gMainRoutineWork
.L_ja_0200d3fc:
	ldr r3, .L_ja_0200d43c
	cmp r2, r3
	bne .L_ja_0200d4e8
	ldr r3, .L_ja_0200d434
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_ja_0200d438
	subs r2, #2
	strh r3, [r2]
	bl Func_02002770
	movs r0, #243
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d440
	movs r1, #164
	movs r2, #196
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02005e6c
	b .L_ja_0200d440
.L_ja_0200d434:
	.4byte 0x00001008
.L_ja_0200d438:
	.4byte 0x00003f10
.L_ja_0200d43c:
	.4byte 0x00000056
.L_ja_0200d440:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #152
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d45c
	movs r1, #208
	movs r2, #196
	movs r0, #11
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Func_02005e6c
.L_ja_0200d45c:
	ldr r0, .L_ja_0200d4e0
	bl Func_02005f6c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200d47e
	movs r0, #8
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	b .L_ja_0200d4ac
.L_ja_0200d47e:
	movs r0, #8
	bl Actor_GetById
	movs r2, #0
	movs r1, #60
	bl Func_02001084
	movs r0, #10
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r3, #3
	movs r2, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #68
	movs r1, #84
	movs r2, #12
	movs r3, #77
	bl Func_02005d9c
.L_ja_0200d4ac:
	movs r0, #8
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r3, .L_ja_0200d4e4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	beq .L_ja_0200d4ca
	b .L_ja_0200dc7c
.L_ja_0200d4ca:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d4d8
	b .L_ja_0200dc7c
.L_ja_0200d4d8:
	bl Func_02004cc8
	b .L_ja_0200dc7c
	.2byte 0x0000
.L_ja_0200d4e0:
	.4byte gData_02006054
.L_ja_0200d4e4:
	.4byte gMainRoutineWork
.L_ja_0200d4e8:
	ldr r3, .L_ja_0200d53c
	cmp r2, r3
	beq .L_ja_0200d4f0
	b .L_ja_0200d61c
.L_ja_0200d4f0:
	bl Func_02002770
	movs r0, #245
	bl Func_02005f4c
	ldr r3, .L_ja_0200d534
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_ja_0200d538
	subs r2, #2
	movs r1, #144
	strh r3, [r2]
	ldr r0, .L_ja_0200d540
	lsls r1, r1, #3
	bl Task_Add
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #155
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d544
	movs r1, #132
	movs r2, #158
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005e6c
	b .L_ja_0200d544
	.2byte 0x0000
.L_ja_0200d534:
	.4byte 0x00000c08
.L_ja_0200d538:
	.4byte 0x00003f10
.L_ja_0200d53c:
	.4byte 0x00000057
.L_ja_0200d540:
	.4byte Func_02001690
.L_ja_0200d544:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #156
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d560
	movs r1, #154
	movs r2, #142
	movs r0, #11
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02005e6c
.L_ja_0200d560:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #157
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d57c
	movs r1, #236
	movs r2, #130
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005e6c
.L_ja_0200d57c:
	ldr r0, .L_ja_0200d8a4
	bl Func_02005f6c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200d5b6
	movs r0, #9
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #10
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	movs r0, #13
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	b .L_ja_0200d5e0
.L_ja_0200d5b6:
	movs r0, #9
	bl Actor_GetById
	movs r1, #60
	movs r2, #0
	bl Func_02001084
	movs r0, #10
	bl Actor_GetById
	movs r1, #180
	movs r2, #0
	bl Func_02001084
	movs r0, #13
	bl Actor_GetById
	movs r1, #200
	movs r2, #0
	bl Func_02001084
.L_ja_0200d5e0:
	movs r0, #9
	bl Actor_GetById
	movs r5, #0
	adds r0, #90
	strb r5, [r0]
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	strb r5, [r0]
	movs r0, #13
	bl Actor_GetById
	adds r0, #90
	strb r5, [r0]
	movs r0, #9
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #10
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #13
	b .L_ja_0200db9e
.L_ja_0200d61c:
	ldr r3, .L_ja_0200d8a8
	cmp r2, r3
	beq .L_ja_0200d624
	b .L_ja_0200d794
.L_ja_0200d624:
	bl Func_02005f54
	movs r1, #11
	movs r2, #12
	movs r0, #0
	bl Func_02005f5c
	movs r1, #13
	movs r2, #14
	movs r0, #1
	bl Func_02005f5c
	movs r1, #15
	movs r2, #16
	movs r0, #2
	bl Func_02005f5c
	movs r1, #17
	movs r2, #18
	movs r0, #3
	bl Func_02005f5c
	movs r1, #19
	movs r2, #20
	movs r0, #4
	bl Func_02005f5c
	movs r0, #18
	bl Actor_GetById
	movs r3, #1
	adds r0, #98
	strb r3, [r0]
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d6c6
	movs r3, #5
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #69
	movs r1, #64
	movs r2, #29
	movs r3, #85
	bl Func_02005d9c
	movs r3, #29
	movs r2, #21
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #20
	movs r2, #5
	movs r3, #5
	bl Func_02005dac
	movs r3, #31
	movs r2, #89
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #89
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r3, #33
	movs r2, #87
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #50
	movs r1, #86
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	b .L_ja_0200d716
.L_ja_0200d6c6:
	movs r3, #5
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #29
	movs r3, #85
	bl Func_02005d9c
	movs r3, #29
	movs r2, #21
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #52
	movs r1, #20
	movs r2, #5
	movs r3, #5
	bl Func_02005dac
	movs r3, #33
	movs r2, #87
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #33
	movs r1, #88
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r3, #31
	movs r2, #89
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #88
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
.L_ja_0200d716:
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d770
	movs r3, #4
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #90
	movs r1, #25
	movs r2, #86
	movs r3, #25
	bl Func_02005d9c
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #21
	movs r1, #88
	movs r2, #24
	movs r3, #89
	bl Func_02005d9c
	movs r3, #89
	movs r5, #23
	str r3, [sp, #4]
	movs r0, #23
	movs r1, #92
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
	movs r3, #25
	str r3, [sp, #4]
	movs r0, #18
	movs r1, #33
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
.L_ja_0200d770:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #158
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d78c
	movs r1, #162
	movs r2, #148
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02005e6c
.L_ja_0200d78c:
	ldr r0, .L_ja_0200d8ac
	bl Func_02005f6c
	b .L_ja_0200dc7c
.L_ja_0200d794:
	ldr r3, .L_ja_0200d8b0
	cmp r2, r3
	bne .L_ja_0200d816
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Func_02002770
	movs r6, #0
.L_ja_0200d7a8:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #2
	adds r0, r6, r3
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d7e0
	lsls r3, r6, #1
	movs r5, #24
	subs r5, r5, r3
	movs r2, #2
	movs r3, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #65
	movs r1, #1
	movs r2, #80
	adds r3, r5, #0
	bl Func_02005d9c
	movs r3, #255
	movs r0, #0
	movs r1, #16
	adds r2, r5, #0
	lsls r3, r3, #8
	bl Func_0200062c
.L_ja_0200d7e0:
	adds r6, #1
	cmp r6, #6
	bne .L_ja_0200d7a8
	movs r0, #8
	bl Actor_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #8
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #8
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r0, #20]
	str r3, [r5, #12]
	movs r0, #8
	bl Actor_GetById
	movs r1, #9
	bl Actor_SetActionCallback
	b .L_ja_0200dc7c
.L_ja_0200d816:
	ldr r3, .L_ja_0200d8b4
	cmp r2, r3
	beq .L_ja_0200d81e
	b .L_ja_0200d976
.L_ja_0200d81e:
	bl Func_02002770
	bl Func_02005f54
	movs r1, #11
	movs r2, #12
	movs r0, #0
	bl Func_02005f5c
	movs r1, #13
	movs r2, #14
	movs r0, #1
	bl Func_02005f5c
	movs r0, #2
	movs r1, #15
	movs r2, #16
	bl Func_02005f5c
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200d8b8
	movs r3, #5
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #69
	movs r1, #64
	movs r2, #32
	movs r3, #79
	bl Func_02005d9c
	movs r3, #32
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #15
	movs r2, #5
	movs r3, #5
	bl Func_02005dac
	movs r3, #34
	movs r2, #82
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #33
	movs r1, #82
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r3, #36
	movs r2, #81
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #52
	movs r1, #81
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	b .L_ja_0200d908
.L_ja_0200d8a4:
	.4byte gData_0200605a
.L_ja_0200d8a8:
	.4byte 0x00000058
.L_ja_0200d8ac:
	.4byte gData_02006064
.L_ja_0200d8b0:
	.4byte 0x00000059
.L_ja_0200d8b4:
	.4byte 0x0000005a
.L_ja_0200d8b8:
	movs r3, #5
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #32
	movs r3, #79
	bl Func_02005d9c
	movs r3, #32
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #49
	movs r1, #21
	movs r2, #5
	movs r3, #5
	bl Func_02005dac
	movs r3, #36
	movs r2, #81
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #81
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	movs r3, #34
	movs r2, #82
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #50
	movs r1, #82
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
.L_ja_0200d908:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d924
	movs r1, #130
	movs r2, #148
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02005e6c
.L_ja_0200d924:
	movs r0, #244
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200d934
	b .L_ja_0200dc7c
.L_ja_0200d934:
	movs r1, #156
	movs r2, #148
	lsls r2, r2, #17
	movs r0, #8
	lsls r1, r1, #17
	bl Func_02005e6c
	movs r1, #2
	movs r0, #8
	bl Actor_SetSpritePriority
	movs r0, #8
	bl Actor_GetById
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	movs r0, #8
	bl Actor_GetById
	ldr r3, .L_ja_0200da80
	movs r2, #18
	str r3, [r0, #20]
	movs r3, #19
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #19
	movs r1, #16
	movs r2, #1
	movs r3, #1
	bl Func_02005dac
	b .L_ja_0200dc7c
.L_ja_0200d976:
	ldr r3, .L_ja_0200da84
	cmp r2, r3
	beq .L_ja_0200d97e
	b .L_ja_0200da90
.L_ja_0200d97e:
	bl Func_02002770
	movs r0, #12
	bl Actor_GetById
	movs r6, #1
	adds r0, #98
	strb r6, [r0]
	movs r0, #13
	bl Actor_GetById
	adds r0, #98
	strb r6, [r0]
	movs r0, #14
	bl Actor_GetById
	adds r0, #98
	strb r6, [r0]
	movs r0, #15
	bl Actor_GetById
	adds r0, #98
	strb r6, [r0]
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200d9cc
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
.L_ja_0200d9cc:
	ldr r0, .L_ja_0200da88
	bl Func_02000908
	movs r0, #141
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d9ec
	movs r1, #172
	movs r2, #174
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005e6c
.L_ja_0200d9ec:
	ldr r0, .L_ja_0200da8c
	bl Func_02005f6c
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200da36
	movs r3, #3
	str r3, [sp, #4]
	movs r0, #39
	movs r1, #82
	movs r2, #38
	movs r3, #82
	str r6, [sp, #0]
	bl Func_02005d9c
	movs r3, #82
	movs r5, #38
	str r3, [sp, #4]
	movs r0, #39
	movs r1, #82
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
	movs r3, #18
	str r3, [sp, #4]
	movs r0, #38
	movs r1, #19
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
.L_ja_0200da36:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200da46
	b .L_ja_0200dc7c
.L_ja_0200da46:
	movs r3, #3
	str r3, [sp, #4]
	movs r0, #39
	movs r1, #82
	movs r2, #38
	movs r3, #99
	str r6, [sp, #0]
	bl Func_02005d9c
	movs r3, #99
	movs r5, #38
	str r3, [sp, #4]
	movs r0, #38
	movs r1, #98
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
	movs r3, #37
	str r3, [sp, #4]
	movs r0, #38
	movs r1, #35
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
	b .L_ja_0200dc7c
.L_ja_0200da80:
	.4byte 0xffe00000
.L_ja_0200da84:
	.4byte 0x0000005b
.L_ja_0200da88:
	.4byte gData_0200621c
.L_ja_0200da8c:
	.4byte gData_02006068
.L_ja_0200da90:
	ldr r3, .L_ja_0200dae0
	cmp r2, r3
	beq .L_ja_0200da98
	b .L_ja_0200dbaa
.L_ja_0200da98:
	movs r0, #245
	bl Func_02005f4c
	ldr r3, .L_ja_0200dad8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_ja_0200dadc
	subs r2, #2
	movs r1, #144
	strh r3, [r2]
	ldr r0, .L_ja_0200dae4
	lsls r1, r1, #3
	bl Task_Add
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #209
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200dae8
	movs r1, #140
	movs r2, #236
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02005e6c
	b .L_ja_0200dae8
	.2byte 0x0000
.L_ja_0200dad8:
	.4byte 0x00000c08
.L_ja_0200dadc:
	.4byte 0x00003f10
.L_ja_0200dae0:
	.4byte 0x0000005c
.L_ja_0200dae4:
	.4byte Func_02001690
.L_ja_0200dae8:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #210
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200db04
	movs r1, #180
	movs r2, #210
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005e6c
.L_ja_0200db04:
	movs r0, #153
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200db28
	movs r1, #184
	movs r2, #154
	movs r0, #12
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02005e6c
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_ja_0200db28:
	movs r1, #2
	movs r0, #9
	bl Actor_SetSpritePriority
	ldr r0, .L_ja_0200dc84
	bl Func_02005f6c
	movs r0, #8
	bl Actor_GetById
	ldr r5, [r0, #8]
	movs r0, #8
	bl Actor_GetById
	ldr r2, [r0, #16]
	asrs r5, r5, #20
	adds r1, r5, #0
	asrs r2, r2, #20
	movs r3, #0
	movs r0, #2
	bl Func_0200062c
	movs r0, #9
	bl Actor_GetById
	ldr r5, [r0, #8]
	movs r0, #9
	bl Actor_GetById
	ldr r2, [r0, #16]
	asrs r5, r5, #20
	asrs r2, r2, #20
	movs r0, #2
	adds r1, r5, #0
	movs r3, #0
	bl Func_0200062c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200db8e
	movs r0, #11
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	b .L_ja_0200db9c
.L_ja_0200db8e:
	movs r0, #11
	bl Actor_GetById
	movs r1, #8
	movs r2, #0
	bl Func_02001084
.L_ja_0200db9c:
	movs r0, #11
.L_ja_0200db9e:
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	b .L_ja_0200dc7c
.L_ja_0200dbaa:
	ldr r3, .L_ja_0200dc88
	cmp r2, r3
	bne .L_ja_0200dc7c
	bl Func_02002770
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #211
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200dbd0
	movs r1, #180
	movs r2, #132
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02005e6c
.L_ja_0200dbd0:
	ldr r0, .L_ja_0200dc8c
	bl Func_02005f6c
	movs r0, #10
	bl Actor_GetById
	ldr r5, [r0, #8]
	movs r0, #10
	bl Actor_GetById
	ldr r2, [r0, #16]
	asrs r5, r5, #20
	movs r3, #0
	asrs r2, r2, #20
	adds r1, r5, #0
	movs r0, #2
	bl Func_0200062c
	movs r0, #10
	bl Actor_GetById
	movs r3, #0
	adds r0, #90
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200dc1c
	movs r0, #10
	bl Actor_GetById
	movs r1, #6
	bl Actor_SetPartAttribute
	b .L_ja_0200dc2a
.L_ja_0200dc1c:
	movs r0, #10
	bl Actor_GetById
	movs r1, #60
	movs r2, #0
	bl Func_02001084
.L_ja_0200dc2a:
	movs r0, #10
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200dc7c
	movs r3, #2
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #15
	movs r1, #80
	movs r2, #15
	movs r3, #77
	bl Func_02005d9c
	movs r3, #79
	movs r5, #15
	str r3, [sp, #4]
	movs r0, #15
	movs r1, #80
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005dac
	movs r0, #17
	movs r1, #15
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005dac
.L_ja_0200dc7c:
	movs r0, #0
	sub sp, #-8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_ja_0200dc84:
	.4byte gData_02006072
.L_ja_0200dc88:
	.4byte 0x0000005d
.L_ja_0200dc8c:
	.4byte gData_0200607e
	.endif
	.section .bss,"aw",%nobits
	.global gData_020073dc
gData_020073dc:
	.space 0x00000004
	.global gData_020073e0
gData_020073e0:
	.space 0x00000004
	.global gData_020073e4
gData_020073e4:
	.space 0x00000004
	.global gData_020073e8
gData_020073e8:
