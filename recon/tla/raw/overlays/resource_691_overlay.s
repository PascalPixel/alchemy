.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {r5, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	cmp r5, #0
	beq .L_0200807c
	adds r3, r5, #0
	adds r3, #84
	ldrb r2, [r3]
	movs r3, #15
	ands r3, r2
	cmp r3, #0
	beq .L_0200807c
	ldr r1, [r5, #80]
	movs r2, #13
	ldrb r0, [r1, #9]
	movs r3, #3
	negs r2, r2
	ands r4, r3
	adds r3, r2, #0
	lsls r4, r4, #2
	ands r3, r0
	orrs r3, r4
	strb r3, [r1, #9]
	adds r1, #37
	ldrb r3, [r1]
	ands r2, r3
	orrs r2, r4
	strb r2, [r1]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
.L_0200807c:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008080,"ax",%progbits
	.global Func_02000080
	.thumb_func
Func_02000080:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl Func_02003a64
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020080ca
	movs r1, #0
	bl Func_02000038
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #14
	bl Func_02003b34
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	adds r0, r5, #0
	b .L_020080cc
.L_020080ca:
	movs r0, #0
.L_020080cc:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020080d0,"ax",%progbits
	.global Func_020000d0
	.thumb_func
Func_020000d0:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl Func_02003a64
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200811e
	movs r1, #1
	bl Func_02000038
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #15
	bl Func_02003b34
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #34
	orrs r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	b .L_02008120
.L_0200811e:
	movs r0, #0
.L_02008120:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008124,"ax",%progbits
	.global Func_02000124
	.thumb_func
Func_02000124:
	ldr r3, [r0, #8]
	ldr r2, [r0, #68]
	ldr r1, [r0, #80]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, [r0, #72]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [r0, #76]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	ldr r2, [r0, #48]
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [r0, #52]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	adds r0, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r0]
	adds r3, r3, r2
	strh r3, [r1, #18]
	bx lr
	.2byte 0x0000
	.section .text.x0200815c,"ax",%progbits
	.global Func_0200015c
	.thumb_func
Func_0200015c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	str r3, [sp, #0]
	ldr r3, .L_0200832c
	mov r8, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r10, r1
	ldr r7, [sp, #48]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r10
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_020081a4
	cmp r7, #0
	beq .L_020081a4
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_020081ac
.L_020081a4:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_020081ac:
	mov r3, r8
	bl Func_02003a64
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020081ba
	b .L_0200831e
.L_020081ba:
	ldr r3, [r6, #80]
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	mov r8, r3
	bl Func_02003a54
	ldr r2, .L_02008330
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_02003a5c
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_02008334
	mov r1, r9
	str r3, [r6, #108]
	ldr r3, [sp, #0]
	adds r0, r6, #0
	str r3, [r6, #68]
	ldr r3, [sp, #36]
	str r3, [r6, #72]
	ldr r3, [sp, #40]
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Func_02000038
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_02008338
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200831e
	cmp r7, #0
	beq .L_0200831e
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200823c
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Func_02003b34
.L_0200823c:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02008274
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r3, #3
	ldrb r2, [r7]
	adds r0, r6, #0
	ands r2, r3
	mov r3, r8
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	lsls r2, r2, #2
	mov r1, r8
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r1, [r7]
	bl Func_02000038
.L_02008274:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_02008288
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02008288:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_020082ce
	ldr r3, .L_02008330
	mov r1, r11
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_020082b6
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_020082c8
.L_020082b6:
	ldr r2, .L_02008338
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02008338
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_020082c8:
	bl Engine_MathDivide
	str r0, [r6, #52]
.L_020082ce:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_020082ea
	adds r0, r6, #0
	movs r1, #1
	bl Func_02003a54
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02003a5c
.L_020082ea:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_020082fc
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #18]
.L_020082fc:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200830e
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200830e:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200831e
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200831e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200832c:
	.4byte gPartyState
.L_02008330:
	.4byte Data_02003e6c
.L_02008334:
	.4byte Func_02000124
.L_02008338:
	.4byte 0xffff0000
	.section .text.x0200833c,"ax",%progbits
	.global Func_0200033c
	.thumb_func
Func_0200033c:
	push {r5, lr}
	ldmia r0!, {r5}
	ldmia r1!, {r3}
	ldmia r0!, {r4}
	subs r5, r5, r3
	ldmia r1!, {r3}
	asrs r5, r5, #16
	ldr r2, [r1]
	subs r4, r4, r3
	ldr r3, [r0]
	asrs r4, r4, #16
	subs r3, r3, r2
	asrs r3, r3, #16
	adds r0, r5, #0
	muls r0, r5
	adds r2, r4, #0
	muls r2, r4
	adds r1, r3, #0
	muls r1, r3
	adds r0, r0, r2
	adds r3, r1, #0
	adds r0, r0, r3
	ldr r3, .L_02008370
	mov lr, r3
	.2byte 0xf800
	pop {r5, pc}
.L_02008370:
	.4byte IwramFillWords + 0x74
	.section .text.x02008374,"ax",%progbits
	.global Func_02000374
	.thumb_func
Func_02000374:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r1, r0, #0
	adds r5, r3, #0
	movs r4, #8
	adds r5, #52
.L_02008384:
	ldmia r5!, {r0}
	ldr r2, [r1]
	ldr r3, [r0, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020083aa
	ldr r2, [r1, #4]
	ldr r3, [r0, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020083aa
	ldr r2, [r1, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_020083b2
.L_020083aa:
	adds r4, #1
	cmp r4, #63
	bls .L_02008384
	movs r0, #0
.L_020083b2:
	pop {r5, pc}
	.section .text.x020083b4,"ax",%progbits
	.global Func_020003b4
	.thumb_func
Func_020003b4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0200852c
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	ldrh r3, [r0, #6]
	ldr r1, .L_02008530
	lsrs r3, r3, #12
	lsls r5, r3, #2
	ldr r2, .L_02008534
	mov r9, r1
	ldr r1, [r1, r5]
	mov r10, r2
	mov r3, r10
	adds r2, r1, #0
	ands r2, r3
	ldr r3, [r0, #8]
	mov r7, sp
	adds r3, r3, r2
	str r3, [r7]
	lsls r1, r1, #16
	ldr r3, [r0, #12]
	mov r8, r0
	str r3, [r7, #4]
	ldr r3, [r0, #16]
	adds r0, r7, #0
	adds r3, r3, r1
	str r3, [r7, #8]
	mov r1, r8
	bl Func_02000374
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200840a
	b .L_02008520
.L_0200840a:
	mov r0, r9
	ldr r1, [r0, r5]
	mov r3, r10
	adds r2, r1, #0
	ands r2, r3
	ldr r3, [r6, #8]
	lsls r1, r1, #16
	adds r3, r3, r2
	str r3, [r7]
	adds r0, r7, #0
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r6, #0
	bl Func_02000374
	cmp r0, #0
	beq .L_02008440
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_02008520
.L_02008440:
	ldr r3, [r6, #8]
	movs r0, #128
	str r3, [r7]
	lsls r0, r0, #13
	ldr r3, [r6, #12]
	adds r1, r6, #0
	adds r3, r3, r0
	str r3, [r7, #4]
	adds r0, r7, #0
	ldr r3, [r6, #16]
	str r3, [r7, #8]
	bl Func_02000374
	cmp r0, #0
	beq .L_0200846c
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_02008520
.L_0200846c:
	adds r2, r6, #0
	adds r2, #34
	movs r3, #2
	strb r3, [r2]
	mov r2, r9
	ldr r1, [r2, r5]
	mov r3, r10
	adds r2, r1, #0
	ands r2, r3
	ldr r3, [r6, #8]
	lsls r1, r1, #16
	adds r3, r3, r2
	str r3, [r7]
	adds r0, r6, #0
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r7, #0
	bl Func_02003aac
	cmp r0, #0
	bgt .L_02008520
	adds r3, r6, #0
	adds r3, #98
	ldrb r3, [r3]
	mov r10, r3
	cmp r3, #0
	bne .L_02008520
	movs r1, #8
	mov r0, r8
	movs r5, #204
	bl Func_02003a54
	lsls r5, r5, #6
	movs r0, #15
	bl WaitFrames
	adds r5, #51
	movs r0, #185
	bl Func_02003c54
	str r5, [r6, #48]
	str r5, [r6, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl Func_02003a84
	mov r0, r8
	str r5, [r0, #48]
	str r5, [r0, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl Func_02003a84
	adds r0, r6, #0
	bl Func_02003a8c
	bl Func_02003bdc
	ldr r3, [r7]
	mov r1, r10
	str r3, [r6, #8]
	ldr r3, [r7, #8]
	str r1, [r6, #36]
	str r3, [r6, #16]
	str r1, [r6, #44]
	movs r3, #128
	mov r2, r8
	lsls r3, r3, #24
	str r3, [r2, #56]
	str r3, [r2, #64]
	movs r0, #10
	ldrsh r3, [r2, r0]
	str r1, [r2, #36]
	lsls r3, r3, #16
	str r1, [r2, #44]
	str r3, [r2, #8]
	movs r1, #18
	ldrsh r3, [r2, r1]
	mov r0, r8
	lsls r3, r3, #16
	str r3, [r2, #16]
	movs r1, #1
	bl Func_02003a54
.L_02008520:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200852c:
	.4byte gPartyState
.L_02008530:
	.4byte Data_02003d10
.L_02008534:
	.4byte 0xffff0000
	.section .text.x02008538,"ax",%progbits
	.global Func_02000538
	.thumb_func
Func_02000538:
	push {r5, r6, lr}
	adds r5, r3, #0
	ldr r3, [sp, #12]
	ldr r6, [sp, #16]
	mov r12, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	beq .L_0200858a
	cmp r0, #2
	bhi .L_02008560
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r0, r0, #1
	lsls r3, r3, #3
	adds r3, r3, r0
	ldr r0, [r4, r3]
	b .L_02008562
.L_02008560:
	ldr r0, .L_02008590
.L_02008562:
	lsls r3, r2, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	movs r1, #0
	adds r0, r0, r3
	cmp r1, r12
	bcs .L_0200858a
.L_02008570:
	lsls r3, r1, #9
	movs r2, #0
	adds r3, r0, r3
	cmp r2, r5
	bcs .L_02008584
.L_0200857a:
	adds r2, #1
	strb r6, [r3, #2]
	adds r3, #4
	cmp r2, r5
	bcc .L_0200857a
.L_02008584:
	adds r1, #1
	cmp r1, r12
	bcc .L_02008570
.L_0200858a:
	movs r0, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008590:
	.4byte gMapCellBuffer
	.section .text.x02008594,"ax",%progbits
	.global Func_02000594
	.thumb_func
Func_02000594:
	push {lr}
	movs r0, #8
	movs r1, #56
	bl Func_02003b84
	pop {pc}
	.section .text.x020085a0,"ax",%progbits
	.global Func_020005a0
	.thumb_func
Func_020005a0:
	push {lr}
	ldr r3, [r1]
	ldr r4, [r0]
	ldr r2, [r1, #8]
	subs r4, r4, r3
	ldr r3, [r0, #8]
	asrs r4, r4, #16
	subs r3, r3, r2
	asrs r3, r3, #16
	adds r2, r3, #0
	muls r2, r3
	adds r0, r4, #0
	muls r0, r4
	adds r3, r2, #0
	adds r0, r0, r3
	ldr r3, .L_020085c8
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_020085c8:
	.4byte IwramFillWords + 0x74
	.section .text.x020085cc,"ax",%progbits
	.global Func_020005cc
	.thumb_func
Func_020005cc:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	mov r10, r3
	movs r3, #192
	sub sp, #4
	adds r4, r1, #0
	lsls r3, r3, #18
	mov r8, r0
	str r4, [sp, #0]
	adds r6, r2, #0
	ldr r5, [r3, #32]
	bl Func_02003adc
	mov r2, r8
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r5, [r5, r3]
	ldr r3, .L_02008624
	ldr r4, [sp, #0]
	ldr r2, .L_02008628
	adds r5, r5, r3
	lsls r6, r6, #7
	asrs r5, r5, #2
	adds r1, r0, #0
	adds r4, r4, r6
	adds r5, r5, r2
	mov r0, r8
	mov r2, r10
	adds r5, r5, r4
	bl Func_02003c24
	strb r0, [r5]
	add sp, #4
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_02008624:
	.4byte 0xfdff0000
.L_02008628:
	.4byte Data_02024000
	.section .text.x0200862c,"ax",%progbits
	.global Func_0200062c
	.thumb_func
Func_0200062c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_0200863c
	movs r0, #0
	b .L_02008662
.L_0200863c:
	cmp r0, #2
	bhi .L_02008650
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_02008652
.L_02008650:
	ldr r4, .L_02008664
.L_02008652:
	lsls r3, r2, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	ldrb r0, [r4, #2]
	ldrb r3, [r4, #3]
	lsls r0, r0, #8
	orrs r0, r3
.L_02008662:
	pop {pc}
.L_02008664:
	.4byte gMapCellBuffer
	.section .text.x02008668,"ax",%progbits
	.global Func_02000668
	.thumb_func
Func_02000668:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	b .L_020086b8
.L_02008672:
	ldrh r0, [r7]
	bl Object_GetById
	movs r3, #34
	adds r6, r0, #0
	adds r3, r3, r6
	ldrb r0, [r3]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	mov r8, r3
	bl Map_GetTerrainHeight
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	adds r2, #4
	strb r3, [r2]
	adds r5, r0, #0
	movs r1, #0
	adds r0, r6, #0
	bl ObjectDispatch_SetSingleChildField26
	mov r2, r8
	ldrb r0, [r2]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	asrs r5, r5, #19
	adds r5, #4
	asrs r2, r2, #20
	adds r3, r5, #0
	asrs r1, r1, #20
	adds r7, #6
	bl Func_020005cc
.L_020086b8:
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_02008672
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020086cc,"ax",%progbits
	.global Func_020006cc
	.thumb_func
Func_020006cc:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	b .L_0200873c
.L_020086d2:
	ldrh r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	adds r7, r5, #0
	movs r3, #0
	adds r7, #99
	strb r3, [r7]
	movs r0, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	bl Func_0200062c
	movs r3, #64
	ands r0, r3
	cmp r0, #0
	beq .L_0200873a
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	cmp r0, #0
	beq .L_02008748
	movs r3, #212
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r2, [r5, #16]
	ldr r3, [r5, #12]
	ldr r1, [r5, #8]
	subs r2, r2, r3
	asrs r2, r2, #20
	adds r3, r2, #1
	asrs r1, r1, #20
	ldr r4, [r0]
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	subs r2, #1
	movs r3, #255
	strb r3, [r4, #2]
	lsls r2, r2, #7
	ldr r4, [r0]
	adds r1, r1, r2
	lsls r1, r1, #2
	movs r3, #1
	negs r3, r3
	adds r4, r4, r1
	strb r3, [r4, #2]
	movs r3, #1
	strb r3, [r7]
.L_0200873a:
	adds r6, #2
.L_0200873c:
	movs r2, #255
	ldrh r3, [r6]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_020086d2
.L_02008748:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200874c,"ax",%progbits
	.global Func_0200074c
	.thumb_func
Func_0200074c:
	push {r5, lr}
	adds r5, r0, #0
	movs r2, #99
	adds r2, r2, r5
	ldrb r3, [r2]
	mov r12, r2
	cmp r3, #0
	beq .L_0200879a
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	cmp r0, #0
	beq .L_0200879a
	movs r3, #212
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r2, [r5, #16]
	ldr r3, [r5, #12]
	ldr r1, [r5, #8]
	subs r2, r2, r3
	asrs r2, r2, #20
	adds r3, r2, #1
	asrs r1, r1, #20
	ldr r4, [r0]
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	subs r2, #1
	movs r3, #0
	strb r3, [r4, #2]
	lsls r2, r2, #7
	ldr r4, [r0]
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r4, r4, r1
	mov r2, r12
	strb r3, [r4, #2]
	strb r3, [r2]
.L_0200879a:
	pop {r5, pc}
	.section .text.x0200879c,"ax",%progbits
	.global Func_0200079c
	.thumb_func
Func_0200079c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	mov r8, r0
	cmp r3, r2
	beq .L_0200887c
.L_020087b2:
	mov r3, r8
	ldrh r0, [r3]
	bl Object_GetById
	movs r2, #34
	adds r7, r0, #0
	adds r2, r2, r7
	mov r10, r2
	ldrb r0, [r2]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	bl Map_GetTerrainHeight
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	adds r6, r0, #0
	asrs r2, r2, #20
	asrs r1, r1, #20
	movs r0, #0
	bl Func_0200062c
	movs r3, #255
	ands r3, r0
	str r3, [r7, #76]
	adds r3, r7, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r3, #4
	strb r5, [r3]
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	asrs r6, r6, #19
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	mov r3, r10
	adds r6, #4
	ldrb r0, [r3]
	adds r3, r6, #0
	adds r6, r7, #0
	asrs r1, r1, #20
	asrs r2, r2, #20
	adds r6, #99
	bl Func_020005cc
	strb r5, [r6]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #0
	bl Func_0200062c
	movs r3, #64
	ands r0, r3
	cmp r0, #0
	beq .L_0200886a
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	cmp r0, #0
	beq .L_0200887c
	movs r2, #212
	lsls r2, r2, #1
	adds r0, r0, r2
	ldr r3, [r7, #12]
	ldr r2, [r7, #16]
	ldr r1, [r7, #8]
	subs r2, r2, r3
	asrs r2, r2, #20
	adds r3, r2, #1
	asrs r1, r1, #20
	ldr r4, [r0]
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	subs r2, #1
	movs r3, #255
	strb r3, [r4, #2]
	lsls r2, r2, #7
	ldr r4, [r0]
	adds r1, r1, r2
	lsls r1, r1, #2
	movs r3, #1
	negs r3, r3
	adds r4, r4, r1
	strb r3, [r4, #2]
	movs r3, #1
	strb r3, [r6]
.L_0200886a:
	movs r3, #2
	add r8, r3
	mov r2, r8
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_020087b2
.L_0200887c:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.section .text.x02008884,"ax",%progbits
	.global Func_02000884
	.thumb_func
Func_02000884:
	ldr r0, .L_02008888
	bx lr
.L_02008888:
	.4byte Data_02003e78
	.section .text.x0200888c,"ax",%progbits
	.global Func_0200088c
	.thumb_func
Func_0200088c:
	push {lr}
	ldr r3, .L_020088b4
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020088b8
	cmp r2, r3
	bne .L_020088a4
	ldr r0, .L_020088bc
	b .L_020088b0
.L_020088a4:
	ldr r3, .L_020088c0
	cmp r2, r3
	bne .L_020088ae
	ldr r0, .L_020088c4
	b .L_020088b0
.L_020088ae:
	ldr r0, .L_020088c8
.L_020088b0:
	pop {pc}
	.2byte 0x0000
.L_020088b4:
	.4byte gPartyState
.L_020088b8:
	.4byte 0x000000e0
.L_020088bc:
	.4byte Data_02003ed8
.L_020088c0:
	.4byte 0x000000e1
.L_020088c4:
	.4byte Data_02003f88
.L_020088c8:
	.4byte Data_02003ea8
	.section .text.x020088cc,"ax",%progbits
	.global Func_020008cc
	.thumb_func
Func_020008cc:
	ldr r0, .L_020088d0
	bx lr
.L_020088d0:
	.4byte Data_02003fc8
	.section .text.x020088d4,"ax",%progbits
	.global Func_020008d4
	.thumb_func
Func_020008d4:
	push {lr}
	ldr r1, .L_0200891c
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008920
	cmp r2, r3
	bne .L_0200890e
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #3
	bgt .L_020088fa
	ldr r0, .L_02008924
	b .L_0200891a
.L_020088fa:
	cmp r3, #8
	bgt .L_02008902
	ldr r0, .L_02008928
	b .L_0200891a
.L_02008902:
	cmp r3, #12
	bgt .L_0200890a
	ldr r0, .L_0200892c
	b .L_0200891a
.L_0200890a:
	ldr r0, .L_02008930
	b .L_0200891a
.L_0200890e:
	ldr r3, .L_02008934
	cmp r2, r3
	bne .L_02008918
	ldr r0, .L_02008938
	b .L_0200891a
.L_02008918:
	ldr r0, .L_0200893c
.L_0200891a:
	pop {pc}
.L_0200891c:
	.4byte gPartyState
.L_02008920:
	.4byte 0x000000e0
.L_02008924:
	.4byte Data_020041a4
.L_02008928:
	.4byte Data_0200427c
.L_0200892c:
	.4byte Data_020043e4
.L_02008930:
	.4byte Data_020044a4
.L_02008934:
	.4byte 0x000000e1
.L_02008938:
	.4byte Data_020045c4
.L_0200893c:
	.4byte Data_0200403c
	.section .text.x02008940,"ax",%progbits
	.global Func_02000940
	.thumb_func
Func_02000940:
	push {r5, r6, lr}
	movs r3, #192
	ldr r1, .L_02008a28
	lsls r3, r3, #18
	movs r2, #240
	lsls r2, r2, #1
	ldr r6, [r3, #108]
	adds r3, #224
	ldr r0, [r3]
	adds r3, r1, r2
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldr r3, .L_02008a2c
	sub sp, #8
	cmp r2, r3
	bne .L_0200898e
	movs r3, #21
	str r3, [sp, #4]
	movs r5, #78
	movs r0, #98
	movs r1, #18
	movs r2, #4
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02003aa4
	movs r3, #24
	str r3, [sp, #4]
	movs r0, #98
	movs r1, #24
	movs r2, #4
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02003aa4
	ldr r0, .L_02008a30
	bl Func_02003c14
	b .L_02008a14
.L_0200898e:
	ldr r3, .L_02008a34
	cmp r2, r3
	bne .L_02008a14
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #3
	bgt .L_020089aa
	ldr r0, .L_02008a38
	bl Func_02003c14
	b .L_02008a14
.L_020089aa:
	cmp r3, #7
	bgt .L_020089b6
	ldr r0, .L_02008a3c
	bl Func_02003c14
	b .L_02008a14
.L_020089b6:
	cmp r3, #11
	bgt .L_020089d6
	movs r3, #79
	movs r2, #36
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #79
	movs r1, #46
	movs r2, #2
	movs r3, #1
	bl Func_02003aa4
	ldr r0, .L_02008a40
	bl Func_02003c14
	b .L_02008a14
.L_020089d6:
	ldr r3, [r0, #20]
	movs r4, #150
	movs r2, #18
	ldrsh r3, [r3, r2]
	lsls r4, r4, #2
	cmp r3, r4
	bne .L_020089fa
	movs r3, #103
	movs r2, #38
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #104
	movs r1, #38
	movs r2, #1
	movs r3, #1
	bl Func_02003aa4
	b .L_02008a0e
.L_020089fa:
	movs r3, #99
	movs r2, #38
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #94
	movs r1, #44
	movs r2, #5
	movs r3, #1
	bl Func_02003aa4
.L_02008a0e:
	ldr r0, .L_02008a44
	bl Func_02003c14
.L_02008a14:
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #188
	adds r3, r6, r2
	ldr r0, [r3]
	bl Func_0200074c
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008a28:
	.4byte gPartyState
.L_02008a2c:
	.4byte 0x000000df
.L_02008a30:
	.4byte Data_02003d98
.L_02008a34:
	.4byte 0x000000e0
.L_02008a38:
	.4byte Data_02003dac
.L_02008a3c:
	.4byte Data_02003dd2
.L_02008a40:
	.4byte Data_02003de4
.L_02008a44:
	.4byte Data_02003de8
	.section .text.x02008a48,"ax",%progbits
	.global Func_02000a48
	.thumb_func
Func_02000a48:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #192
	lsls r0, r0, #4
	adds r0, #188
	adds r3, r3, r0
	ldr r6, [r3]
	sub sp, #8
	bl Func_02003aec
	movs r0, #0
	bl Func_02003bcc
	bl Func_02003c1c
	ldr r1, .L_02008be8
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldr r3, .L_02008bec
	cmp r2, r3
	bne .L_02008aaa
	movs r3, #21
	str r3, [sp, #4]
	movs r5, #78
	movs r0, #98
	movs r1, #21
	movs r2, #4
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02003aa4
	movs r3, #24
	str r3, [sp, #4]
	movs r0, #98
	movs r1, #27
	movs r2, #4
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02003aa4
	ldr r0, .L_02008bf0
	bl Func_020006cc
	b .L_02008b1e
.L_02008aaa:
	ldr r3, .L_02008bf4
	cmp r2, r3
	bne .L_02008b1e
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bgt .L_02008ac6
	ldr r0, .L_02008bf8
	bl Func_020006cc
	b .L_02008b1e
.L_02008ac6:
	cmp r3, #8
	bgt .L_02008ad2
	ldr r0, .L_02008bfc
	bl Func_020006cc
	b .L_02008b1e
.L_02008ad2:
	cmp r3, #12
	bgt .L_02008af2
	movs r3, #79
	movs r2, #36
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #79
	movs r1, #37
	movs r2, #2
	movs r3, #1
	bl Func_02003aa4
	ldr r0, .L_02008c00
	bl Func_020006cc
	b .L_02008b1e
.L_02008af2:
	movs r3, #103
	str r3, [sp, #0]
	movs r5, #38
	movs r0, #103
	movs r1, #39
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003aa4
	movs r3, #99
	str r3, [sp, #0]
	movs r0, #99
	movs r1, #42
	movs r2, #5
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003aa4
	ldr r0, .L_02008c04
	bl Func_020006cc
.L_02008b1e:
	ldr r3, [r6, #8]
	asrs r2, r3, #20
	ldr r3, [r6, #16]
	asrs r1, r3, #20
	ldr r3, .L_02008be8
	mov r12, r3
	movs r3, #240
	lsls r3, r3, #1
	add r3, r12
	movs r4, #0
	ldrsh r0, [r3, r4]
	ldr r3, .L_02008bec
	cmp r0, r3
	bne .L_02008b62
	cmp r2, #15
	bne .L_02008b4e
	cmp r1, #25
	bne .L_02008b4e
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #214
	bl Func_02003a44
	b .L_02008be0
.L_02008b4e:
	cmp r2, #16
	bne .L_02008be0
	cmp r1, #22
	bne .L_02008be0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #213
	bl Func_02003a44
	b .L_02008be0
.L_02008b62:
	ldr r3, .L_02008bf4
	cmp r0, r3
	bne .L_02008be0
	movs r3, #241
	lsls r3, r3, #1
	add r3, r12
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #3
	bgt .L_02008b8a
	cmp r2, #27
	bne .L_02008be0
	cmp r1, #33
	bne .L_02008be0
	movs r0, #251
	lsls r0, r0, #3
	adds r0, #255
	bl Func_02003a44
	b .L_02008be0
.L_02008b8a:
	cmp r3, #8
	bgt .L_02008ba2
	cmp r2, #55
	bne .L_02008be0
	cmp r1, #35
	bne .L_02008be0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #216
	bl Func_02003a44
	b .L_02008be0
.L_02008ba2:
	cmp r3, #12
	bgt .L_02008bba
	cmp r2, #16
	bne .L_02008be0
	cmp r1, #36
	bne .L_02008be0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #217
	bl Func_02003a44
	b .L_02008be0
.L_02008bba:
	cmp r2, #40
	bne .L_02008bce
	cmp r1, #37
	bne .L_02008bce
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #218
	bl Func_02003a44
	b .L_02008be0
.L_02008bce:
	cmp r2, #35
	bne .L_02008be0
	cmp r1, #38
	bne .L_02008be0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #219
	bl Func_02003a44
.L_02008be0:
	bl Func_02003af4
	add sp, #8
	pop {r5, r6, pc}
.L_02008be8:
	.4byte gPartyState
.L_02008bec:
	.4byte 0x000000df
.L_02008bf0:
	.4byte Data_02003d98
.L_02008bf4:
	.4byte 0x000000e0
.L_02008bf8:
	.4byte Data_02003dac
.L_02008bfc:
	.4byte Data_02003dd2
.L_02008c00:
	.4byte Data_02003de4
.L_02008c04:
	.4byte Data_02003de8
	.section .text.x02008c08,"ax",%progbits
	.global Func_02000c08
	.thumb_func
Func_02000c08:
	push {r5, lr}
	adds r5, r0, #0
	movs r1, #1
	movs r0, #151
	bl Func_02003bac
	lsls r5, r5, #16
	ldr r3, .L_02008c3c
	asrs r5, r5, #16
	movs r2, #133
	lsls r2, r2, #2
	lsls r5, r5, #16
	adds r3, r3, r2
	lsrs r5, r5, #16
	ldr r0, [r3]
	adds r1, r5, #0
	bl Func_02003bb4
	movs r0, #1
	bl Func_02003ba4
	bl Func_02003bbc
	bl Func_02003bc4
	pop {r5, pc}
.L_02008c3c:
	.4byte gPartyState
	.section .text.x02008c40,"ax",%progbits
	.global Func_02000c40
	.thumb_func
Func_02000c40:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r0, #0
	movs r1, #100
	adds r1, r1, r7
	movs r2, #0
	ldrsh r3, [r1, r2]
	sub sp, #56
	ldr r5, [r7, #104]
	mov r10, r1
	cmp r3, #49
	bgt .L_02008cc2
	ldr r3, .L_02008d30
	ldr r6, [r3]
	mov r9, r3
	movs r3, #3
	ands r6, r3
	cmp r6, #0
	bne .L_02008cba
	add r1, sp, #16
	mov r8, r1
	movs r3, #148
	mov r2, r8
	adds r3, #255
	strh r3, [r2, #24]
	movs r3, #1
	str r3, [r2]
	ldr r3, .L_02008d34
	movs r5, #15
	str r3, [r2, #28]
	bl Random16Far
	ands r5, r0
	bl Random16Far
	ldr r1, [r7, #12]
	movs r3, #31
	ands r3, r0
	lsls r3, r3, #16
	adds r1, r1, r3
	ldr r3, .L_02008d38
	ldr r0, [r7, #8]
	adds r1, r1, r3
	movs r3, #200
	lsls r3, r3, #14
	adds r3, #1
	subs r5, #8
	ldr r2, [r7, #16]
	lsls r5, r5, #16
	str r3, [sp, #8]
	mov r3, r8
	str r3, [sp, #12]
	adds r0, r0, r5
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl Func_0200015c
.L_02008cba:
	mov r1, r9
	ldr r3, [r1]
	movs r2, #4
	b .L_02008cd6
.L_02008cc2:
	cmp r3, #99
	bgt .L_02008ccc
	ldr r3, .L_02008d30
	movs r2, #4
	b .L_02008cd4
.L_02008ccc:
	cmp r3, #139
	bgt .L_02008cf0
	ldr r3, .L_02008d30
	movs r2, #2
.L_02008cd4:
	ldr r3, [r3]
.L_02008cd6:
	ands r3, r2
	cmp r3, #0
	beq .L_02008ce6
	adds r0, r7, #0
	movs r1, #10
	bl Func_02003b34
	b .L_02008d1a
.L_02008ce6:
	adds r0, r7, #0
	movs r1, #0
	bl Func_02003b34
	b .L_02008d1a
.L_02008cf0:
	cmp r3, #143
	bgt .L_02008cfe
	adds r0, r7, #0
	movs r1, #7
	bl Func_02003b34
	b .L_02008d1a
.L_02008cfe:
	adds r0, r7, #0
	movs r1, #0
	bl Func_02003b34
	movs r3, #0
	str r3, [r7, #108]
	ldr r3, [r7, #8]
	str r3, [r5, #8]
	ldr r3, [r7, #12]
	str r3, [r5, #12]
	ldr r3, [r7, #16]
	str r3, [r5, #16]
	ldr r3, .L_02008d3c
	str r3, [r5, #108]
.L_02008d1a:
	mov r2, r10
	ldrh r3, [r2]
	mov r1, r10
	adds r3, #1
	strh r3, [r1]
	add sp, #56
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008d30:
	.4byte Data_0300122c
.L_02008d34:
	.4byte Data_02003d74
.L_02008d38:
	.4byte 0xfff80000
.L_02008d3c:
	.4byte Func_02000dc0
	.section .text.x02008d40,"ax",%progbits
	.global Func_02000d40
	.thumb_func
Func_02000d40:
	push {r5, r6, r7, lr}
	ldr r3, .L_02008db8
	sub sp, #56
	ldr r2, [r3]
	movs r3, #3
	ands r3, r2
	adds r6, r0, #0
	cmp r3, #0
	bne .L_02008db4
	movs r3, #148
	add r7, sp, #16
	adds r3, #255
	strh r3, [r7, #24]
	movs r3, #2
	str r3, [r7]
	ldr r3, .L_02008dbc
	str r3, [r7, #28]
	movs r3, #15
	ands r3, r2
	cmp r3, #0
	bne .L_02008d7e
	movs r0, #152
	lsls r0, r0, #2
	bl Func_02003a3c
	cmp r0, #0
	bne .L_02008d7e
	adds r0, r6, #0
	movs r1, #136
	bl Func_02003c4c
.L_02008d7e:
	bl Random16Far
	movs r5, #15
	ands r5, r0
	bl Random16Far
	movs r3, #31
	ands r3, r0
	subs r3, #16
	lsls r3, r3, #12
	ldr r2, [r6, #16]
	str r3, [sp, #4]
	movs r3, #200
	subs r5, #8
	lsls r3, r3, #14
	adds r3, #1
	lsls r5, r5, #15
	movs r4, #0
	str r3, [sp, #8]
	ldr r0, [r6, #8]
	ldr r1, [r6, #12]
	adds r2, r2, r5
	movs r3, #0
	str r4, [sp, #0]
	str r7, [sp, #12]
	bl Func_0200015c
.L_02008db4:
	add sp, #56
	pop {r5, r6, r7, pc}
.L_02008db8:
	.4byte Data_0300122c
.L_02008dbc:
	.4byte Data_02003d50
	.section .text.x02008dc0,"ax",%progbits
	.global Func_02000dc0
	.thumb_func
Func_02000dc0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	ldrh r0, [r6, #6]
	bl Math_Cosine
	adds r3, r6, #0
	movs r2, #98
	adds r2, r2, r6
	adds r3, #102
	adds r5, r0, #0
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldrb r3, [r2]
	mov r10, r2
	adds r0, r0, r3
	bl Object_GetById
	ldr r3, .L_02008ecc
	movs r4, #133
	lsls r4, r4, #2
	adds r3, r3, r4
	mov r8, r0
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r6, #8]
	lsls r5, r5, #1
	adds r3, r3, r5
	adds r7, r0, #0
	adds r0, r6, #0
	str r3, [r6, #8]
	bl Func_02000d40
	ldr r1, .L_02008ed0
	ldr r0, [r7, #8]
	movs r4, #192
	adds r2, r0, r1
	ldr r1, [r6, #8]
	lsls r4, r4, #11
	adds r3, r1, r4
	cmp r2, r3
	bge .L_02008e7a
	ldr r3, .L_02008ed4
	movs r4, #128
	lsls r4, r4, #12
	adds r2, r1, r3
	adds r3, r0, r4
	cmp r2, r3
	bge .L_02008e7a
	ldr r1, .L_02008ed0
	ldr r0, [r7, #16]
	movs r4, #192
	adds r2, r0, r1
	ldr r1, [r6, #16]
	lsls r4, r4, #11
	adds r3, r1, r4
	cmp r2, r3
	bge .L_02008e7a
	ldr r3, .L_02008ed4
	movs r4, #128
	lsls r4, r4, #12
	adds r2, r1, r3
	adds r3, r0, r4
	cmp r2, r3
	bge .L_02008e7a
	ldr r2, [r6, #12]
	ldr r0, [r7, #12]
	adds r3, r2, r4
	cmp r0, r3
	bge .L_02008e7a
	ldr r1, .L_02008ed0
	movs r4, #240
	lsls r4, r4, #12
	adds r2, r2, r1
	adds r3, r0, r4
	cmp r2, r3
	bge .L_02008e7a
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #181
	lsls r2, r2, #1
	adds r1, r3, r2
	movs r2, #0
	movs r3, #200
	strh r3, [r1]
	str r2, [r6, #108]
	str r2, [r6, #8]
	str r2, [r6, #12]
	str r2, [r6, #16]
.L_02008e7a:
	adds r5, r6, #0
	adds r5, #8
	mov r1, r8
	adds r1, #8
	adds r0, r5, #0
	bl Func_020005a0
	cmp r0, #8
	bgt .L_02008edc
	mov r4, r8
	ldr r2, [r6, #12]
	ldr r3, [r4, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02008edc
	movs r1, #0
	str r1, [r5]
	ldr r2, .L_02008ec8
	ldrh r3, [r6, #6]
	str r1, [r6, #108]
	eors r3, r2
	strh r3, [r6, #6]
	str r1, [r6, #12]
	str r1, [r6, #16]
	mov r2, r10
	ldrb r3, [r2]
	movs r2, #1
	eors r3, r2
	mov r4, r10
	strb r3, [r4]
	mov r3, r8
	adds r3, #100
	strh r1, [r3]
	ldr r3, .L_02008ed8
	mov r1, r8
	str r3, [r1, #108]
	str r6, [r1, #104]
	b .L_02008edc
.L_02008ec8:
	.4byte 0x00009000
.L_02008ecc:
	.4byte gPartyState
.L_02008ed0:
	.4byte 0xfff80000
.L_02008ed4:
	.4byte 0xfffa0000
.L_02008ed8:
	.4byte Func_02000c40
.L_02008edc:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.section .text.x02008ee4,"ax",%progbits
	.global Func_02000ee4
	.thumb_func
Func_02000ee4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	lsls r1, r1, #16
	lsls r2, r2, #16
	lsls r0, r0, #16
	asrs r1, r1, #16
	asrs r2, r2, #16
	lsrs r0, r0, #16
	mov r8, r1
	mov r9, r2
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	adds r0, #255
	bl Func_02003a3c
	mov r11, r0
	cmp r0, #0
	bne .L_02008fbc
	mov r2, r8
	lsls r3, r2, #16
	mov r2, r9
	lsrs r7, r3, #16
	lsls r3, r2, #16
	lsrs r3, r3, #16
	adds r5, r7, r3
	adds r0, r5, #0
	mov r10, r3
	bl Object_GetById
	ldr r3, [r0, #8]
	adds r0, r5, #0
	str r3, [r6, #8]
	bl Object_GetById
	ldr r3, [r0, #12]
	adds r0, r5, #0
	str r3, [r6, #12]
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r1, #2
	str r3, [r6, #16]
	adds r0, r6, #0
	bl Func_02000038
	adds r3, r6, #0
	mov r2, r8
	adds r3, #102
	strh r2, [r3]
	movs r2, #1
	mov r3, r9
	eors r3, r2
	adds r2, r6, #0
	adds r2, #98
	strb r3, [r2]
	ldr r3, .L_02008fc8
	movs r2, #85
	str r3, [r6, #108]
	adds r3, r7, #1
	mov r8, r3
	adds r2, r2, r6
	mov r3, r10
	mov r9, r2
	cmp r3, #0
	beq .L_02008f96
	adds r0, r7, #0
	bl Object_GetById
	adds r5, r0, #0
	mov r0, r8
	bl Object_GetById
	ldr r2, [r5, #8]
	ldr r3, [r0, #8]
	cmp r2, r3
	bge .L_02008f90
	movs r3, #128
	lsls r3, r3, #8
	b .L_02008fb4
.L_02008f90:
	mov r2, r11
	strh r2, [r6, #6]
	b .L_02008fb6
.L_02008f96:
	adds r0, r7, #0
	bl Object_GetById
	adds r5, r0, #0
	mov r0, r8
	bl Object_GetById
	ldr r2, [r5, #8]
	ldr r3, [r0, #8]
	cmp r2, r3
	ble .L_02008fb2
	movs r3, #128
	lsls r3, r3, #8
	b .L_02008fb4
.L_02008fb2:
	mov r3, r10
.L_02008fb4:
	strh r3, [r6, #6]
.L_02008fb6:
	movs r3, #0
	mov r2, r9
	strb r3, [r2]
.L_02008fbc:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008fc8:
	.4byte Func_02000dc0
	.section .text.x02008fcc,"ax",%progbits
	.global Func_02000fcc
	.thumb_func
Func_02000fcc:
	push {lr}
	ldr r3, .L_02008ff0
	adds r2, r0, #0
	adds r2, #99
	ldrb r2, [r2]
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02008fe6
	movs r1, #10
	bl Func_02003b34
	b .L_02008fec
.L_02008fe6:
	movs r1, #0
	bl Func_02003b34
.L_02008fec:
	pop {pc}
	.2byte 0x0000
.L_02008ff0:
	.4byte Data_0300122c
	.section .text.x02008ff4,"ax",%progbits
	.global Func_02000ff4
	.thumb_func
Func_02000ff4:
	push {r5, r6, lr}
	movs r3, #192
	adds r6, r0, #0
	lsls r3, r3, #18
	adds r6, #100
	ldr r5, [r3, #108]
	movs r1, #0
	ldrsh r3, [r6, r1]
	movs r1, #132
	lsls r1, r1, #2
	ldrh r2, [r6]
	cmp r3, r1
	ble .L_02009012
	movs r3, #2
	b .L_0200901c
.L_02009012:
	movs r1, #200
	lsls r1, r1, #1
	cmp r3, r1
	ble .L_02009032
	movs r3, #16
.L_0200901c:
	ands r3, r2
	cmp r3, #0
	beq .L_0200902a
	movs r1, #10
	bl Func_02003b34
	b .L_0200903a
.L_0200902a:
	movs r1, #0
	bl Func_02003b34
	b .L_0200903a
.L_02009032:
	ldr r3, .L_02009080
	str r3, [r0, #108]
	movs r3, #1
	strh r3, [r6]
.L_0200903a:
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200907c
	subs r2, #12
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200907c
	adds r2, #4
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200907c
	adds r2, #10
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200907c
	adds r2, #74
	adds r3, r5, r2
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_0200907c
	ldrh r3, [r6]
	subs r3, #1
	strh r3, [r6]
.L_0200907c:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009080:
	.4byte Func_02001738
	.section .text.x02009084,"ax",%progbits
	.global Func_02001084
	.thumb_func
Func_02001084:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r6, [r3]
	adds r5, r0, #0
	ldr r1, [r6, #4]
	ldr r2, [r6, #12]
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #0
	bl Func_0200062c
	movs r7, #0
	asrs r2, r0, #8
	b .L_020090a8
.L_020090a4:
	ldrh r7, [r5]
	adds r5, #2
.L_020090a8:
	movs r1, #255
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	beq .L_020090c8
	adds r5, #2
	adds r0, r3, #0
	ldrh r3, [r5]
	adds r5, #2
	cmp r2, r3
	bne .L_020090a4
	bl Object_GetById
	ldrh r7, [r5]
	str r0, [r6, #20]
.L_020090c8:
	adds r0, r7, #0
	pop {r5, r6, r7, pc}
	.section .text.x020090cc,"ax",%progbits
	.global Func_020010cc
	.thumb_func
Func_020010cc:
	push {r5, r6, lr}
	adds r3, r0, #0
	adds r4, r1, #0
	asrs r3, r3, #20
	asrs r4, r4, #20
	adds r5, r2, #0
	adds r1, r3, #0
	adds r2, r4, #0
	movs r0, #0
	bl Func_0200062c
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	movs r6, #0
	asrs r0, r0, #8
	b .L_020090fe
.L_020090f0:
	adds r5, #2
	ldrh r6, [r5]
	adds r5, #2
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
.L_020090fe:
	cmp r3, r2
	beq .L_0200910c
	adds r5, #2
	ldrh r3, [r5]
	cmp r0, r3
	bne .L_020090f0
	ldrh r6, [r5, #2]
.L_0200910c:
	adds r0, r6, #0
	pop {r5, r6, pc}
	.section .text.x02009110,"ax",%progbits
	.global Func_02001110
	.thumb_func
Func_02001110:
	push {r5, r6, r7, lr}
	movs r3, #192
	ldr r1, .L_02009190
	adds r6, r0, #0
	lsls r3, r3, #18
	movs r0, #240
	adds r3, #224
	lsls r0, r0, #1
	ldr r7, [r3]
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009194
	ldr r5, .L_02009198
	cmp r2, r3
	bne .L_02009156
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #3
	bgt .L_02009142
	ldr r5, .L_0200919c
	b .L_0200915e
.L_02009142:
	cmp r3, #8
	bgt .L_0200914a
	ldr r5, .L_020091a0
	b .L_0200915e
.L_0200914a:
	cmp r3, #12
	bgt .L_02009152
	ldr r5, .L_020091a4
	b .L_0200915e
.L_02009152:
	ldr r5, .L_020091a8
	b .L_0200915e
.L_02009156:
	ldr r3, .L_020091ac
	cmp r2, r3
	bne .L_0200915e
	ldr r5, .L_020091b0
.L_0200915e:
	cmp r6, #2
	bne .L_02009176
	adds r0, r5, #0
	bl Func_02001084
	ldr r3, [r7, #20]
	movs r2, #4
	adds r1, r3, #0
	adds r1, #99
	strb r2, [r1]
	ldr r2, .L_020091b4
	str r2, [r3, #108]
.L_02009176:
	cmp r6, #3
	bne .L_0200918e
	adds r0, r5, #0
	bl Func_02001084
	ldr r3, [r7, #20]
	movs r2, #2
	adds r1, r3, #0
	adds r1, #99
	strb r2, [r1]
	ldr r2, .L_020091b4
	str r2, [r3, #108]
.L_0200918e:
	pop {r5, r6, r7, pc}
.L_02009190:
	.4byte gPartyState
.L_02009194:
	.4byte 0x000000e0
.L_02009198:
	.4byte Data_02003d9e
.L_0200919c:
	.4byte Data_02003db2
.L_020091a0:
	.4byte Data_02003dc4
.L_020091a4:
	.4byte Data_02003dd6
.L_020091a8:
	.4byte Data_02003dee
.L_020091ac:
	.4byte 0x000000e1
.L_020091b0:
	.4byte Data_02003e02
.L_020091b4:
	.4byte Func_02000fcc
	.section .text.x020091b8,"ax",%progbits
	.global Func_020011b8
	.thumb_func
Func_020011b8:
	ldr r4, [r0, #68]
	ldr r3, [r0, #8]
	ldr r1, [r0, #72]
	adds r3, r3, r4
	str r3, [r0, #8]
	ldr r3, [r0, #12]
	ldr r2, [r0, #76]
	adds r3, r3, r1
	str r3, [r0, #12]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	asrs r3, r4, #3
	subs r4, r4, r3
	asrs r3, r1, #3
	subs r1, r1, r3
	asrs r3, r2, #3
	subs r2, r2, r3
	str r4, [r0, #68]
	str r1, [r0, #72]
	str r2, [r0, #76]
	bx lr
	.section .text.x020091e4,"ax",%progbits
	.global Func_020011e4
	.thumb_func
Func_020011e4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	ldr r1, [r0, #8]
	mov r8, r0
	movs r0, #148
	adds r0, #255
	sub sp, #68
	bl Func_02003a64
	mov r1, r8
	adds r6, r0, #0
	ldrh r0, [r1, #6]
	bl Math_Cosine
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	lsls r0, r0, #1
	strb r3, [r2]
	movs r1, #1
	mov r10, r0
	adds r0, r6, #0
	bl Func_02003a54
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	bl Func_02003aec
	movs r0, #0
	bl Func_02003bcc
	ldr r3, [r6, #8]
	add r2, sp, #56
	str r3, [r2]
	ldr r3, [r6, #12]
	str r3, [r2, #4]
	ldr r3, [r6, #16]
	str r3, [r2, #8]
.L_0200923e:
	add r7, sp, #56
	ldr r1, [r7]
	ldr r2, [r7, #8]
	ldr r3, [r7, #4]
	add r1, r10
	subs r2, r2, r3
	str r1, [r7]
	asrs r2, r2, #20
	asrs r1, r1, #20
	movs r0, #2
	bl Func_0200062c
	asrs r0, r0, #8
	cmp r0, #212
	beq .L_020092d6
	cmp r0, #222
	beq .L_020092d6
	movs r2, #8
	mov r9, r2
.L_02009264:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	mov r0, r9
	lsls r3, r0, #2
	adds r3, #20
	ldr r5, [r2, r3]
	ldr r3, [r5]
	cmp r3, #0
	beq .L_020092ca
	mov r1, r8
	ldr r2, [r1, #8]
	ldr r3, [r5, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_0200929e
	ldr r2, [r1, #12]
	ldr r3, [r5, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_0200929e
	ldr r2, [r1, #16]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_020092ca
.L_0200929e:
	adds r0, r5, #0
	adds r0, #8
	adds r1, r7, #0
	bl Func_020005a0
	cmp r0, #8
	bgt .L_020092ca
	ldr r2, [r5, #12]
	ldr r3, [r7, #4]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020092ca
	ldr r3, [r5, #80]
	movs r0, #136
	ldr r3, [r3, #40]
	lsls r0, r0, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, r0
	bne .L_020093ae
	b .L_020092d6
.L_020092ca:
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #64
	bne .L_02009264
	b .L_0200923e
.L_020092d6:
	ldr r3, .L_02009430
	movs r5, #128
	lsls r5, r5, #10
	add r7, sp, #56
	str r5, [r6, #52]
	str r5, [r6, #48]
	ldr r2, [r7, #4]
	str r3, [r6, #108]
	ldr r1, [r7]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl Func_02003a84
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #7
	bl Func_02003b5c
	ldr r1, [r7, #4]
	ldr r2, [r7, #8]
	movs r3, #1
	ldr r0, [r7]
	bl Func_02003b64
	adds r0, r6, #0
	bl Func_02003a8c
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	bl Func_02003a6c
	movs r3, #0
	mov r9, r3
.L_0200931a:
	ldr r3, .L_02009434
	movs r2, #1
	ldr r3, [r3]
	mov r10, r3
	mov r0, r10
	ands r0, r2
	mov r10, r0
	cmp r0, #0
	bne .L_02009378
	add r1, sp, #16
	mov r8, r1
	movs r3, #148
	mov r0, r8
	adds r3, #255
	strh r3, [r0, #24]
	ldr r3, .L_02009438
	str r2, [r0]
	str r3, [r0, #36]
	ldr r3, .L_0200943c
	movs r6, #31
	str r3, [r0, #28]
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	ands r0, r6
	ldr r4, [r7]
	mov r3, r10
	ands r5, r6
	subs r0, #16
	ldr r1, [r7, #4]
	ldr r2, [r7, #8]
	lsls r0, r0, #13
	subs r5, #16
	str r3, [sp, #4]
	movs r3, #153
	lsls r3, r3, #17
	lsls r5, r5, #13
	str r0, [sp, #0]
	mov r0, r8
	str r3, [sp, #8]
	str r0, [sp, #12]
	adds r3, r5, #0
	adds r0, r4, #0
	bl Func_0200015c
.L_02009378:
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #16
	bne .L_0200931a
	movs r0, #24
	bl WaitFrames
	ldr r3, .L_02009440
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	movs r1, #1
	bl Func_02003b54
	bl Func_02003b6c
	movs r0, #10
	bl WaitFrames
	bl Func_02003af4
	b .L_02009424
.L_020093ae:
	ldr r3, .L_02009430
	movs r5, #128
	lsls r5, r5, #10
	str r5, [r6, #52]
	str r5, [r6, #48]
	ldr r2, [r7, #4]
	str r3, [r6, #108]
	ldr r1, [r7]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl Func_02003a84
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #7
	bl Func_02003b5c
	ldr r2, [r7, #8]
	ldr r1, [r7, #4]
	movs r3, #1
	ldr r0, [r7]
	bl Func_02003b64
	adds r0, r6, #0
	bl Func_02003a8c
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	bl Func_02003a6c
	movs r0, #188
	lsls r0, r0, #2
	bl Func_02003a44
	mov r1, r9
	lsls r0, r1, #16
	lsrs r0, r0, #16
	bl Func_02000c08
	movs r0, #188
	lsls r0, r0, #2
	bl Func_02003a4c
	ldr r3, .L_02009440
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Func_02003b54
	bl Func_02003b6c
	movs r0, #10
	bl WaitFrames
	bl Func_02003af4
.L_02009424:
	add sp, #68
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009430:
	.4byte Func_02000d40
.L_02009434:
	.4byte Data_0300122c
.L_02009438:
	.4byte Func_020011b8
.L_0200943c:
	.4byte Data_02003d50
.L_02009440:
	.4byte gPartyState
	.section .text.x02009444,"ax",%progbits
	.global Func_02001444
	.thumb_func
Func_02001444:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r5, [r3]
	bl Func_02003aec
	movs r0, #0
	bl Func_02003bcc
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, [r5, #20]
	movs r3, #0
	str r3, [r0, #108]
	movs r1, #7
	bl Func_02003b34
	movs r0, #3
	bl Battle_WaitMode0
	movs r1, #0
	ldr r0, [r5, #20]
	bl Func_02003b34
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r5, #20]
	bl Func_020011e4
	bl Func_02003af4
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200948c,"ax",%progbits
	.global Func_0200148c
	.thumb_func
Func_0200148c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r5, [r3]
	bl Func_02003aec
	movs r0, #0
	bl Func_02003bcc
	ldr r1, [r5, #20]
	movs r3, #128
	adds r2, r1, #0
	adds r2, #100
	lsls r3, r3, #3
	strh r3, [r2]
	ldr r3, .L_020094b8
	str r3, [r1, #108]
	bl Func_02003af4
	pop {r5, pc}
	.2byte 0x0000
.L_020094b8:
	.4byte Func_02000ff4
	.section .text.x020094bc,"ax",%progbits
	.global Func_020014bc
	.thumb_func
Func_020014bc:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r6, r1, #0
	adds r7, r0, #0
	ldr r5, [r3]
	cmp r6, #0
	bne .L_020094d2
	bl Func_020038dc
.L_020094d2:
	cmp r7, #2
	bne .L_020094ea
	ldr r3, [r5, #20]
	movs r2, #240
	ldr r1, [r3, #12]
	lsls r2, r2, #12
	ldr r0, [r3, #8]
	adds r1, r1, r2
	ldr r2, [r3, #16]
	movs r3, #1
	bl Func_02003820
.L_020094ea:
	cmp r7, #3
	bne .L_0200950c
	ldr r3, [r5, #20]
	movs r2, #240
	ldr r1, [r3, #12]
	lsls r2, r2, #12
	ldr r0, [r3, #8]
	adds r1, r1, r2
	ldr r2, [r3, #16]
	movs r3, #8
	bl Func_02003820
	ldr r2, [r5, #20]
	movs r3, #0
	str r3, [r2, #8]
	str r3, [r2, #12]
	str r3, [r2, #16]
.L_0200950c:
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	cmp r6, r3
	bne .L_0200951a
	bl Func_02003998
.L_0200951a:
	cmp r6, #178
	bne .L_02009526
	ldr r0, [r5, #20]
	movs r1, #3
	bl Func_02003a54
.L_02009526:
	pop {r5, r6, r7, pc}
	.section .text.x02009528,"ax",%progbits
	.global Func_02001528
	.thumb_func
Func_02001528:
	push {r5, r6, lr}
	ldr r5, [r0, #104]
	adds r4, r5, #0
	adds r4, #100
	movs r1, #0
	ldrsh r2, [r4, r1]
	cmp r2, #35
	ble .L_0200955c
	adds r3, r0, #0
	adds r3, #98
	ldrb r3, [r3]
	lsls r3, r3, #1
	adds r3, #36
	cmp r2, r3
	bge .L_0200955c
	ldr r2, [r5, #12]
	movs r3, #128
	lsls r3, r3, #14
	adds r2, r2, r3
	str r2, [r0, #12]
	ldr r1, .L_020095e0
	movs r6, #0
	ldrsh r3, [r4, r6]
	lsls r3, r3, #19
	adds r2, r2, r3
	b .L_020095be
.L_0200955c:
	movs r2, #98
	adds r2, r2, r0
	mov r12, r2
	ldrb r2, [r2]
	movs r6, #0
	ldrsh r1, [r4, r6]
	lsls r3, r2, #1
	adds r3, #36
	cmp r3, r1
	bge .L_02009588
	movs r3, #166
	lsls r3, r3, #1
	cmp r1, r3
	bge .L_02009588
	ldr r3, [r5, #12]
	lsls r2, r2, #20
	movs r6, #128
	adds r3, r3, r2
	lsls r6, r6, #14
	adds r3, r3, r6
	str r3, [r0, #12]
	b .L_020095de
.L_02009588:
	movs r2, #0
	ldrsh r1, [r4, r2]
	movs r3, #166
	lsls r3, r3, #1
	cmp r1, r3
	ble .L_020095c4
	mov r6, r12
	ldrb r2, [r6]
	movs r6, #78
	lsls r3, r2, #1
	adds r6, #255
	adds r3, r3, r6
	cmp r1, r3
	bge .L_020095c4
	lsls r3, r2, #20
	ldr r2, [r5, #12]
	movs r1, #128
	adds r2, r2, r3
	lsls r1, r1, #14
	adds r2, r2, r1
	str r2, [r0, #12]
	movs r1, #166
	movs r6, #0
	ldrsh r3, [r4, r6]
	lsls r1, r1, #20
	lsls r3, r3, #19
	subs r2, r2, r3
.L_020095be:
	adds r2, r2, r1
	str r2, [r0, #12]
	b .L_020095de
.L_020095c4:
	adds r3, r0, #0
	adds r3, #98
	ldrb r3, [r3]
	movs r6, #78
	lsls r3, r3, #1
	adds r6, #255
	adds r2, r3, r6
	movs r1, #0
	ldrsh r3, [r4, r1]
	cmp r2, r3
	bge .L_020095de
	bl Func_02003a6c
.L_020095de:
	pop {r5, r6, pc}
.L_020095e0:
	.4byte 0xfee80000
	.section .text.x020095e4,"ax",%progbits
	.global Func_020015e4
	.thumb_func
Func_020015e4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	ldr r0, [r6, #104]
	movs r1, #224
	mov r8, r0
	mov r4, r8
	adds r4, #100
	ldrh r2, [r4]
	lsls r1, r1, #11
	adds r3, r2, #0
	subs r3, #35
	lsls r3, r3, #16
	sub sp, #56
	cmp r3, r1
	bhi .L_02009624
	mov r3, r8
	ldr r2, [r3, #12]
	movs r0, #192
	lsls r0, r0, #13
	adds r2, r2, r0
	str r2, [r6, #12]
	movs r1, #0
	ldrsh r3, [r4, r1]
	lsls r3, r3, #19
	adds r2, r2, r3
	ldr r3, .L_0200971c
	adds r2, r2, r3
	str r2, [r6, #12]
	b .L_020096a4
.L_02009624:
	adds r3, r2, #0
	subs r3, #9
	movs r1, #161
	adds r0, r6, #0
	lsls r3, r3, #16
	lsls r1, r1, #17
	adds r0, #98
	cmp r3, r1
	bhi .L_0200964a
	ldrb r2, [r0]
	mov r0, r8
	ldr r3, [r0, #12]
	lsls r2, r2, #20
	movs r1, #192
	adds r3, r3, r2
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r6, #12]
	b .L_020096a4
.L_0200964a:
	lsls r3, r2, #16
	movs r2, #166
	asrs r1, r3, #16
	lsls r2, r2, #1
	cmp r1, r2
	ble .L_02009686
	ldrb r2, [r0]
	movs r0, #78
	lsls r3, r2, #1
	adds r0, #255
	adds r3, r3, r0
	cmp r1, r3
	bge .L_02009686
	mov r1, r8
	lsls r3, r2, #20
	ldr r2, [r1, #12]
	movs r1, #166
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #13
	adds r2, r2, r3
	str r2, [r6, #12]
	lsls r1, r1, #20
	movs r0, #0
	ldrsh r3, [r4, r0]
	lsls r3, r3, #19
	subs r2, r2, r3
	adds r2, r2, r1
	str r2, [r6, #12]
	b .L_020096a4
.L_02009686:
	adds r3, r6, #0
	adds r3, #98
	ldrb r3, [r3]
	movs r0, #78
	lsls r3, r3, #1
	adds r0, #255
	adds r2, r3, r0
	movs r1, #0
	ldrsh r3, [r4, r1]
	cmp r2, r3
	bge .L_020096a4
	ldr r3, [r6, #20]
	str r3, [r6, #12]
	movs r3, #0
	str r3, [r6, #108]
.L_020096a4:
	ldr r3, .L_02009720
	movs r2, #7
	ldr r7, [r3]
	mov r10, r2
	ands r7, r2
	cmp r7, #0
	bne .L_020096f8
	movs r3, #148
	add r5, sp, #16
	adds r3, #255
	strh r3, [r5, #24]
	movs r3, #10
	str r3, [r5, #4]
	movs r3, #2
	str r3, [r5]
	ldr r3, .L_02009724
	str r3, [r5, #36]
	ldr r3, .L_02009728
	str r3, [r5, #28]
	bl Random16Far
	adds r3, r0, #0
	mov r0, r10
	ands r3, r0
	ldr r0, [r6, #8]
	ldr r4, .L_0200972c
	movs r1, #128
	lsls r1, r1, #9
	ldr r2, .L_02009730
	adds r0, r0, r1
	ldr r1, [r6, #12]
	str r4, [sp, #0]
	ldr r4, .L_02009734
	subs r3, #3
	adds r1, r1, r2
	lsls r3, r3, #14
	ldr r2, [r6, #16]
	str r7, [sp, #4]
	str r4, [sp, #8]
	str r5, [sp, #12]
	bl Func_0200015c
.L_020096f8:
	mov r3, r8
	adds r3, #34
	ldrb r0, [r3]
	mov r3, r8
	ldr r1, [r3, #8]
	ldr r2, [r3, #16]
	ldr r3, [r6, #12]
	asrs r1, r1, #20
	asrs r3, r3, #19
	asrs r2, r2, #20
	adds r3, #1
	bl Func_020005cc
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200971c:
	.4byte 0xfef00000
.L_02009720:
	.4byte Data_0300122c
.L_02009724:
	.4byte Func_020011b8
.L_02009728:
	.4byte Data_02003d50
.L_0200972c:
	.4byte 0xfffe0000
.L_02009730:
	.4byte 0xfffc0000
.L_02009734:
	.4byte 0x01330000
	.section .text.x02009738,"ax",%progbits
	.global Func_02001738
	.thumb_func
Func_02001738:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, .L_020099b0
	mov r9, r3
	ldr r3, [r5]
	mov r8, r0
	movs r0, #15
	ands r3, r0
	sub sp, #72
	mov r10, r0
	cmp r3, #0
	bne .L_02009768
	mov r0, r8
	movs r1, #136
	bl Func_02003c4c
.L_02009768:
	movs r1, #10
	mov r0, r8
	bl Func_02003b34
	movs r1, #100
	add r1, r8
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r11, r1
	cmp r3, #31
	bgt .L_020097e6
	ldr r7, [r5]
	movs r3, #3
	ands r7, r3
	cmp r7, #0
	beq .L_0200978a
	b .L_02009954
.L_0200978a:
	movs r3, #148
	add r6, sp, #32
	adds r3, #255
	strh r3, [r6, #24]
	movs r3, #10
	str r3, [r6, #4]
	movs r3, #2
	str r3, [r6]
	ldr r3, .L_020099b4
	str r3, [r6, #36]
	ldr r3, .L_020099b8
	str r3, [r6, #28]
	bl Random16Far
	mov r3, r10
	adds r5, r0, #0
	ands r5, r3
	bl Random16Far
	mov r4, r10
	mov r1, r8
	ands r0, r4
	ldr r4, [r1, #8]
	ldr r1, [r1, #12]
	movs r3, #192
	lsls r3, r3, #13
	movs r2, #128
	lsls r2, r2, #9
	adds r1, r1, r3
	mov r3, r8
	adds r4, r4, r2
	ldr r2, [r3, #16]
	ldr r3, .L_020099bc
	subs r5, #8
	subs r0, #8
	lsls r0, r0, #14
	lsls r5, r5, #14
	str r0, [sp, #4]
	str r3, [sp, #8]
	adds r0, r4, #0
	adds r3, r5, #0
	str r7, [sp, #0]
	str r6, [sp, #12]
	bl Func_0200015c
	b .L_02009954
.L_020097e6:
	cmp r3, #33
	beq .L_020097ec
	b .L_0200993e
.L_020097ec:
	mov r0, r8
	movs r1, #144
	bl Func_02003c4c
	mov r4, r8
	ldr r1, [r4, #8]
	ldr r2, [r4, #12]
	movs r0, #128
	lsls r0, r0, #9
	movs r3, #128
	adds r1, r1, r0
	lsls r3, r3, #14
	movs r0, #96
	adds r2, r2, r3
	adds r0, #255
	ldr r3, [r4, #16]
	bl Func_02003a64
	add r7, sp, #16
	str r0, [r7]
	movs r4, #1
	mov r10, r4
.L_02009818:
	ldr r3, [r7]
	movs r0, #96
	ldr r6, [r3, #80]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	adds r0, #255
	bl Func_02003a64
	mov r1, r10
	ldrb r3, [r6, #16]
	lsls r5, r1, #2
	ldr r1, [r0, #80]
	movs r2, #1
	strb r3, [r1, #16]
	ldrb r3, [r1, #17]
	str r0, [r7, r5]
	orrs r3, r2
	strb r3, [r1, #17]
	ldr r2, [r7]
	movs r6, #0
	ldr r3, [r2, #8]
	movs r1, #0
	str r3, [r0, #8]
	ldr r3, [r2, #12]
	str r3, [r0, #12]
	ldr r3, [r2, #16]
	str r3, [r0, #16]
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r7, r5]
	movs r1, #3
	bl Func_02003a54
	ldr r0, [r7, r5]
	mov r2, r10
	movs r3, #3
	subs r3, r3, r2
	ldr r5, .L_020099c0
	adds r2, r0, #0
	adds r2, #98
	strb r3, [r2]
	mov r3, r8
	str r3, [r0, #104]
	str r5, [r0, #108]
	movs r1, #3
	bl Func_02003b34
	movs r4, #1
	add r10, r4
	mov r0, r10
	cmp r0, #4
	bne .L_02009818
	ldr r0, [r7]
	movs r1, #0
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r7]
	movs r1, #3
	bl Func_02003a54
	ldr r0, [r7]
	movs r1, #3
	bl Func_02000038
	ldr r0, [r7]
	movs r3, #3
	adds r2, r0, #0
	adds r2, #98
	mov r1, r8
	strb r3, [r2]
	str r1, [r0, #104]
	str r5, [r0, #108]
	movs r1, #3
	bl Func_02003b34
	ldr r1, .L_020099c4
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldr r3, .L_020099c8
	movs r0, #0
	cmp r2, r3
	bne .L_020098de
	mov r1, r8
	ldr r0, [r1, #8]
	ldr r2, .L_020099cc
	ldr r1, [r1, #16]
	bl Func_020010cc
	b .L_0200991e
.L_020098de:
	ldr r3, .L_020099d0
	cmp r2, r3
	bne .L_0200991e
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	mov r1, r8
	ldr r0, [r1, #8]
	ldr r1, [r1, #16]
	cmp r3, #3
	bgt .L_02009900
	ldr r2, .L_020099d4
	bl Func_020010cc
	b .L_0200991e
.L_02009900:
	cmp r3, #8
	bgt .L_0200990c
	ldr r2, .L_020099d8
	bl Func_020010cc
	b .L_0200991e
.L_0200990c:
	cmp r3, #12
	bgt .L_02009918
	ldr r2, .L_020099dc
	bl Func_020010cc
	b .L_0200991e
.L_02009918:
	ldr r2, .L_020099e0
	bl Func_020010cc
.L_0200991e:
	bl Object_GetById
	adds r2, r0, #0
	adds r2, #98
	movs r3, #4
	strb r3, [r2]
	ldr r3, .L_020099e4
	mov r2, r8
	str r2, [r0, #104]
	str r3, [r0, #108]
	mov r4, r11
	ldrh r3, [r4]
	mov r0, r11
	adds r3, #1
	strh r3, [r0]
	b .L_02009954
.L_0200993e:
	movs r1, #182
	lsls r1, r1, #1
	cmp r3, r1
	ble .L_02009954
	mov r0, r8
	movs r1, #0
	bl Func_02003b34
	movs r3, #0
	mov r2, r8
	str r3, [r2, #108]
.L_02009954:
	movs r3, #179
	lsls r3, r3, #1
	add r3, r9
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	bne .L_020099a2
	movs r3, #173
	lsls r3, r3, #1
	add r3, r9
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	bne .L_020099a2
	movs r3, #175
	lsls r3, r3, #1
	add r3, r9
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_020099a2
	movs r3, #180
	lsls r3, r3, #1
	add r3, r9
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020099a2
	movs r3, #217
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_020099a2
	mov r2, r8
	adds r2, #100
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020099a2:
	add sp, #72
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020099b0:
	.4byte Data_0300122c
.L_020099b4:
	.4byte Func_020011b8
.L_020099b8:
	.4byte Data_02003d50
.L_020099bc:
	.4byte 0x01330000
.L_020099c0:
	.4byte Func_02001528
.L_020099c4:
	.4byte gPartyState
.L_020099c8:
	.4byte 0x000000df
.L_020099cc:
	.4byte Data_02003d9e
.L_020099d0:
	.4byte 0x000000e0
.L_020099d4:
	.4byte Data_02003db2
.L_020099d8:
	.4byte Data_02003dc4
.L_020099dc:
	.4byte Data_02003dd6
.L_020099e0:
	.4byte Data_02003dee
.L_020099e4:
	.4byte Func_020015e4
	.section .text.x020099e8,"ax",%progbits
	.global Func_020019e8
	.thumb_func
Func_020019e8:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r5, [r3]
	ldr r3, .L_02009a2c
	adds r7, r1, #0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009a30
	adds r6, r0, #0
	cmp r2, r3
	bne .L_02009a0c
	movs r0, #15
	b .L_02009a14
.L_02009a0c:
	ldr r3, .L_02009a34
	cmp r2, r3
	bne .L_02009a1a
	movs r0, #14
.L_02009a14:
	bl Object_GetById
	str r0, [r5, #20]
.L_02009a1a:
	adds r0, r6, #0
	adds r1, r7, #0
	bl Func_020014bc
	movs r0, #128
	lsls r0, r0, #2
	bl Func_02003a44
	pop {r5, r6, r7, pc}
.L_02009a2c:
	.4byte gPartyState
.L_02009a30:
	.4byte 0x000000df
.L_02009a34:
	.4byte 0x000000e0
	.section .text.x02009a38,"ax",%progbits
	.global Func_02001a38
	.thumb_func
Func_02001a38:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r5, [r3]
	ldr r3, .L_02009a80
	adds r7, r1, #0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009a84
	adds r6, r0, #0
	cmp r2, r3
	bne .L_02009a5c
	movs r0, #16
	b .L_02009a64
.L_02009a5c:
	ldr r3, .L_02009a88
	cmp r2, r3
	bne .L_02009a6a
	movs r0, #10
.L_02009a64:
	bl Object_GetById
	str r0, [r5, #20]
.L_02009a6a:
	adds r0, r6, #0
	adds r1, r7, #0
	bl Func_020014bc
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003a44
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009a80:
	.4byte gPartyState
.L_02009a84:
	.4byte 0x000000df
.L_02009a88:
	.4byte 0x000000e0
	.section .text.x02009a8c,"ax",%progbits
	.global Func_02001a8c
	.thumb_func
Func_02001a8c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r6, r0, #0
	movs r0, #17
	ldr r5, [r3]
	mov r8, r1
	bl Object_GetById
	mov r1, r8
	str r0, [r5, #20]
	adds r0, r6, #0
	bl Func_020014bc
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl Func_02003a44
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02009ac0,"ax",%progbits
	.global Func_02001ac0
	.thumb_func
Func_02001ac0:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r6, r0, #0
	movs r0, #18
	ldr r5, [r3]
	mov r8, r1
	bl Object_GetById
	mov r1, r8
	str r0, [r5, #20]
	adds r0, r6, #0
	bl Func_020014bc
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003a44
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02009af4,"ax",%progbits
	.global Func_02001af4
	.thumb_func
Func_02001af4:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r6, r0, #0
	movs r0, #11
	ldr r5, [r3]
	mov r8, r1
	bl Object_GetById
	mov r1, r8
	str r0, [r5, #20]
	adds r0, r6, #0
	bl Func_020014bc
	movs r0, #129
	lsls r0, r0, #2
	bl Func_02003a44
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.section .text.x02009b24,"ax",%progbits
	.global Func_02001b24
	.thumb_func
Func_02001b24:
	push {lr}
	ldr r3, .L_02009b40
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	ldrb r3, [r3]
	cmp r3, #2
	beq .L_02009b3e
	movs r0, #190
	lsls r0, r0, #2
	bl Func_02003a4c
.L_02009b3e:
	pop {pc}
.L_02009b40:
	.4byte gPartyState
	.section .text.x02009b44,"ax",%progbits
	.global Func_02001b44
	.thumb_func
Func_02001b44:
	push {r5, lr}
	ldr r5, .L_02009ba0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r5, r2
	ldrb r3, [r3]
	cmp r3, #2
	beq .L_02009b96
	bl Func_02003aec
	movs r0, #0
	bl Func_02003bcc
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #132
	movs r2, #220
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r5]
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #123
	bl Func_02003c54
	movs r0, #2
	bl Func_02003b7c
	bl Func_02003af4
.L_02009b96:
	movs r0, #190
	lsls r0, r0, #2
	bl Func_02003a44
	pop {r5, pc}
.L_02009ba0:
	.4byte gPartyState
	.section .text.x02009ba4,"ax",%progbits
	.global Func_02001ba4
	.thumb_func
Func_02001ba4:
	push {lr}
	movs r0, #0
	bl Func_02003bcc
	pop {pc}
	.2byte 0x0000
	.section .text.x02009bb0,"ax",%progbits
	.global Func_02001bb0
	.thumb_func
Func_02001bb0:
	push {lr}
	ldr r0, .L_02009bbc
	bl Func_02002e30
	pop {pc}
	.2byte 0x0000
.L_02009bbc:
	.4byte Data_02003dba
	.section .text.x02009bc0,"ax",%progbits
	.global Func_02001bc0
	.thumb_func
Func_02001bc0:
	push {lr}
	movs r1, #132
	movs r2, #138
	movs r0, #0
	lsls r1, r1, #17
	lsls r2, r2, #18
	movs r3, #30
	bl Func_02003ad4
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl Func_02003a44
	pop {pc}
	.2byte 0x0000
	.section .text.x02009be0,"ax",%progbits
	.global Func_02001be0
	.thumb_func
Func_02001be0:
	push {lr}
	movs r1, #132
	movs r2, #138
	movs r0, #0
	lsls r1, r1, #17
	lsls r2, r2, #18
	movs r3, #0
	bl Func_02003ad4
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl Func_02003a4c
	pop {pc}
	.2byte 0x0000
	.section .text.x02009c00,"ax",%progbits
	.global Func_02001c00
	.thumb_func
Func_02001c00:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r5, .L_02009df8
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02003aec
	movs r0, #0
	bl Func_02003bcc
	movs r0, #152
	lsls r0, r0, #2
	bl Func_02003a44
	movs r0, #133
	bl Func_02003c54
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #6
	bl Func_02003b34
	ldr r0, [r5]
	movs r1, #10
	bl Object_SetModeById
	ldr r0, [r5]
	movs r1, #32
	bl Object_SetActionById
	add r3, sp, #28
	mov r9, r3
	mov r2, r9
	movs r3, #1
	str r3, [r2]
	movs r3, #14
	str r3, [r2, #4]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_02009c6c:
	bl Random16Far
	movs r5, #15
	ldr r3, .L_02009dfc
	ands r0, r5
	movs r2, #16
	lsls r0, r0, #16
	add r2, sp
	adds r0, r0, r3
	str r0, [r2]
	mov r8, r2
	bl Random16Far
	ands r0, r5
	lsls r0, r0, #16
	mov r2, r8
	str r0, [r2, #4]
	ldr r3, [r2]
	ldr r4, [r7, #8]
	ldr r2, [r7, #16]
	adds r4, r4, r3
	ldr r3, .L_02009e00
	ldr r1, [r7, #12]
	str r3, [sp, #0]
	movs r3, #176
	lsls r3, r3, #12
	str r3, [sp, #8]
	mov r3, r9
	adds r1, r1, r0
	str r3, [sp, #12]
	adds r0, r4, #0
	movs r3, #0
	movs r5, #0
	str r5, [sp, #4]
	bl Func_0200015c
	movs r0, #5
	bl WaitFrames
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #8
	bne .L_02009c6c
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	subs r2, #2
	movs r3, #0
	bl Func_02003b64
	ldr r3, .L_02009df8
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	adds r2, r7, #0
	strb r5, [r3]
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r7, #72]
	str r5, [r7, #68]
	movs r0, #1
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r7, #6]
	movs r3, #60
	mov r10, r3
.L_02009d02:
	ldr r3, .L_02009e04
	ldr r6, [r3]
	movs r3, #1
	ands r6, r3
	cmp r6, #0
	bne .L_02009d50
	bl Random16Far
	movs r5, #15
	ldr r2, .L_02009dfc
	ands r0, r5
	lsls r0, r0, #16
	adds r0, r0, r2
	mov r3, r8
	str r0, [r3]
	bl Random16Far
	ands r0, r5
	lsls r0, r0, #16
	mov r2, r8
	str r0, [r2, #4]
	ldr r3, [r2]
	ldr r4, [r7, #8]
	ldr r1, [r7, #12]
	adds r4, r4, r3
	ldr r3, .L_02009e00
	ldr r2, [r7, #16]
	str r3, [sp, #0]
	movs r3, #176
	lsls r3, r3, #12
	str r3, [sp, #8]
	mov r3, r9
	adds r1, r1, r0
	str r3, [sp, #12]
	adds r0, r4, #0
	movs r3, #0
	str r6, [sp, #4]
	bl Func_0200015c
.L_02009d50:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #40]
	cmp r3, #0
	beq .L_02009d68
	movs r2, #1
	negs r2, r2
	add r10, r2
	mov r3, r10
	cmp r3, #0
	bne .L_02009d02
.L_02009d68:
	movs r0, #188
	bl Func_02003c54
	adds r0, r7, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	ldr r5, .L_02009df8
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r1, #22
	bl Object_SetModeById
	movs r0, #160
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #12
	lsls r2, r2, #9
	bl Func_02003ac4
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02003ac4
	bl Func_02003acc
	ldr r0, [r5]
	movs r1, #1
	bl Func_02003b74
	bl Func_02003b6c
	ldr r0, [r5]
	movs r1, #4
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl Func_02003b34
	ldr r0, [r5]
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #152
	lsls r0, r0, #2
	bl Func_02003a4c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	bl Func_02003af4
	add sp, #68
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009df8:
	.4byte gPartyState
.L_02009dfc:
	.4byte 0xfff80000
.L_02009e00:
	.4byte 0x00013333
.L_02009e04:
	.4byte Data_0300122c
	.section .text.x02009e08,"ax",%progbits
	.global Func_02001e08
	.thumb_func
Func_02001e08:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009ed4
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #20
	bl Object_GetById
	movs r2, #8
	adds r1, r0, #0
	add r2, sp
	ldr r3, [r1, #8]
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	mov r2, r10
	str r3, [r2]
	mov r0, r10
	ldr r3, [r1, #12]
	movs r6, #79
	str r3, [r2, #4]
	ldr r3, [r1, #16]
	str r3, [r2, #8]
	bl Func_02000374
	adds r5, r0, #0
	bl Func_0200074c
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldr r3, [r5, #20]
	adds r7, r5, #0
	adds r7, #34
	asrs r1, r1, #20
	asrs r2, r2, #20
	asrs r3, r3, #19
	ldrb r0, [r7]
	bl Func_020005cc
	movs r3, #36
	str r3, [sp, #4]
	movs r0, #79
	movs r1, #46
	movs r2, #2
	mov r8, r3
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02003aa4
	bl Func_020003b4
	movs r3, #0
	mov r1, r8
	strb r3, [r7]
	movs r0, #79
	str r1, [sp, #4]
	movs r2, #2
	movs r1, #37
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02003aa4
	ldr r3, [r5, #20]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	asrs r3, r3, #19
	asrs r1, r1, #20
	asrs r2, r2, #20
	adds r3, #4
	ldrb r0, [r7]
	bl Func_020005cc
	ldr r0, .L_02009ed8
	bl Func_020006cc
	ldr r2, [r5, #8]
	mov r3, r10
	asrs r2, r2, #20
	str r2, [r3]
	mov r1, r10
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	str r3, [r1, #8]
	cmp r2, #16
	bne .L_02009eca
	cmp r3, #36
	bne .L_02009eca
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #217
	bl Func_02003a44
.L_02009eca:
	add sp, #20
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02009ed4:
	.4byte gPartyState
.L_02009ed8:
	.4byte Data_02003de4
	.section .text.x02009edc,"ax",%progbits
	.global Func_02001edc
	.thumb_func
Func_02001edc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009fa4
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #20
	bl Object_GetById
	add r2, sp, #8
	adds r1, r0, #0
	ldr r3, [r1, #8]
	mov r10, r2
	ldr r2, .L_02009fa8
	mov r0, r10
	adds r3, r3, r2
	mov r2, r10
	str r3, [r2]
	movs r6, #99
	ldr r3, [r1, #12]
	str r3, [r2, #4]
	ldr r3, [r1, #16]
	str r3, [r2, #8]
	bl Func_02000374
	adds r5, r0, #0
	bl Func_0200074c
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldr r3, [r5, #20]
	adds r7, r5, #0
	adds r7, #34
	asrs r1, r1, #20
	asrs r2, r2, #20
	asrs r3, r3, #19
	ldrb r0, [r7]
	bl Func_020005cc
	movs r3, #38
	str r3, [sp, #4]
	movs r0, #94
	movs r1, #44
	movs r2, #5
	mov r8, r3
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02003aa4
	bl Func_020003b4
	movs r3, #0
	mov r1, r8
	strb r3, [r7]
	movs r0, #99
	str r1, [sp, #4]
	movs r2, #5
	movs r1, #42
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02003aa4
	ldr r3, [r5, #20]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	asrs r3, r3, #19
	asrs r1, r1, #20
	asrs r2, r2, #20
	adds r3, #4
	ldrb r0, [r7]
	bl Func_020005cc
	ldr r0, .L_02009fac
	bl Func_020006cc
	ldr r2, [r5, #8]
	mov r3, r10
	asrs r2, r2, #20
	str r2, [r3]
	mov r1, r10
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	str r3, [r1, #8]
	cmp r2, #35
	bne .L_02009f9a
	cmp r3, #38
	bne .L_02009f9a
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #219
	bl Func_02003a44
.L_02009f9a:
	add sp, #20
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02009fa4:
	.4byte gPartyState
.L_02009fa8:
	.4byte 0xfff00000
.L_02009fac:
	.4byte Data_02003de8
	.section .text.x02009fb0,"ax",%progbits
	.global Func_02001fb0
	.thumb_func
Func_02001fb0:
	push {lr}
	sub sp, #8
	movs r3, #123
	movs r2, #48
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #48
	movs r2, #1
	movs r3, #3
	movs r0, #121
	bl Func_02003aa4
	movs r0, #148
	lsls r0, r0, #2
	bl Func_02003a44
	add sp, #8
	pop {pc}
	.section .text.x02009fd4,"ax",%progbits
	.global Func_02001fd4
	.thumb_func
Func_02001fd4:
	push {lr}
	sub sp, #8
	movs r3, #125
	movs r2, #48
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #48
	movs r2, #1
	movs r3, #3
	movs r0, #121
	bl Func_02003aa4
	movs r0, #169
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003a44
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02009ffc,"ax",%progbits
	.global Func_02001ffc
	.thumb_func
Func_02001ffc:
	push {lr}
	sub sp, #8
	movs r3, #109
	movs r2, #56
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #56
	movs r2, #1
	movs r3, #3
	movs r0, #108
	bl Func_02003aa4
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #82
	bl Func_02003a44
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x0200a024,"ax",%progbits
	.global Func_02002024
	.thumb_func
Func_02002024:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #68
	movs r3, #128
	add r7, sp, #28
	lsls r3, r3, #10
	str r3, [r7, #8]
	str r3, [r7, #12]
	movs r3, #148
	adds r3, #255
	strh r3, [r7, #24]
	ldr r3, .L_0200a0c8
	movs r2, #1
	str r2, [r7]
	str r3, [r7, #28]
	movs r3, #0
	mov r11, r0
	mov r9, r1
	mov r10, r3
.L_0200a054:
	bl Random16Far
	movs r3, #63
	ands r0, r3
	ldr r6, .L_0200a0cc
	lsls r0, r0, #16
	add r0, r11
	add r5, sp, #16
	adds r0, r0, r6
	mov r8, r3
	str r0, [r5]
	bl Random16Far
	movs r3, #15
	ands r3, r0
	lsls r3, r3, #12
	str r3, [r5, #4]
	bl Random16Far
	mov r3, r8
	adds r2, r0, #0
	ands r2, r3
	lsls r2, r2, #16
	ldr r3, [r5, #4]
	add r2, r9
	adds r2, r2, r6
	str r2, [r5, #8]
	ldr r0, [r5]
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #232
	lsls r3, r3, #14
	adds r3, #1
	str r3, [sp, #8]
	movs r1, #0
	movs r3, #0
	str r7, [sp, #12]
	bl Func_0200015c
	movs r3, #1
	add r10, r3
	mov r3, r10
	cmp r3, #4
	bne .L_0200a054
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #220
	bl Func_02003a44
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a0c8:
	.4byte Data_02003d50
.L_0200a0cc:
	.4byte 0xffe00000
	.section .text.x0200a0d0,"ax",%progbits
	.global Func_020020d0
	.thumb_func
Func_020020d0:
	push {lr}
	ldr r3, .L_0200a0f4
	adds r2, r0, #0
	adds r2, #99
	ldrb r2, [r2]
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200a0ea
	movs r1, #7
	bl Func_02003b34
	b .L_0200a0f0
.L_0200a0ea:
	movs r1, #0
	bl Func_02003b34
.L_0200a0f0:
	pop {pc}
	.2byte 0x0000
.L_0200a0f4:
	.4byte Data_0300122c
	.section .text.x0200a0f8,"ax",%progbits
	.global Func_020020f8
	.thumb_func
Func_020020f8:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r7, r1, #0
	sub sp, #8
	adds r6, r0, #0
	ldr r5, [r3]
	cmp r7, #0
	bne .L_0200a11e
	bl Func_020038dc
	ldr r3, [r5, #20]
	movs r2, #4
	adds r1, r3, #0
	adds r1, #99
	strb r2, [r1]
	ldr r2, .L_0200a1e0
	str r2, [r3, #108]
.L_0200a11e:
	cmp r6, #2
	bne .L_0200a140
	ldr r3, [r5, #20]
	ldr r0, [r3, #8]
	ldr r1, [r3, #16]
	bl Func_02002024
	ldr r3, [r5, #20]
	movs r2, #240
	ldr r1, [r3, #12]
	lsls r2, r2, #12
	ldr r0, [r3, #8]
	adds r1, r1, r2
	ldr r2, [r3, #16]
	movs r3, #1
	bl Func_02003820
.L_0200a140:
	cmp r6, #3
	bne .L_0200a1ce
	ldr r3, [r5, #20]
	movs r6, #8
	ldr r0, [r3, #8]
	ldr r1, [r3, #16]
	bl Func_02002024
	ldr r3, [r5, #20]
	ldr r0, [r3, #8]
	ldr r1, [r3, #16]
	bl Func_02002024
	ldr r3, [r5, #20]
	ldr r0, [r3, #8]
	ldr r1, [r3, #16]
	ldr r3, .L_0200a1e4
	adds r1, r1, r3
	bl Func_02002024
	ldr r3, [r5, #20]
	movs r2, #240
	ldr r1, [r3, #12]
	lsls r2, r2, #12
	ldr r0, [r3, #8]
	adds r1, r1, r2
	ldr r2, [r3, #16]
	movs r3, #16
	bl Func_02003820
	ldr r2, [r5, #20]
	movs r3, #0
	str r3, [r2, #8]
	str r3, [r2, #12]
	str r3, [r2, #16]
	movs r3, #17
	str r3, [sp, #4]
	movs r5, #42
	movs r0, #0
	movs r1, #32
	movs r2, #5
	movs r3, #8
	str r5, [sp, #0]
	bl Func_02003aa4
	movs r3, #81
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #96
	movs r2, #5
	movs r3, #8
	str r5, [sp, #0]
	bl Func_02003aa4
	movs r5, #5
	movs r0, #0
	movs r1, #32
	movs r2, #42
	movs r3, #17
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003a94
	movs r0, #0
	movs r1, #96
	movs r2, #42
	movs r3, #81
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003a94
.L_0200a1ce:
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	cmp r7, r3
	bne .L_0200a1dc
	bl Func_02003998
.L_0200a1dc:
	add sp, #8
	pop {r5, r6, r7, pc}
.L_0200a1e0:
	.4byte Func_020020d0
.L_0200a1e4:
	.4byte 0xffe00000
	.section .text.x0200a1e8,"ax",%progbits
	.global Func_020021e8
	.thumb_func
Func_020021e8:
	push {lr}
	ldr r1, .L_0200a230
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_0200a234
	cmp r2, r3
	bne .L_0200a222
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #3
	bgt .L_0200a20e
	ldr r0, .L_0200a238
	b .L_0200a22e
.L_0200a20e:
	cmp r3, #8
	bgt .L_0200a216
	ldr r0, .L_0200a23c
	b .L_0200a22e
.L_0200a216:
	cmp r3, #12
	bgt .L_0200a21e
	ldr r0, .L_0200a240
	b .L_0200a22e
.L_0200a21e:
	ldr r0, .L_0200a244
	b .L_0200a22e
.L_0200a222:
	ldr r3, .L_0200a248
	cmp r2, r3
	bne .L_0200a22c
	ldr r0, .L_0200a24c
	b .L_0200a22e
.L_0200a22c:
	ldr r0, .L_0200a250
.L_0200a22e:
	pop {pc}
.L_0200a230:
	.4byte gPartyState
.L_0200a234:
	.4byte 0x000000e0
.L_0200a238:
	.4byte Data_02004720
.L_0200a23c:
	.4byte Data_020047e0
.L_0200a240:
	.4byte Data_0200487c
.L_0200a244:
	.4byte Data_02004930
.L_0200a248:
	.4byte 0x000000e1
.L_0200a24c:
	.4byte Data_02004a5c
.L_0200a250:
	.4byte Data_0200460c
	.section .text.x0200a254,"ax",%progbits
	.global Func_02002254
	.thumb_func
Func_02002254:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	adds r3, r3, r7
	movs r1, #18
	str r3, [r6, #16]
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0200a284
	adds r3, #15
.L_0200a284:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a2ac,"ax",%progbits
	.global Func_020022ac
	.thumb_func
Func_020022ac:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a438
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02003aec
	movs r0, #0
	bl Func_02003bcc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Func_02003b64
	bl Func_02003a7c
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #12]
	movs r4, #130
	lsls r4, r4, #16
	adds r3, r3, r4
	str r3, [r7, #12]
	movs r3, #128
	lsls r3, r3, #8
	adds r5, r7, #0
	str r3, [r7, #72]
	adds r5, #85
	movs r3, #0
	str r3, [r7, #68]
	strb r3, [r5]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #204
	bl Func_02003c54
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r3, sp, #28
	mov r8, r3
	mov r4, r8
	movs r3, #7
	str r3, [r4, #4]
	ldr r3, .L_0200a43c
	movs r2, #0
	str r3, [r4, #36]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r4, #8]
	str r3, [r4, #12]
	mov r10, r2
.L_0200a34a:
	mov r3, r10
	lsls r5, r3, #12
	adds r0, r5, #0
	bl Math_Cosine
	add r6, sp, #16
	movs r3, #0
	str r0, [r6]
	adds r0, r5, #0
	str r3, [r6, #4]
	bl Math_Sine
	ldr r3, [r6]
	str r0, [r6, #8]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r6]
	adds r3, r3, r0
	ldr r4, .L_0200a440
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	adds r2, r2, r4
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r2, .L_0200a444
	lsls r3, r3, #13
	lsrs r3, r3, #16
	ldr r4, [r6, #4]
	adds r5, r5, r3
	adds r5, r5, r2
	str r5, [r6, #8]
	ldr r2, [r7, #16]
	ldr r3, [r6]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r4, [sp, #0]
	ldr r4, .L_0200a448
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_0200015c
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200a34a
	movs r0, #188
	bl Func_02003c54
	ldr r5, .L_0200a438
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02003b44
	ldr r0, [r5]
	movs r1, #22
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02003ac4
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02003ac4
	bl Func_02003acc
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02003b44
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	bl Func_02003af4
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a438:
	.4byte gPartyState
.L_0200a43c:
	.4byte Func_02002254
.L_0200a440:
	.4byte 0xffffa000
.L_0200a444:
	.4byte 0xffffd000
.L_0200a448:
	.4byte 0x01090001
	.section .text.x0200a44c,"ax",%progbits
	.global Func_0200244c
	.thumb_func
Func_0200244c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a4c4
	adds r7, r0, #0
	mov r8, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r8
	ldr r0, [r3]
	mov r10, r3
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #10
	adds r0, #255
	bl Func_02003a3c
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200a4bc
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, [r5, #12]
	movs r2, #128
	adds r3, r3, r7
	str r3, [r5, #12]
	lsls r2, r2, #2
	movs r3, #128
	lsls r3, r3, #9
	adds r2, #18
	str r3, [r5, #48]
	add r2, r8
	movs r3, #2
	str r6, [r5, #40]
	strb r3, [r2]
	adds r2, r5, #0
	adds r2, #90
	movs r3, #1
	strb r3, [r2]
	mov r3, r10
	ldr r0, [r3]
	movs r1, #0
	bl Func_02003b54
	bl Func_02003a7c
	movs r0, #1
	bl WaitFrames
.L_0200a4bc:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a4c4:
	.4byte gPartyState
	.section .text.x0200a4c8,"ax",%progbits
	.global Func_020024c8
	.thumb_func
Func_020024c8:
	push {r5, r6, lr}
	ldr r3, .L_0200a514
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #10
	adds r0, #255
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200a510
	movs r0, #1
	bl WaitFrames
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Map_GetTerrainHeight
	movs r3, #0
	str r3, [r5, #40]
	movs r3, #128
	lsls r3, r3, #24
	str r0, [r5, #20]
	str r0, [r5, #12]
	str r3, [r5, #60]
	movs r1, #0
	ldr r0, [r6]
	bl Func_02003b54
.L_0200a510:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a514:
	.4byte gPartyState
	.section .text.x0200a518,"ax",%progbits
	.global Func_02002518
	.thumb_func
Func_02002518:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	lsls r0, r0, #16
	lsrs r5, r0, #16
	adds r0, r5, #0
	sub sp, #16
	bl Object_GetById
	ldr r3, .L_0200a610
	ldr r2, [r0, #108]
	cmp r2, r3
	bne .L_0200a604
	adds r0, r5, #0
	bl Object_GetById
	ldr r0, [r0, #104]
	ldr r1, [r0, #8]
	mov r8, r0
	mov r3, r8
	movs r0, #128
	ldr r2, [r3, #12]
	lsls r0, r0, #9
	adds r1, r1, r0
	movs r0, #128
	lsls r0, r0, #14
	adds r2, r2, r0
	movs r0, #96
	ldr r3, [r3, #16]
	adds r0, #255
	bl Func_02003a64
	mov r7, sp
	str r0, [r7]
	movs r3, #1
	mov r10, r3
.L_0200a562:
	ldr r3, [r7]
	movs r0, #96
	ldr r6, [r3, #80]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	adds r0, #255
	bl Func_02003a64
	mov r3, r10
	ldr r1, [r0, #80]
	lsls r5, r3, #2
	ldrb r3, [r6, #16]
	movs r2, #1
	strb r3, [r1, #16]
	ldrb r3, [r1, #17]
	str r0, [r7, r5]
	orrs r3, r2
	strb r3, [r1, #17]
	ldr r2, [r7]
	movs r6, #0
	ldr r3, [r2, #8]
	movs r1, #0
	str r3, [r0, #8]
	ldr r3, [r2, #12]
	str r3, [r0, #12]
	ldr r3, [r2, #16]
	str r3, [r0, #16]
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r7, r5]
	movs r1, #3
	bl Func_02003a54
	ldr r0, [r7, r5]
	mov r3, r10
	adds r2, r0, #0
	ldr r5, .L_0200a614
	adds r3, #255
	adds r2, #98
	strb r3, [r2]
	mov r3, r8
	str r3, [r0, #104]
	str r5, [r0, #108]
	movs r1, #3
	bl Func_02003b34
	movs r0, #1
	add r10, r0
	mov r3, r10
	cmp r3, #4
	bne .L_0200a562
	ldr r0, [r7]
	movs r1, #0
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r7]
	movs r1, #3
	bl Func_02003a54
	ldr r0, [r7]
	movs r1, #3
	bl Func_02000038
	ldr r0, [r7]
	movs r3, #3
	adds r2, r0, #0
	adds r2, #98
	strb r3, [r2]
	mov r3, r8
	str r3, [r0, #104]
	str r5, [r0, #108]
	movs r1, #3
	bl Func_02003b34
.L_0200a604:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a610:
	.4byte Func_020015e4
.L_0200a614:
	.4byte Func_02001528
	.section .text.x0200a618,"ax",%progbits
	.global Func_02002618
	.thumb_func
Func_02002618:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r1, .L_0200a664
	movs r0, #214
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	str r2, [r3]
	mov r8, r1
	movs r3, #152
	ldr r1, .L_0200a668
	lsls r3, r3, #2
	add r3, r8
	adds r2, #94
	strh r1, [r3]
	add r2, r8
	movs r3, #1
	strh r3, [r2]
	movs r3, #240
	lsls r3, r3, #1
	add r3, r8
	movs r0, #0
	ldrsh r2, [r3, r0]
	sub sp, #8
	ldr r7, .L_0200a660
	cmp r2, r1
	beq .L_0200a65e
	b .L_0200a780
.L_0200a65e:
	b .L_0200a66c
.L_0200a660:
	.4byte 0x00000000
.L_0200a664:
	.4byte gPartyState
.L_0200a668:
	.4byte 0x000000df
.L_0200a66c:
	movs r3, #133
	lsls r3, r3, #2
	add r3, r8
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #32
	orrs r3, r5
	strb r3, [r0]
	movs r0, #8
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #10
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	ldr r0, .L_0200a9fc
	bl Func_02000668
	movs r0, #10
	adds r0, #255
	bl Func_02003a3c
	cmp r0, #0
	bne .L_0200a6ba
	movs r0, #21
	movs r1, #13
	movs r2, #0
	bl Func_02000ee4
.L_0200a6ba:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #213
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200a6d6
	movs r1, #132
	movs r2, #180
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02003b0c
.L_0200a6d6:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #214
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200a6f2
	movs r1, #248
	movs r2, #204
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02003b0c
.L_0200a6f2:
	ldr r6, .L_0200aa00
	movs r5, #78
	adds r0, r6, #0
	bl Func_02003c0c
	movs r3, #21
	str r3, [sp, #4]
	movs r0, #98
	movs r1, #21
	movs r2, #4
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02003aa4
	movs r3, #24
	str r3, [sp, #4]
	movs r0, #98
	movs r1, #27
	movs r2, #4
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02003aa4
	adds r0, r6, #0
	bl Func_020006cc
	movs r0, #10
	adds r0, #255
	bl Func_02003a3c
	cmp r0, #0
	bne .L_0200a75e
	movs r0, #20
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r1, #128
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r0, #8]
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	adds r3, #4
	strb r7, [r3]
	movs r2, #192
	ldr r3, [r0, #12]
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r0, #12]
	str r3, [r0, #20]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_0200a75e:
	movs r0, #20
	bl Func_02002518
	movs r3, #241
	lsls r3, r3, #1
	add r3, r8
	movs r0, #0
	ldrsh r2, [r3, r0]
	cmp r2, #4
	ble .L_0200a774
	b .L_0200ac20
.L_0200a774:
	cmp r2, #3
	bge .L_0200a77a
	b .L_0200ac20
.L_0200a77a:
	movs r0, #128
	lsls r0, r0, #13
	b .L_0200ac1c
.L_0200a780:
	ldr r3, .L_0200aa04
	cmp r2, r3
	beq .L_0200a788
	b .L_0200ac26
.L_0200a788:
	movs r1, #133
	lsls r1, r1, #2
	add r1, r8
	ldr r0, [r1]
	mov r10, r1
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r2, #241
	lsls r2, r2, #1
	add r2, r8
	strb r3, [r0]
	movs r0, #0
	ldrsh r3, [r2, r0]
	mov r9, r2
	cmp r3, #3
	bgt .L_0200a848
	ldr r0, .L_0200aa08
	bl Func_02002d08
	ldr r0, .L_0200aa0c
	bl Func_02000668
	bl Func_02003bec
	movs r0, #0
	movs r1, #11
	movs r2, #12
	bl Func_02003bf4
	movs r0, #251
	lsls r0, r0, #3
	adds r0, #255
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200a7e6
	movs r1, #220
	movs r2, #134
	movs r0, #15
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02003b0c
.L_0200a7e6:
	ldr r0, .L_0200aa10
	bl Func_02003c0c
	mov r2, r9
	movs r1, #0
	ldrsh r3, [r2, r1]
	cmp r3, #1
	bne .L_0200a81c
	movs r0, #10
	adds r0, #255
	bl Func_02003a3c
	cmp r0, #0
	bne .L_0200a814
	mov r3, r10
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r0, #16]
.L_0200a814:
	ldr r0, .L_0200aa14
	bl Func_0200244c
	b .L_0200acda
.L_0200a81c:
	cmp r3, #2
	beq .L_0200a822
	b .L_0200acda
.L_0200a822:
	movs r0, #10
	adds r0, #255
	bl Func_02003a3c
	cmp r0, #0
	bne .L_0200a840
	mov r2, r10
	ldr r0, [r2]
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r0, #16]
.L_0200a840:
	ldr r0, .L_0200aa18
	bl Func_0200244c
	b .L_0200acda
.L_0200a848:
	cmp r3, #8
	ble .L_0200a84e
	b .L_0200aa28
.L_0200a84e:
	movs r0, #10
	adds r0, #255
	bl Func_02003a3c
	cmp r0, #0
	bne .L_0200a908
	movs r0, #8
	bl Object_GetById
	adds r0, #85
	strb r7, [r0]
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r5, #160
	lsls r5, r5, #15
	adds r3, r3, r5
	str r3, [r0, #12]
	movs r0, #9
	bl Object_GetById
	adds r0, #85
	strb r7, [r0]
	movs r0, #9
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r1, #8
	adds r3, r3, r5
	movs r2, #0
	str r3, [r0, #12]
	movs r0, #19
	bl Func_02000ee4
	movs r0, #10
	bl Object_GetById
	adds r0, #85
	strb r7, [r0]
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r5, #160
	lsls r5, r5, #16
	adds r3, r3, r5
	str r3, [r0, #12]
	movs r0, #11
	bl Object_GetById
	adds r0, #85
	strb r7, [r0]
	movs r0, #11
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r1, #10
	adds r3, r3, r5
	movs r2, #1
	str r3, [r0, #12]
	movs r0, #20
	bl Func_02000ee4
	movs r0, #12
	bl Object_GetById
	adds r0, #85
	strb r7, [r0]
	movs r0, #12
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r5, #240
	lsls r5, r5, #16
	adds r3, r3, r5
	str r3, [r0, #12]
	movs r0, #13
	bl Object_GetById
	adds r0, #85
	strb r7, [r0]
	movs r0, #13
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r1, #12
	adds r3, r3, r5
	str r3, [r0, #12]
	movs r2, #0
	movs r0, #21
	bl Func_02000ee4
.L_0200a908:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #216
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200a924
	movs r1, #222
	movs r2, #142
	movs r0, #16
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02003b0c
.L_0200a924:
	ldr r0, .L_0200aa1c
	bl Func_02003c0c
	ldr r0, .L_0200aa20
	bl Func_02000668
	movs r0, #10
	adds r0, #255
	bl Func_02003a3c
	cmp r0, #0
	bne .L_0200a98c
	movs r0, #15
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r6, #128
	lsls r6, r6, #9
	adds r3, r3, r6
	str r3, [r0, #8]
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	adds r3, #4
	strb r7, [r3]
	movs r5, #192
	ldr r3, [r0, #12]
	lsls r5, r5, #13
	adds r3, r3, r5
	str r3, [r0, #12]
	str r3, [r0, #20]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #18
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r1, #0
	adds r3, r3, r6
	str r3, [r0, #8]
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	adds r3, #4
	strb r7, [r3]
	ldr r3, [r0, #12]
	adds r3, r3, r5
	str r3, [r0, #12]
	str r3, [r0, #20]
	bl ObjectDispatch_SetSingleChildField26
.L_0200a98c:
	movs r0, #15
	bl Func_02002518
	movs r0, #18
	bl Func_02002518
	mov r0, r9
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, #5
	bne .L_0200a9ac
	movs r0, #144
	lsls r0, r0, #17
	bl Func_0200244c
	b .L_0200ac20
.L_0200a9ac:
	cmp r3, #6
	bne .L_0200a9c4
	movs r0, #10
	adds r0, #255
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200a9be
	b .L_0200ac20
.L_0200a9be:
	bl Func_020022ac
	b .L_0200ac20
.L_0200a9c4:
	cmp r3, #7
	bne .L_0200a9ee
	movs r0, #10
	adds r0, #255
	bl Func_02003a3c
	cmp r0, #0
	bne .L_0200a9e6
	mov r1, r10
	ldr r0, [r1]
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r0, #16]
.L_0200a9e6:
	ldr r0, .L_0200aa24
	bl Func_0200244c
	b .L_0200ac20
.L_0200a9ee:
	cmp r3, #8
	beq .L_0200a9f4
	b .L_0200ac20
.L_0200a9f4:
	movs r0, #208
	lsls r0, r0, #15
	b .L_0200ac1c
	.2byte 0x0000
.L_0200a9fc:
	.4byte Data_02003d9e
.L_0200aa00:
	.4byte Data_02003d98
.L_0200aa04:
	.4byte 0x000000e0
.L_0200aa08:
	.4byte Data_02003dba
.L_0200aa0c:
	.4byte Data_02003db2
.L_0200aa10:
	.4byte Data_02003dac
.L_0200aa14:
	.4byte 0xfff00000
.L_0200aa18:
	.4byte 0xffa00000
.L_0200aa1c:
	.4byte Data_02003dd2
.L_0200aa20:
	.4byte Data_02003dc4
.L_0200aa24:
	.4byte 0xffd00000
.L_0200aa28:
	cmp r3, #12
	bgt .L_0200aaee
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #217
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200aa48
	movs r1, #132
	movs r2, #146
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02003b0c
.L_0200aa48:
	ldr r5, .L_0200ace8
	adds r0, r5, #0
	bl Func_02003c0c
	adds r0, r5, #0
	bl Func_020006cc
	bl Func_02003be4
	movs r1, #148
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #14
	movs r3, #13
	bl Func_02003bfc
	ldr r0, .L_0200acec
	bl Func_02000668
	movs r0, #10
	adds r0, #255
	bl Func_02003a3c
	cmp r0, #0
	bne .L_0200aaca
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r6, #128
	lsls r6, r6, #9
	adds r3, r3, r6
	str r3, [r0, #8]
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	adds r3, #4
	strb r7, [r3]
	movs r5, #192
	ldr r3, [r0, #12]
	lsls r5, r5, #13
	adds r3, r3, r5
	str r3, [r0, #12]
	str r3, [r0, #20]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #12
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r1, #0
	adds r3, r3, r6
	str r3, [r0, #8]
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	adds r3, #4
	strb r7, [r3]
	ldr r3, [r0, #12]
	adds r3, r3, r5
	str r3, [r0, #12]
	str r3, [r0, #20]
	bl ObjectDispatch_SetSingleChildField26
.L_0200aaca:
	movs r0, #10
	bl Func_02002518
	movs r0, #12
	bl Func_02002518
	mov r0, r9
	ldrh r3, [r0]
	movs r1, #128
	subs r3, #9
	lsls r3, r3, #16
	lsls r1, r1, #9
	cmp r3, r1
	bls .L_0200aae8
	b .L_0200ac20
.L_0200aae8:
	movs r0, #192
	lsls r0, r0, #14
	b .L_0200ac1c
.L_0200aaee:
	ldr r0, .L_0200acf0
	bl Func_02000668
	movs r0, #10
	adds r0, #255
	bl Func_02003a3c
	cmp r0, #0
	bne .L_0200ab2c
	movs r0, #13
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r0, #8]
	adds r3, r0, #0
	adds r3, #85
	strb r7, [r3]
	adds r3, #4
	strb r7, [r3]
	movs r1, #192
	ldr r3, [r0, #12]
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r0, #12]
	str r3, [r0, #20]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_0200ab2c:
	movs r0, #13
	bl Func_02002518
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #218
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200ab4e
	movs r1, #162
	movs r2, #150
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02003b0c
.L_0200ab4e:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #219
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200ab6a
	movs r1, #142
	movs r2, #154
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02003b0c
.L_0200ab6a:
	ldr r0, .L_0200acf4
	bl Func_0200079c
	movs r0, #148
	lsls r0, r0, #2
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200ab90
	movs r3, #123
	movs r2, #48
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #121
	movs r1, #48
	movs r2, #1
	movs r3, #3
	bl Func_02003aa4
.L_0200ab90:
	movs r0, #169
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200abb2
	movs r3, #125
	movs r2, #48
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #121
	movs r1, #48
	movs r2, #1
	movs r3, #3
	bl Func_02003aa4
.L_0200abb2:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #82
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200abd4
	movs r3, #109
	movs r2, #56
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #108
	movs r1, #56
	movs r2, #1
	movs r3, #3
	bl Func_02003aa4
.L_0200abd4:
	mov r3, r9
	ldrh r2, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #14
	bne .L_0200abea
	movs r0, #192
	lsls r0, r0, #14
	bl Func_0200244c
	b .L_0200ac20
.L_0200abea:
	adds r3, r2, #0
	subs r3, #16
	movs r1, #128
	lsls r3, r3, #16
	lsls r1, r1, #9
	cmp r3, r1
	bhi .L_0200ac20
	movs r0, #10
	adds r0, #255
	bl Func_02003a3c
	cmp r0, #0
	bne .L_0200ac1a
	movs r3, #133
	lsls r3, r3, #2
	add r3, r8
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r0, #16]
.L_0200ac1a:
	ldr r0, .L_0200acf8
.L_0200ac1c:
	bl Func_0200244c
.L_0200ac20:
	bl Func_020024c8
	b .L_0200acda
.L_0200ac26:
	ldr r3, .L_0200acfc
	cmp r2, r3
	bne .L_0200acda
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #220
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200ac8e
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02003b0c
	movs r3, #17
	str r3, [sp, #4]
	movs r5, #42
	movs r0, #0
	movs r1, #32
	movs r2, #5
	movs r3, #8
	str r5, [sp, #0]
	bl Func_02003aa4
	movs r3, #81
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #96
	movs r2, #5
	movs r3, #8
	str r5, [sp, #0]
	bl Func_02003aa4
	movs r5, #5
	movs r6, #8
	movs r0, #0
	movs r1, #32
	movs r2, #42
	movs r3, #17
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003a94
	movs r0, #0
	movs r1, #96
	movs r2, #42
	movs r3, #81
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003a94
.L_0200ac8e:
	movs r5, #133
	lsls r5, r5, #2
	add r5, r8
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	ldr r0, .L_0200ad00
	bl Func_02000668
	movs r3, #241
	lsls r3, r3, #1
	add r3, r8
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #2
	bgt .L_0200acda
	movs r0, #10
	adds r0, #255
	bl Func_02003a3c
	cmp r0, #0
	bne .L_0200acd4
	ldr r0, [r5]
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r0, #16]
.L_0200acd4:
	ldr r0, .L_0200acf8
	bl Func_0200244c
.L_0200acda:
	movs r0, #0
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200ace8:
	.4byte Data_02003de4
.L_0200acec:
	.4byte Data_02003dd6
.L_0200acf0:
	.4byte Data_02003dee
.L_0200acf4:
	.4byte Data_02003de8
.L_0200acf8:
	.4byte 0xffe00000
.L_0200acfc:
	.4byte 0x000000e1
.L_0200ad00:
	.4byte Data_02003e02
	.section .text.x0200ad04,"ax",%progbits
	.global Func_02002d04
	.thumb_func
Func_02002d04:
	movs r0, #0
	bx lr
	.section .text.x0200ad08,"ax",%progbits
	.global Func_02002d08
	.thumb_func
Func_02002d08:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #20
	str r3, [sp, #16]
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	mov r11, r0
	cmp r3, r2
	beq .L_0200adee
.L_0200ad2e:
	mov r3, r11
	ldrh r3, [r3]
	adds r0, r3, #0
	str r3, [sp, #12]
	bl Object_GetById
	mov r2, r11
	ldrh r2, [r2, #2]
	adds r7, r0, #0
	str r2, [sp, #8]
	movs r3, #34
	adds r3, r3, r7
	adds r0, r2, #0
	ldrb r2, [r3]
	mov r9, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [sp, #16]
	adds r0, #1
	ldr r5, [r2, r3]
	ldr r2, .L_0200ae24
	adds r3, r5, r2
	ldr r2, .L_0200ae28
	asrs r3, r3, #2
	adds r6, r3, r2
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200ad7c
	ldr r0, [sp, #12]
	movs r1, #0
	movs r2, #0
	bl Func_02003b0c
	b .L_0200addc
.L_0200ad7c:
	adds r0, r7, #0
	bl Func_02003c04
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r5, r5, r3
	str r5, [sp, #4]
	mov r2, r9
	ldrb r0, [r2]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	bl Func_02003adc
	mov r3, r9
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	mov r10, r0
	ldrb r0, [r3]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	ldr r0, [sp, #12]
	bl Func_02003b3c
	ldr r2, [sp, #4]
	movs r3, #128
	asrs r5, r5, #19
	strb r3, [r2, #3]
	adds r5, #4
	mov r3, r9
	adds r2, r5, #0
	ldrb r0, [r3]
	mov r1, r10
	bl Func_02003c24
	add r8, r6
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200addc
	adds r0, r7, #0
	movs r1, #0
	bl Func_02003a54
.L_0200addc:
	movs r3, #4
	add r11, r3
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200ad2e
.L_0200adee:
	ldr r3, .L_0200ae2c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	cmp r3, r0
	bge .L_0200ae16
	str r0, [r5, #20]
	str r0, [r5, #12]
.L_0200ae16:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200ae24:
	.4byte 0xfdff0000
.L_0200ae28:
	.4byte Data_02024000
.L_0200ae2c:
	.4byte gPartyState
	.section .text.x0200ae30,"ax",%progbits
	.global Func_02002e30
	.thumb_func
Func_02002e30:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #12
	adds r5, r0, #0
	bl Func_02003b8c
	cmp r0, #0
	beq .L_0200ae46
	b .L_0200afb6
.L_0200ae46:
	ldr r3, .L_0200afc0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	mov r0, sp
	str r3, [r0]
	movs r1, #0
	ldr r3, [r6, #12]
	str r3, [r0, #4]
	ldr r3, [r6, #16]
	str r3, [r0, #8]
	bl Func_02003c2c
	mov r8, r0
	cmp r0, #0
	bne .L_0200ae72
	b .L_0200afb6
.L_0200ae72:
	b .L_0200afa8
.L_0200ae74:
	ldrh r7, [r5]
	adds r0, r7, #0
	bl Object_GetById
	cmp r0, r8
	beq .L_0200ae84
	adds r5, #4
	b .L_0200afa8
.L_0200ae84:
	ldrh r5, [r5, #2]
	bl Func_02003aec
	adds r0, r5, #0
	bl Func_02003a3c
	cmp r0, #0
	bne .L_0200aef2
	movs r0, #125
	bl Func_02003c54
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #7
	bl Func_02003b34
	movs r0, #2
	bl WaitFrames
	movs r1, #0
	mov r0, r8
	bl Func_02003a54
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #0
	bl Func_02003b34
	movs r0, #2
	bl WaitFrames
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #7
	bl Func_02003b34
	movs r0, #4
	bl WaitFrames
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #0
	bl Func_02003b34
	movs r0, #0
	bl Func_020034ac
	adds r0, r5, #0
	bl Func_02003a44
	b .L_0200afa2
.L_0200aef2:
	adds r5, #1
	mov r10, r5
	mov r0, r10
	bl Func_02003a3c
	cmp r0, #0
	bne .L_0200afa2
	adds r6, #85
	strb r0, [r6]
	movs r0, #185
	bl Func_02003c54
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02003ac4
	movs r0, #0
	bl Func_020034ac
	movs r5, #2
	movs r0, #8
	mov r7, r8
	bl WaitFrames
	negs r5, r5
	mov r0, r8
	movs r1, #2
	adds r7, #34
	bl Func_02003a54
	ldrb r1, [r7]
	adds r0, r5, #0
	bl Func_02003784
	movs r0, #1
	bl Func_020034ac
	movs r0, #16
	bl WaitFrames
	ldrb r1, [r7]
	adds r0, r5, #0
	bl Func_02003784
	movs r0, #4
	bl WaitFrames
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02003ac4
	movs r0, #8
	bl WaitFrames
	movs r3, #3
	strb r3, [r6]
	movs r0, #5
	bl WaitFrames
	movs r1, #0
	movs r2, #0
	movs r3, #0
	mov r0, r8
	bl Object_SetPositionAndResetMotion
	movs r0, #2
	bl WaitFrames
	movs r0, #188
	bl Func_02003c54
	bl Func_020035e0
	movs r0, #20
	bl WaitFrames
	mov r0, r10
	bl Func_02003a44
.L_0200afa2:
	bl Func_02003af4
	b .L_0200afb6
.L_0200afa8:
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_0200afb6
	b .L_0200ae74
.L_0200afb6:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200afc0:
	.4byte gPartyState
	.section .text.x0200afc4,"ax",%progbits
	.global Func_02002fc4
	.thumb_func
Func_02002fc4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	adds r5, r0, #0
	adds r0, r6, #0
	bl Object_GetById
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	adds r7, r0, #0
	cmp r3, r2
	beq .L_0200b09e
.L_0200afe2:
	ldrh r3, [r5]
	cmp r3, r6
	beq .L_0200afec
	adds r5, #4
	b .L_0200b092
.L_0200afec:
	ldrh r5, [r5, #2]
	bl Func_02003aec
	adds r3, r5, #1
	mov r8, r3
	mov r0, r8
	bl Func_02003a3c
	cmp r0, #0
	bne .L_0200b08c
	movs r0, #185
	bl Func_02003c54
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02003ac4
	movs r0, #0
	bl Func_020034ac
	movs r0, #8
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #2
	bl Func_02003a54
	adds r3, r7, #0
	adds r3, #34
	movs r0, #4
	ldrb r1, [r3]
	adds r2, r6, #0
	negs r0, r0
	bl Func_020036f8
	movs r0, #1
	bl Func_020034ac
	movs r0, #16
	bl WaitFrames
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02003ac4
	movs r0, #8
	bl WaitFrames
	movs r1, #0
	movs r2, #0
	movs r3, #0
	adds r0, r7, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #2
	bl WaitFrames
	movs r0, #188
	bl Func_02003c54
	bl Func_020035e0
	movs r0, #20
	bl WaitFrames
	adds r0, r5, #0
	bl Func_02003a44
	mov r0, r8
	bl Func_02003a44
.L_0200b08c:
	bl Func_02003af4
	b .L_0200b09e
.L_0200b092:
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200afe2
.L_0200b09e:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x0200b0a4,"ax",%progbits
	.global Func_020030a4
	.thumb_func
Func_020030a4:
	push {r5, lr}
	adds r5, r0, #0
	bl Object_GetById
	movs r3, #3
	adds r0, #92
	strb r3, [r0]
	adds r0, r5, #0
	bl Func_02003b3c
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200b0bc,"ax",%progbits
	.global Func_020030bc
	.thumb_func
Func_020030bc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #16
	ldr r5, .L_0200b218
	str r3, [sp, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	mov r11, r0
	ldr r1, [r5]
	movs r0, #8
	bl Func_02003b14
	ldr r1, [r5]
	movs r0, #9
	bl Func_02003b14
	ldr r1, [r5]
	movs r0, #10
	bl Func_02003b14
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	bl Func_020030a4
	movs r0, #9
	bl Func_020030a4
	movs r0, #10
	bl Func_020030a4
	movs r1, #0
	movs r0, #9
	bl Object_SetModeById
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02003b0c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02003b0c
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_02003b0c
	movs r0, #1
	bl WaitFrames
	b .L_0200b1f6
.L_0200b142:
	mov r3, r11
	ldrh r3, [r3]
	mov r9, r3
	mov r0, r9
	bl Object_GetById
	mov r2, r11
	ldrh r2, [r2, #2]
	adds r5, r0, #0
	str r2, [sp, #8]
	adds r7, r5, #0
	adds r7, #34
	adds r0, r2, #0
	ldrb r2, [r7]
	adds r0, #1
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [sp, #12]
	ldr r6, [r2, r3]
	ldr r2, .L_0200b21c
	adds r3, r6, r2
	ldr r2, .L_0200b220
	asrs r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	adds r2, r5, #0
	adds r2, #92
	movs r3, #3
	strb r3, [r2]
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200b198
	mov r0, r9
	movs r1, #0
	movs r2, #0
	bl Func_02003b0c
	b .L_0200b1f2
.L_0200b198:
	adds r0, r5, #0
	bl Func_02003c04
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r6, r6, r3
	str r6, [sp, #4]
	add r8, r10
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r7]
	bl Func_02003adc
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	mov r10, r0
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	mov r0, r9
	bl Func_02003b3c
	ldr r6, [sp, #4]
	asrs r5, r5, #19
	movs r3, #128
	adds r5, #4
	adds r2, r5, #0
	strb r3, [r6, #3]
	ldrb r0, [r7]
	mov r1, r10
	bl Func_02003c24
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl Func_02003a3c
	cmp r0, #0
	beq .L_0200b1f2
	mov r0, r9
	movs r1, #9
	bl Func_02003b4c
.L_0200b1f2:
	movs r3, #4
	add r11, r3
.L_0200b1f6:
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200b142
	movs r0, #10
	bl WaitFrames
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200b218:
	.4byte gPartyState
.L_0200b21c:
	.4byte 0xfdff0000
.L_0200b220:
	.4byte Data_02024000
	.section .text.x0200b224,"ax",%progbits
	.global Func_02003224
	.thumb_func
Func_02003224:
	push {r5, lr}
	adds r5, r1, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	bl Object_SetPositionAndResetMotion
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Object_SetPositionAndResetMotion
	pop {r5, pc}
	.section .text.x0200b240,"ax",%progbits
	.global Func_02003240
	.thumb_func
Func_02003240:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #12
	adds r6, r0, #0
	bl Func_02003b8c
	cmp r0, #0
	beq .L_0200b258
	b .L_0200b41a
.L_0200b258:
	ldr r3, .L_0200b428
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #8
	bl Object_GetById
	ldr r3, [r5, #8]
	adds r7, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r1, #0
	ldr r3, [r5, #12]
	str r3, [r0, #4]
	ldr r3, [r5, #16]
	str r3, [r0, #8]
	bl Func_02003c2c
	mov r10, r0
	cmp r0, #0
	bne .L_0200b28c
	b .L_0200b41a
.L_0200b28c:
	b .L_0200b40c
.L_0200b28e:
	ldrh r3, [r6]
	mov r8, r3
	mov r0, r8
	bl Object_GetById
	cmp r0, r10
	beq .L_0200b2a0
	adds r6, #4
	b .L_0200b40c
.L_0200b2a0:
	ldrh r6, [r6, #2]
	bl Func_02003aec
	adds r0, r6, #0
	bl Func_02003a3c
	cmp r0, #0
	bne .L_0200b33a
	adds r0, r7, #0
	movs r1, #1
	bl Func_02003a54
	mov r1, r10
	adds r0, r7, #0
	bl Func_02003224
	movs r0, #1
	bl WaitFrames
	movs r0, #125
	bl Func_02003c54
	movs r0, #8
	bl Object_GetById
	movs r1, #7
	bl Func_02003b34
	movs r0, #2
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #0
	bl Func_02003a54
	movs r1, #9
	mov r0, r8
	bl Func_02003b4c
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl Func_02003b34
	movs r0, #2
	bl WaitFrames
	movs r0, #8
	bl Object_GetById
	movs r1, #7
	bl Func_02003b34
	movs r0, #4
	bl WaitFrames
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl Func_02003b34
	movs r0, #0
	bl Func_020034ac
	mov r0, r10
	adds r1, r7, #0
	bl Func_02003224
	movs r0, #1
	bl WaitFrames
	adds r0, r6, #0
	bl Func_02003a44
	b .L_0200b406
.L_0200b33a:
	adds r6, #1
	mov r9, r6
	mov r0, r9
	bl Func_02003a3c
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200b406
	adds r0, r7, #0
	movs r1, #0
	bl Func_02003a54
	mov r1, r10
	adds r0, r7, #0
	bl Func_02003224
	adds r5, #85
	movs r0, #1
	bl WaitFrames
	strb r6, [r5]
	movs r0, #185
	bl Func_02003c54
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02003ac4
	movs r0, #0
	bl Func_020034ac
	mov r8, r5
	movs r0, #8
	movs r6, #2
	mov r5, r10
	bl WaitFrames
	negs r6, r6
	adds r0, r7, #0
	movs r1, #2
	adds r5, #34
	bl Func_02003a54
	ldrb r1, [r5]
	adds r0, r6, #0
	bl Func_02003784
	movs r0, #1
	bl Func_020034ac
	movs r0, #16
	bl WaitFrames
	ldrb r1, [r5]
	adds r0, r6, #0
	bl Func_02003784
	movs r0, #4
	bl WaitFrames
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02003ac4
	movs r0, #8
	bl WaitFrames
	movs r3, #3
	mov r2, r8
	strb r3, [r2]
	movs r0, #5
	bl WaitFrames
	movs r1, #0
	movs r2, #0
	movs r3, #0
	adds r0, r7, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #2
	bl WaitFrames
	movs r0, #188
	bl Func_02003c54
	bl Func_020035e0
	movs r0, #20
	bl WaitFrames
	mov r0, r9
	bl Func_02003a44
.L_0200b406:
	bl Func_02003af4
	b .L_0200b41a
.L_0200b40c:
	movs r2, #255
	ldrh r3, [r6]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_0200b41a
	b .L_0200b28e
.L_0200b41a:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b428:
	.4byte gPartyState
	.section .text.x0200b42c,"ax",%progbits
	.global Func_0200342c
	.thumb_func
Func_0200342c:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	adds r3, r3, r7
	movs r1, #18
	str r3, [r6, #16]
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0200b45c
	adds r3, #15
.L_0200b45c:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	ldr r3, .L_0200b4a8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r4, [r6, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r4, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r4, #9]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b4a8:
	.4byte gPartyState
	.section .text.x0200b4ac,"ax",%progbits
	.global Func_020034ac
	.thumb_func
Func_020034ac:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0200b5b8
	mov r8, r0
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	movs r2, #0
	adds r7, r0, #0
	mov r9, r2
	mov r10, r2
.L_0200b4ce:
	bl Random16Far
	lsls r3, r0, #3
	subs r3, r3, r0
	ldr r2, [r7, #12]
	lsls r3, r3, #1
	lsrs r3, r3, #16
	lsls r3, r3, #16
	subs r2, r2, r3
	mov r3, r10
	lsls r1, r3, #17
	ldr r3, [r7, #8]
	ldr r0, .L_0200b5bc
	adds r1, r1, r3
	ldr r3, .L_0200b5c0
	adds r1, r1, r0
	movs r0, #30
	adds r2, r2, r3
	adds r0, #255
	ldr r3, [r7, #16]
	bl Func_02003a64
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200b5a2
	mov r1, r9
	ldr r0, [r6, #80]
	bl Func_02003bd4
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	adds r2, #4
	strb r3, [r2]
	movs r1, #0
	mov r9, r0
	adds r0, r6, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r6, #0
	movs r1, #2
	bl Func_02003a54
	adds r0, r6, #0
	ldr r1, .L_0200b5c4
	bl Func_02003a5c
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r6, #24]
	str r3, [r6, #28]
	ldr r1, [r6, #80]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	mov r2, r8
	strb r3, [r1, #9]
	cmp r2, #0
	beq .L_0200b56c
	mov r3, r10
	lsls r5, r3, #13
	adds r0, r5, #0
	bl Math_Cosine
	ldr r3, .L_0200b5c8
	ldr r1, .L_0200b5cc
	mov lr, r3
	.2byte 0xf800
	str r0, [r6, #68]
	adds r0, r5, #0
	bl Math_Sine
	b .L_0200b570
.L_0200b56c:
	mov r0, r8
	str r0, [r6, #68]
.L_0200b570:
	str r0, [r6, #76]
	bl Random16Far
	movs r2, #192
	lsls r0, r0, #14
	lsls r2, r2, #7
	lsrs r0, r0, #16
	adds r0, r0, r2
	negs r0, r0
	str r0, [r6, #72]
	bl Random16Far
	ldr r3, .L_0200b5d0
	lsls r0, r0, #9
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r6, #0
	adds r3, #100
	strh r0, [r3]
	ldr r3, .L_0200b5d4
	str r3, [r6, #48]
	ldr r3, .L_0200b5d8
	str r3, [r6, #52]
	ldr r3, .L_0200b5dc
	str r3, [r6, #108]
.L_0200b5a2:
	movs r0, #1
	add r10, r0
	mov r2, r10
	cmp r2, #7
	bls .L_0200b4ce
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b5b8:
	.4byte gPartyState
.L_0200b5bc:
	.4byte 0xfff80000
.L_0200b5c0:
	.4byte 0xfffe0000
.L_0200b5c4:
	.4byte Data_02003e0c
.L_0200b5c8:
	.4byte IwramMulQ16
.L_0200b5cc:
	.4byte 0x00013333
.L_0200b5d0:
	.4byte 0xffffff00
.L_0200b5d4:
	.4byte 0xfffff800
.L_0200b5d8:
	.4byte 0xfffffa00
.L_0200b5dc:
	.4byte Func_0200342c
	.section .text.x0200b5e0,"ax",%progbits
	.global Func_020035e0
	.thumb_func
Func_020035e0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0200b6e0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200b6d4
	movs r3, #0
	mov r9, r3
	mov r10, r3
.L_0200b604:
	movs r0, #30
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, #255
	bl Func_02003a64
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200b6ca
	mov r1, r9
	ldr r0, [r7, #80]
	bl Func_02003bd4
	movs r4, #0
	mov r8, r4
	adds r3, r7, #0
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	adds r3, #4
	strb r2, [r3]
	movs r1, #0
	mov r9, r0
	adds r0, r7, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r7, #0
	movs r1, #2
	bl Func_02003a54
	ldr r1, .L_0200b6e4
	adds r0, r7, #0
	bl Func_02003a5c
	mov r3, r10
	lsls r5, r3, #12
	adds r0, r5, #0
	bl Math_Cosine
	mov r4, r8
	str r4, [r7, #72]
	str r0, [r7, #68]
	adds r0, r5, #0
	bl Math_Sine
	ldr r3, [r7, #68]
	str r0, [r7, #76]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r7, #68]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r7, #68]
	adds r3, r3, r0
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_0200b6e8
	adds r2, r2, r3
	str r2, [r7, #68]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r7, #76]
	adds r3, r3, r0
	ldr r4, .L_0200b6ec
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r2, r2, r3
	adds r2, r2, r4
	str r2, [r7, #76]
	bl Random16Far
	ldr r2, .L_0200b6f0
	lsls r0, r0, #12
	lsrs r0, r0, #16
	adds r3, r7, #0
	adds r0, r0, r2
	adds r3, #100
	strh r0, [r3]
	mov r3, r8
	str r3, [r7, #48]
	str r3, [r7, #52]
	ldr r3, .L_0200b6f4
	ldr r0, [r7, #80]
	str r3, [r7, #108]
	ldr r3, [r6, #80]
	movs r1, #12
	ldrb r3, [r3, #9]
	movs r4, #13
	ands r1, r3
	ldrb r3, [r0, #9]
	negs r4, r4
	adds r2, r4, #0
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #9]
.L_0200b6ca:
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200b604
.L_0200b6d4:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b6e0:
	.4byte gPartyState
.L_0200b6e4:
	.4byte Data_02003e3c
.L_0200b6e8:
	.4byte 0xffffa000
.L_0200b6ec:
	.4byte 0xffffd000
.L_0200b6f0:
	.4byte 0xfffff800
.L_0200b6f4:
	.4byte Func_0200342c
	.section .text.x0200b6f8,"ax",%progbits
	.global Func_020036f8
	.thumb_func
Func_020036f8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r10, r0
	adds r0, r2, #0
	adds r5, r1, #0
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	lsls r3, r5, #3
	subs r3, r3, r5
	movs r1, #156
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r3, [r2, r3]
	ldr r2, .L_0200b778
	adds r7, r0, #0
	ldr r1, .L_0200b77c
	adds r3, r3, r2
	adds r5, r7, #0
	asrs r3, r3, #2
	adds r5, #34
	adds r6, r3, r1
	ldr r2, [r7, #16]
	ldr r1, [r7, #8]
	ldrb r0, [r5]
	bl Func_02003adc
	ldr r2, [r7, #16]
	mov r8, r0
	ldr r1, [r7, #8]
	ldrb r0, [r5]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #8]
	asrs r2, r0, #19
	add r2, r10
	cmp r3, #0
	bge .L_0200b752
	ldr r1, .L_0200b780
	adds r3, r3, r1
.L_0200b752:
	ldr r0, [r7, #16]
	asrs r1, r3, #20
	cmp r0, #0
	bge .L_0200b75e
	ldr r3, .L_0200b780
	adds r0, r0, r3
.L_0200b75e:
	asrs r3, r0, #20
	lsls r3, r3, #7
	adds r3, r1, r3
	ldrb r0, [r5]
	mov r1, r8
	adds r6, r6, r3
	bl Func_02003c24
	strb r0, [r6]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200b778:
	.4byte 0xfdff0000
.L_0200b77c:
	.4byte Data_02024000
.L_0200b780:
	.4byte 0x000fffff
	.section .text.x0200b784,"ax",%progbits
	.global Func_02003784
	.thumb_func
Func_02003784:
	push {lr}
	ldr r3, .L_0200b798
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r2, [r3]
	bl Func_020036f8
	pop {pc}
	.2byte 0x0000
.L_0200b798:
	.4byte gPartyState
	.section .text.x0200b79c,"ax",%progbits
	.global Func_0200379c
	.thumb_func
Func_0200379c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	movs r2, #160
	lsls r2, r2, #3
	movs r3, #192
	adds r5, r7, r2
	lsls r3, r3, #4
	movs r2, #63
	adds r6, r7, r3
	mov r8, r2
.L_0200b7ba:
	ldr r3, [r5, #24]
	cmp r3, #19
	bhi .L_0200b808
	movs r2, #176
	lsls r2, r2, #5
	adds r2, #2
	adds r1, r7, r2
	ldrh r1, [r1]
	movs r2, #7
	asrs r3, r3, #2
	ands r3, r2
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r3, .L_0200b7fc
	ldr r2, .L_0200b800
	ands r1, r3
	ldrh r3, [r6, #8]
	adds r0, r6, #0
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #8]
	adds r1, r5, #0
	bl Func_02003c3c
	adds r0, r5, #0
	movs r1, #63
	ldr r2, .L_0200b804
	bl Func_02003c44
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	b .L_0200b808
.L_0200b7fc:
	.4byte 0x000003ff
.L_0200b800:
	.4byte 0xfffffc00
.L_0200b804:
	.4byte 0xffff8000
.L_0200b808:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r2, r8
	adds r6, #40
	adds r5, #28
	cmp r2, #0
	bge .L_0200b7ba
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b820,"ax",%progbits
	.global Func_02003820
	.thumb_func
Func_02003820:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r2, [sp, #0]
	str r0, [sp, #8]
	str r1, [sp, #4]
	adds r2, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r11, r3
	cmp r2, #0
	ble .L_0200b8cc
	adds r7, r2, #0
.L_0200b848:
	bl Random16Far
	movs r1, #176
	lsls r1, r1, #5
	add r1, r11
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r10, r1
	lsls r6, r3, #3
	subs r6, r6, r3
	lsls r6, r6, #2
	movs r3, #160
	add r6, r11
	lsls r3, r3, #3
	adds r5, r6, r3
	movs r1, #0
	str r1, [r5, #24]
	ldr r2, [sp, #8]
	mov r8, r1
	str r2, [r5]
	ldr r3, [sp, #4]
	mov r9, r0
	str r3, [r5, #4]
	ldr r1, [sp, #0]
	subs r7, #1
	str r1, [r5, #8]
	bl Random16Far
	movs r2, #128
	lsls r2, r2, #12
	lsls r0, r0, #3
	adds r0, r0, r2
	mov r1, r9
	adds r2, r5, #0
	bl Vector_AddPolarOffsetFar
	mov r3, r8
	str r3, [r5, #12]
	movs r3, #160
	lsls r3, r3, #11
	mov r1, r8
	str r3, [r5, #16]
	str r1, [r5, #20]
	bl Random16Far
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #12
	movs r2, #128
	adds r6, r6, r3
	lsls r2, r2, #10
	lsls r0, r0, #1
	adds r0, r0, r2
	mov r1, r9
	adds r2, r6, #0
	bl Vector_AddPolarOffsetFar
	mov r1, r10
	ldrh r3, [r1]
	movs r2, #63
	adds r3, #1
	ands r3, r2
	mov r2, r10
	strh r3, [r2]
	cmp r7, #0
	bne .L_0200b848
.L_0200b8cc:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b8dc,"ax",%progbits
	.global Func_020038dc
	.thumb_func
Func_020038dc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #176
	lsls r1, r1, #5
	adds r1, #8
	movs r0, #220
	sub sp, #4
	bl Runtime_AllocateHeapBlockFar
	adds r6, r0, #0
	ldr r0, .L_0200b990
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Func_02003a14
	bl Resource_FindFreeEntry
	movs r1, #160
	lsls r1, r1, #3
	adds r2, r6, #0
	adds r5, r0, #0
	bl VramBlock_LoadCached
	movs r1, #176
	lsls r1, r1, #5
	adds r1, #2
	mov r10, r0
	adds r3, r6, r1
	mov r2, r10
	adds r1, #2
	strh r2, [r3]
	adds r3, r6, r1
	strh r5, [r3]
	movs r2, #160
	movs r3, #192
	lsls r2, r2, #3
	lsls r3, r3, #4
	movs r1, #63
	adds r7, r6, r2
	adds r5, r6, r3
	mov r8, r1
.L_0200b934:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	bl Func_02003c34
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	movs r1, #13
	strb r3, [r5, #5]
	negs r1, r1
	movs r3, #15
	ands r3, r2
	adds r2, r1, #0
	ands r3, r2
	strb r3, [r5, #9]
	movs r3, #240
	strh r3, [r5, #30]
	subs r3, #241
	add r8, r3
	mov r2, r8
	str r3, [r7, #24]
	adds r5, #40
	adds r7, #28
	cmp r2, #0
	bge .L_0200b934
	movs r1, #176
	lsls r1, r1, #5
	adds r2, r6, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200b994
	bl Func_020039d4
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200b990:
	.4byte 0x000001f0
.L_0200b994:
	.4byte Func_0200379c
	.section .text.x0200b998,"ax",%progbits
	.global Func_02003998
	.thumb_func
Func_02003998:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r0, .L_0200b9c0
	ldr r5, [r3]
	bl Scheduler_RemoveCallbackFar
	movs r3, #176
	lsls r3, r3, #5
	adds r3, #4
	adds r5, r5, r3
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl Func_02003a1c
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	pop {r5, pc}
.L_0200b9c0:
	.4byte Func_0200379c
	.section .rodata.x0200bc5c,"a",%progbits
.L_0200bc5c:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
.L_0200bc98:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
.L_0200bcd4:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02003d10
Data_02003d10:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.global Data_02003d50
Data_02003d50:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000012
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02003d74
Data_02003d74:
	.4byte 0x00000027
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02003d98
Data_02003d98:
	.4byte 0x000a0008
	.2byte 0xffff
	.global Data_02003d9e
Data_02003d9e:
	.2byte 0x0009
	.4byte 0x00140032
	.4byte 0x001e000c
	.4byte 0xffffffff
	.global Data_02003dac
Data_02003dac:
	.4byte 0x000f0008
	.2byte 0xffff
	.global Data_02003db2
Data_02003db2:
	.2byte 0x000d
	.4byte 0xffff001e
	.2byte 0xffff
	.global Data_02003dba
Data_02003dba:
	.2byte 0x0009
	.4byte 0x000a0200
	.4byte 0xffff0202
	.global Data_02003dc4
Data_02003dc4:
	.4byte 0x0032000e
	.4byte 0x0011000f
	.4byte 0x00120033
	.2byte 0xffff
	.global Data_02003dd2
Data_02003dd2:
	.2byte 0x0010
	.2byte 0xffff
	.global Data_02003dd6
Data_02003dd6:
	.2byte 0x0009
	.4byte 0x000a0032
	.4byte 0x0033000b
	.4byte 0xffff000c
	.global Data_02003de4
Data_02003de4:
	.4byte 0xffff0008
	.global Data_02003de8
Data_02003de8:
	.4byte 0x00090008
	.2byte 0xffff
	.global Data_02003dee
Data_02003dee:
	.2byte 0x000b
	.4byte 0xffff001e
	.4byte 0x0032000c
	.4byte 0x000f000d
	.4byte 0xffff001f
	.2byte 0xffff
	.global Data_02003e02
Data_02003e02:
	.2byte 0x0009
	.4byte 0xffff0032
	.4byte 0x0000ffff
	.global Data_02003e0c
Data_02003e0c:
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_02003e3c
Data_02003e3c:
	.4byte 0x00000000
	.4byte 0x0000002c
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_02003e6c
Data_02003e6c:
	.4byte .L_0200bc5c
	.4byte .L_0200bc98
	.4byte .L_0200bcd4
	.global Data_02003e78
Data_02003e78:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003ea8
Data_02003ea8:
	.4byte 0x014800f0
	.4byte 0x01000160
	.4byte 0x01700158
	.4byte 0x0003ffff
	.4byte 0x014801c0
	.4byte 0x01d00160
	.4byte 0x01700158
	.4byte 0x0004ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003ed8
Data_02003ed8:
	.4byte 0x00380110
	.4byte 0x01200240
	.4byte 0x02500048
	.4byte 0x0001ffff
	.4byte 0x00280160
	.4byte 0x01700230
	.4byte 0x02400038
	.4byte 0x0002ffff
	.4byte Data_0200024c + 0xa4
	.4byte Data_030001e4 + 0x34
	.4byte 0x02280210
	.4byte 0x0005ffff
	.4byte 0x00500310
	.4byte 0x03200250
	.4byte 0x02600060
	.4byte 0x0007ffff
	.4byte Data_0200024c + 0x134
	.4byte 0x03900210
	.4byte 0x02200210
	.4byte 0x0008ffff
	.4byte 0x00300110
	.4byte 0x01200240
	.4byte 0x02500040
	.4byte 0x0009ffff
	.4byte 0x00200160
	.4byte 0x01700230
	.4byte 0x02400030
	.4byte 0x000affff
	.4byte 0x00380310
	.4byte 0x03200250
	.4byte 0x02600048
	.4byte 0x000effff
	.4byte 0xfea802f0
	.4byte IwramFillWords + 0x40
	.4byte 0x02b0feb8
	.4byte 0x0010ffff
	.4byte 0xfea803d0
	.4byte 0x03e002a0
	.4byte 0x02b0feb8
	.4byte 0x0011ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003f88
Data_02003f88:
	.4byte 0xffc800f0
	.4byte 0x010001c0
	.4byte 0x01d0ffd8
	.4byte 0x0001ffff
	.4byte 0xffc80180
	.4byte 0x019001c0
	.4byte 0x01d0ffd8
	.4byte 0x0002ffff
	.4byte 0xff8001a0
	.4byte 0x01b001c0
	.4byte 0x01d0ff90
	.4byte 0x0004ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003fc8
Data_02003fc8:
	.4byte 0x000000df
	.4byte 0x00133002
	.4byte 0x0020b0e8
	.4byte 0x003100e0
	.4byte 0x004110e0
	.4byte 0x000000e0
	.4byte 0x001090e0
	.4byte 0x0020a0e0
	.4byte 0x003040e0
	.4byte 0x004030e0
	.4byte 0x005010e1
	.4byte 0x006020e1
	.4byte 0x0070e0e0
	.4byte 0x008020e1
	.4byte 0x009010e0
	.4byte 0x00a020e0
	.4byte 0x00b0d0e0
	.4byte 0x00c0f0e0
	.4byte 0x00d0b0e0
	.4byte 0x00e070e0
	.4byte 0x00f0c0e0
	.4byte 0x010030df
	.4byte 0x011040df
	.4byte 0x000000e1
	.4byte 0x001050e0
	.4byte 0x002080e0
	.4byte 0x003080e3
	.4byte 0x004060e0
	.4byte 0x000001ff
	.global Data_0200403c
Data_0200403c:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02028000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapBlocks
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02028000
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff017b
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0193
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020041a4
Data_020041a4:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapBlocks
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200427c
Data_0200427c:
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapBlocks
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02028000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapBlocks
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02028000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapBlocks
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02028000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff017b
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff017b
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0193
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00028000
	.4byte 0xffff0193
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00028000
	.4byte 0xffff0193
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020043e4
Data_020043e4:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x0000000e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x0000000f
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff017b
	.4byte 0x00000001
	.4byte 0x0000000f
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff017b
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000012
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020044a4
Data_020044a4:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000013
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000014
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02028000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000017
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff017b
	.4byte 0x00000001
	.4byte 0x00000017
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00000018
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02028000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x0000001c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020045c4
Data_020045c4:
	.4byte 0xffff0180
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200460c
Data_0200460c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000ca02
	.4byte 0x02f80002
	.4byte Func_02001b44
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_02001b24
	.4byte 0x00008f15
	.4byte 0xffff0013
	.4byte 0x00000000
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000940
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000a48
	.4byte 0x10008c15
	.4byte Field_Map182 + 0xb88
	.4byte Func_02000940
	.4byte 0x00008c15
	.4byte Field_Map182 + 0xb88
	.4byte Func_02000a48
	.4byte 0x10008c15
	.4byte Tileset_Set82TilesB + 0xa7a
	.4byte Func_02000940
	.4byte 0x00008c15
	.4byte Tileset_Set82TilesB + 0xa7a
	.4byte Func_02000a48
	.4byte 0x50009705
	.4byte Data_02000000 + 0x1f
	.4byte Func_020019e8
	.4byte 0x50009705
	.4byte Data_02010020
	.4byte Func_02001a38
	.4byte 0x50009705
	.4byte Data_02020004 + 0x1d
	.4byte Func_02001a8c
	.4byte 0x50009705
	.4byte Data_02030000 + 0x22
	.4byte Func_02001ac0
	.4byte 0x50009705
	.4byte 0x02040023
	.4byte Func_02001af4
	.4byte 0x80009705
	.4byte 0xffff001e
	.4byte Func_02001ba4
	.4byte 0x50009705
	.4byte 0x02f0001e
	.4byte Func_02001110
	.4byte 0x50009705
	.4byte 0xffff001e
	.4byte Func_020014bc
	.4byte 0x00009705
	.4byte 0xffff001e
	.4byte Func_02001444
	.4byte 0x80009705
	.4byte 0xffff0032
	.4byte Func_02001ba4
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte Func_02001110
	.4byte 0x00009705
	.4byte 0xffff0032
	.4byte Func_0200148c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004720
Data_02004720:
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte Func_02001bb0
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000940
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000a48
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte Func_02000940
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte Func_02000a48
	.4byte 0x10008c15
	.4byte Tileset_Set83TilesD + 0x157
	.4byte Func_02000940
	.4byte 0x00008c15
	.4byte Tileset_Set83TilesD + 0x157
	.4byte Func_02000a48
	.4byte 0x50009705
	.4byte Data_02000000 + 0x1f
	.4byte Func_020019e8
	.4byte 0x00000002
	.4byte 0x02120032
	.4byte Func_02001bc0
	.4byte 0x00000002
	.4byte 0xffff0033
	.4byte Func_02001be0
	.4byte 0x80009705
	.4byte 0xffff001e
	.4byte Func_02001ba4
	.4byte 0x50009705
	.4byte 0x02f0001e
	.4byte Func_02001110
	.4byte 0x50009705
	.4byte 0xffff001e
	.4byte Func_020014bc
	.4byte 0x00009705
	.4byte 0xffff001e
	.4byte Func_02001444
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020047e0
Data_020047e0:
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000940
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000a48
	.4byte 0x10008c15
	.4byte Tileset_Palette39 + 0x44
	.4byte Func_02000940
	.4byte 0x00008c15
	.4byte Tileset_Palette39 + 0x44
	.4byte Func_02000a48
	.4byte 0x80009705
	.4byte 0xffff0032
	.4byte Func_02001ba4
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte Func_02001110
	.4byte 0x00009705
	.4byte 0xffff0032
	.4byte Func_0200148c
	.4byte 0x80009705
	.4byte 0xffff0033
	.4byte Func_02001ba4
	.4byte 0x50009705
	.4byte 0xffff0033
	.4byte Func_02001110
	.4byte 0x00009705
	.4byte 0xffff0033
	.4byte Func_0200148c
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte Func_02001c00
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200487c
Data_0200487c:
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000602
	.4byte 0x00080028
	.4byte Func_02001e08
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000940
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000a48
	.4byte 0x10008c15
	.4byte Tileset_Set88TilesA + 0x3ac
	.4byte Func_02000940
	.4byte 0x00008c15
	.4byte Tileset_Set88TilesA + 0x3ac
	.4byte Func_02000a48
	.4byte 0x00008515
	.4byte 0x0250000e
	.4byte 0x00000000
	.4byte 0x80009705
	.4byte 0xffff0032
	.4byte Func_02001ba4
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte Func_02001110
	.4byte 0x00009705
	.4byte 0xffff0032
	.4byte Func_0200148c
	.4byte 0x80009705
	.4byte 0xffff0033
	.4byte Func_02001ba4
	.4byte 0x50009705
	.4byte 0xffff0033
	.4byte Func_02001110
	.4byte 0x00009705
	.4byte 0xffff0033
	.4byte Func_0200148c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004930
Data_02004930:
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00008602
	.4byte 0xffff0028
	.4byte Func_02001edc
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000940
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000a48
	.4byte 0x10008c15
	.4byte Tileset_Set89TilesC + 0x16a8
	.4byte Func_02000940
	.4byte 0x00008c15
	.4byte Tileset_Set89TilesC + 0x16a8
	.4byte Func_02000a48
	.4byte 0x10008c15
	.4byte Tileset_Set91TilesC + 0x1175
	.4byte Func_02000940
	.4byte 0x00008c15
	.4byte Tileset_Set91TilesC + 0x1175
	.4byte Func_02000a48
	.4byte 0x80009705
	.4byte 0xffff001e
	.4byte Func_02001ba4
	.4byte 0x50009705
	.4byte 0x02f0001e
	.4byte Func_02001110
	.4byte 0x50009705
	.4byte 0xffff001e
	.4byte Func_020014bc
	.4byte 0x00009705
	.4byte 0xffff001e
	.4byte Func_02001444
	.4byte 0x80009705
	.4byte 0xffff001f
	.4byte Func_02001ba4
	.4byte 0x50009705
	.4byte 0x02f0001f
	.4byte Func_02001110
	.4byte 0x50009705
	.4byte 0xffff001f
	.4byte Func_020014bc
	.4byte 0x00009705
	.4byte 0xffff001f
	.4byte Func_02001444
	.4byte 0x50009705
	.4byte Data_02010020
	.4byte Func_02001a38
	.4byte 0x80009705
	.4byte 0xffff0032
	.4byte Func_02001ba4
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte Func_02001110
	.4byte 0x00009705
	.4byte 0xffff0032
	.4byte Func_0200148c
	.4byte 0x00000c15
	.4byte 0x02500010
	.4byte Func_02001fb0
	.4byte 0x00000c15
	.4byte 0x02510011
	.4byte Func_02001fd4
	.4byte 0x00000c15
	.4byte 0x02520012
	.4byte Func_02001ffc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004a5c
Data_02004a5c:
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x80009705
	.4byte 0xffff0032
	.4byte Func_02001ba4
	.4byte 0x50009705
	.4byte 0x02f00032
	.4byte Func_02001110
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte Func_020020f8
	.4byte 0x00009705
	.4byte 0xffff0032
	.4byte Func_02001444
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
