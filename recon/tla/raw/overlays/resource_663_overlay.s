.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080ce,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009908,"ax",%progbits
	.global Func_02001908
	.thumb_func
Func_02001908:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r7, .L_020099fc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r7, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r5, r0, #0
	mov r8, r3
	movs r3, #179
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009976
	movs r3, #173
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009976
	movs r3, #175
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009976
	movs r3, #180
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009976
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	adds r3, r3, r7
	mov r10, r3
	ldrb r3, [r3]
	cmp r3, #5
	bne .L_02009980
.L_02009976:
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetAnimationSpeed
	b .L_02009b42
.L_02009980:
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
	bl Func_02000348
	asrs r0, r0, #8
	cmp r0, #212
	bne .L_020099ce
.L_020099a0:
	movs r1, #142
	lsls r1, r1, #1
	adds r0, r6, #0
	adds r1, #255
	bl Func_02003cd0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #99
	strb r3, [r2]
	str r3, [r6, #108]
	ldrh r3, [r6, #6]
	movs r2, #192
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_020099c4
	ldr r1, .L_02009a00
	b .L_020099c6
.L_020099c4:
	ldr r1, .L_02009a04
.L_020099c6:
	adds r0, r6, #0
	bl Func_02003b38
	b .L_02009b42
.L_020099ce:
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_02009a7a
	mov r2, r10
	ldrb r3, [r2]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r7, #0
	adds r0, #8
	adds r1, #8
	cmp r3, #2
	bne .L_02009a08
	bl Func_020018d8
	cmp r0, #16
	bgt .L_02009a1e
	ldr r2, [r5, #16]
	ldr r3, [r6, #16]
	b .L_02009a14
	.2byte 0x0000
.L_020099fc:
	.4byte gMainRoutineWork
.L_02009a00:
	.4byte gData_02003de0
.L_02009a04:
	.4byte gData_02003dcc
.L_02009a08:
	bl Func_020018ac
	cmp r0, #8
	bgt .L_02009a1e
	ldr r2, [r5, #12]
	ldr r3, [r6, #12]
.L_02009a14:
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02009a1e
	movs r7, #1
.L_02009a1e:
	cmp r7, #0
	beq .L_02009a7a
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009a7a
	ldrh r3, [r6, #6]
	str r6, [r5, #104]
	strh r3, [r5, #6]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r2, #12
	ldr r3, [r6, #80]
	ldr r0, [r5, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r0, #9]
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
	ldr r3, .L_02009a88
	movs r2, #128
	ldr r4, .L_02009a84
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
.L_02009a7a:
	mov r3, r8
	adds r3, #52
	str r3, [sp, #0]
	movs r7, #8
	b .L_02009a8c
.L_02009a84:
	.4byte 0x00000000
.L_02009a88:
	.4byte gMainRoutineWork
.L_02009a8c:
	ldr r3, [sp, #0]
	ldmia r3!, {r5}
	adds r2, r3, #0
	str r2, [sp, #0]
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02009ade
	ldr r1, [r6, #104]
	ldr r3, [r5, #8]
	ldr r2, [r1, #8]
	asrs r3, r3, #20
	asrs r2, r2, #20
	cmp r2, r3
	bne .L_02009ac0
	ldr r2, [r1, #12]
	ldr r3, [r5, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02009ac0
	ldr r2, [r1, #16]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_02009ade
.L_02009ac0:
	adds r0, r5, #0
	adds r1, r6, #0
	adds r0, #8
	adds r1, #8
	bl Func_020018ac
	cmp r0, #8
	bgt .L_02009ade
	ldr r2, [r5, #12]
	ldr r3, [r6, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02009ade
	b .L_020099a0
.L_02009ade:
	adds r7, #1
	cmp r7, #64
	bne .L_02009a8c
	ldrh r3, [r6, #6]
	movs r2, #192
	lsls r2, r2, #8
	adds r0, r3, #0
	cmp r3, r2
	bne .L_02009b1a
	bl Math_Cosine
	movs r1, #192
	lsls r1, r1, #9
	ldr r5, .L_02009b50
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
	ldr r3, [r6, #12]
	subs r3, r3, r0
	str r3, [r6, #12]
	b .L_02009b42
.L_02009b1a:
	bl Math_Cosine
	movs r1, #192
	lsls r1, r1, #9
	ldr r5, .L_02009b50
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
.L_02009b42:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009b50:
	.4byte IwramMulQ16
	.section .bss,"aw",%nobits
	.space 0x00000004
	.global gData_020046a0
gData_020046a0:
