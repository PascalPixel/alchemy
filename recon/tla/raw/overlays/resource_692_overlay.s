.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
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
	bl Func_02004ca8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020080ca
	movs r1, #0
	bl Object_SetSpritePriority
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
	bl Object_SetPartAttribute
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
	bl Func_02004ca8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200811e
	movs r1, #1
	bl Object_SetSpritePriority
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
	bl Object_SetPartAttribute
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
	bl Func_02004ca8
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
	bl Func_02004c98
	ldr r2, .L_02008330
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_02004ca0
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
	bl Object_SetSpritePriority
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
	bl Object_SetPartAttribute
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
	bl Object_SetSpritePriority
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
	bl Func_02004c98
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02004ca0
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
	.4byte Data_02005698
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
	bl Func_02004cf8
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
	bl Func_02004c98
	lsls r5, r5, #6
	movs r0, #15
	bl WaitFrames
	adds r5, #51
	movs r0, #185
	bl Func_02004f30
	str r5, [r6, #48]
	str r5, [r6, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl Func_02004cc8
	mov r0, r8
	str r5, [r0, #48]
	str r5, [r0, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl Func_02004cc8
	adds r0, r6, #0
	bl Func_02004cd0
	bl Func_02004eb8
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
	bl Func_02004c98
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
	.4byte Data_02004fec
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
	movs r0, #15
	movs r1, #56
	bl Func_02004e30
	pop {pc}
	.section .text.x020085a0,"ax",%progbits
	.global Func_020005a0
	.thumb_func
Func_020005a0:
	push {lr}
	movs r0, #17
	movs r1, #2
	movs r2, #19
	bl Func_02004ea8
	pop {pc}
	.2byte 0x0000
	.section .text.x020085b0,"ax",%progbits
	.global Func_020005b0
	.thumb_func
Func_020005b0:
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
	ldr r3, .L_020085d8
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_020085d8:
	.4byte IwramFillWords + 0x74
	.section .text.x020085dc,"ax",%progbits
	.global Func_020005dc
	.thumb_func
Func_020005dc:
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
	bl Func_02004d20
	mov r2, r8
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r5, [r5, r3]
	ldr r3, .L_02008634
	ldr r4, [sp, #0]
	ldr r2, .L_02008638
	adds r5, r5, r3
	lsls r6, r6, #7
	asrs r5, r5, #2
	adds r1, r0, #0
	adds r4, r4, r6
	adds r5, r5, r2
	mov r0, r8
	mov r2, r10
	adds r5, r5, r4
	bl Func_02004ef0
	strb r0, [r5]
	add sp, #4
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_02008634:
	.4byte 0xfdff0000
.L_02008638:
	.4byte Data_02024000
	.section .text.x0200863c,"ax",%progbits
	.global Func_0200063c
	.thumb_func
Func_0200063c:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r5, #60
.L_02008642:
	cmp r5, #0
	beq .L_02008654
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #40]
	subs r5, #1
	cmp r3, #0
	bne .L_02008642
.L_02008654:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008658,"ax",%progbits
	.global Func_02000658
	.thumb_func
Func_02000658:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_02008668
	movs r0, #0
	b .L_0200868e
.L_02008668:
	cmp r0, #2
	bhi .L_0200867c
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_0200867e
.L_0200867c:
	ldr r4, .L_02008690
.L_0200867e:
	lsls r3, r2, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	ldrb r0, [r4, #2]
	ldrb r3, [r4, #3]
	lsls r0, r0, #8
	orrs r0, r3
.L_0200868e:
	pop {pc}
.L_02008690:
	.4byte gMapCellBuffer
	.section .text.x02008694,"ax",%progbits
	.global Func_02000694
	.thumb_func
Func_02000694:
	push {r5, lr}
	adds r5, r1, #0
	adds r1, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_020086a8
	movs r0, #0
	b .L_020086d4
.L_020086a8:
	cmp r0, #2
	bhi .L_020086bc
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_020086be
.L_020086bc:
	ldr r4, .L_020086d8
.L_020086be:
	lsls r3, r2, #7
	adds r3, r5, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	ldrb r0, [r4, #2]
	ldrb r3, [r4, #3]
	lsls r0, r0, #8
	orrs r0, r3
	asrs r3, r1, #8
	strb r3, [r4, #2]
	strb r1, [r4, #3]
.L_020086d4:
	pop {r5, pc}
	.2byte 0x0000
.L_020086d8:
	.4byte gMapCellBuffer
	.section .text.x020086dc,"ax",%progbits
	.global Func_020006dc
	.thumb_func
Func_020006dc:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	b .L_0200874c
.L_020086e2:
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
	bl Func_02000658
	movs r3, #64
	ands r0, r3
	cmp r0, #0
	beq .L_0200874a
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	cmp r0, #0
	beq .L_02008758
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
.L_0200874a:
	adds r6, #2
.L_0200874c:
	movs r2, #255
	ldrh r3, [r6]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_020086e2
.L_02008758:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200875c,"ax",%progbits
	.global Func_0200075c
	.thumb_func
Func_0200075c:
	push {r5, lr}
	adds r5, r0, #0
	movs r2, #99
	adds r2, r2, r5
	ldrb r3, [r2]
	mov r12, r2
	cmp r3, #0
	beq .L_020087aa
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	cmp r0, #0
	beq .L_020087aa
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
.L_020087aa:
	pop {r5, pc}
	.section .text.x020087ac,"ax",%progbits
	.global Func_020007ac
	.thumb_func
Func_020007ac:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r4, #0
	ldrsh r3, [r3, r4]
	adds r0, #91
	cmp r3, #0
	bne .L_020087f4
	subs r1, #12
	adds r3, r2, r1
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	bne .L_020087f4
	adds r1, #4
	adds r3, r2, r1
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	bne .L_020087f4
	adds r1, #10
	adds r3, r2, r1
	movs r4, #0
	ldrsh r1, [r3, r4]
	cmp r1, #0
	bne .L_020087f4
	movs r4, #217
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrh r3, [r3]
	cmp r3, #0
	bne .L_020087fa
.L_020087f4:
	movs r3, #1
	strb r3, [r0]
	b .L_020087fc
.L_020087fa:
	strb r1, [r0]
.L_020087fc:
	pop {pc}
	.2byte 0x0000
	.section .text.x02008800,"ax",%progbits
	.global Func_02000800
	.thumb_func
Func_02000800:
	push {r5, r6, r7, lr}
	lsls r5, r2, #16
	adds r7, r1, #0
	bl Object_GetById
	movs r1, #4
	adds r6, r0, #0
	bl Func_02004c98
	adds r2, r6, #0
	adds r2, #90
	movs r3, #0
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r6, #48]
	str r3, [r6, #52]
	subs r2, #5
	movs r3, #2
	strb r3, [r2]
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	asrs r5, r5, #16
	movs r3, #128
	orrs r3, r2
	lsls r5, r5, #16
	strb r3, [r1]
	adds r0, r6, #0
	movs r1, #0
	lsrs r5, r5, #16
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200886a
	movs r3, #128
	lsls r3, r3, #13
	movs r0, #10
	str r3, [r6, #20]
	str r3, [r6, #12]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008870
	adds r0, r6, #0
	adds r1, r7, #0
	bl Func_02004ca0
	b .L_02008870
.L_0200886a:
	ldr r3, .L_02008878
	str r3, [r6, #20]
	str r3, [r6, #12]
.L_02008870:
	ldr r3, .L_0200887c
	str r3, [r6, #108]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008878:
	.4byte 0xfff20000
.L_0200887c:
	.4byte Func_020007ac
	.section .text.x02008880,"ax",%progbits
	.global Func_02000880
	.thumb_func
Func_02000880:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	b .L_020088d0
.L_0200888a:
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
	bl Func_020005dc
.L_020088d0:
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200888a
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020088f8,"ax",%progbits
	.global Func_020008f8
	.thumb_func
Func_020008f8:
	push {lr}
	ldr r3, .L_02008958
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200895c
	cmp r2, r3
	beq .L_02008952
	ldr r3, .L_02008960
	cmp r2, r3
	bne .L_02008916
	ldr r0, .L_02008964
	b .L_02008954
.L_02008916:
	ldr r3, .L_02008968
	cmp r2, r3
	bne .L_02008920
	ldr r0, .L_0200896c
	b .L_02008954
.L_02008920:
	ldr r3, .L_02008970
	cmp r2, r3
	bne .L_0200892a
	ldr r0, .L_02008974
	b .L_02008954
.L_0200892a:
	ldr r3, .L_02008978
	cmp r2, r3
	bne .L_02008934
	ldr r0, .L_0200897c
	b .L_02008954
.L_02008934:
	ldr r3, .L_02008980
	cmp r2, r3
	bne .L_0200893e
	ldr r0, .L_02008984
	b .L_02008954
.L_0200893e:
	ldr r3, .L_02008988
	cmp r2, r3
	bne .L_02008948
	ldr r0, .L_0200898c
	b .L_02008954
.L_02008948:
	ldr r3, .L_02008990
	cmp r2, r3
	bne .L_02008952
	ldr r0, .L_02008994
	b .L_02008954
.L_02008952:
	ldr r0, .L_02008998
.L_02008954:
	pop {pc}
	.2byte 0x0000
.L_02008958:
	.4byte gPartyState
.L_0200895c:
	.4byte 0x000000e2
.L_02008960:
	.4byte 0x000000e3
.L_02008964:
	.4byte Data_02005870
.L_02008968:
	.4byte 0x000000e4
.L_0200896c:
	.4byte Data_02005978
.L_02008970:
	.4byte 0x000000e5
.L_02008974:
	.4byte Data_02005a50
.L_02008978:
	.4byte 0x000000e6
.L_0200897c:
	.4byte Data_02005c30
.L_02008980:
	.4byte 0x000000e7
.L_02008984:
	.4byte Data_02005ca8
.L_02008988:
	.4byte 0x000000e8
.L_0200898c:
	.4byte Data_02005d38
.L_02008990:
	.4byte 0x000000e9
.L_02008994:
	.4byte Data_02005ed0
.L_02008998:
	.4byte Data_02005840
	.section .text.x0200899c,"ax",%progbits
	.global Func_0200099c
	.thumb_func
Func_0200099c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	ldr r3, .L_02008a04
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008a08
	cmp r2, r3
	bne .L_020089be
	ldr r0, .L_02008a0c
	bl Func_02004ee0
	b .L_020089f4
.L_020089be:
	ldr r3, .L_02008a10
	cmp r2, r3
	bne .L_020089cc
	ldr r0, .L_02008a14
	bl Func_02004ee0
	b .L_020089f4
.L_020089cc:
	ldr r3, .L_02008a18
	cmp r2, r3
	bne .L_020089da
	ldr r0, .L_02008a1c
	bl Func_02004ee0
	b .L_020089f4
.L_020089da:
	ldr r3, .L_02008a20
	cmp r2, r3
	bne .L_020089e8
	ldr r0, .L_02008a24
	bl Func_02004ee0
	b .L_020089f4
.L_020089e8:
	ldr r3, .L_02008a28
	cmp r2, r3
	bne .L_020089f4
	ldr r0, .L_02008a2c
	bl Func_02004ee0
.L_020089f4:
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #188
	adds r3, r5, r2
	ldr r0, [r3]
	bl Func_0200075c
	pop {r5, pc}
.L_02008a04:
	.4byte gPartyState
.L_02008a08:
	.4byte 0x000000e3
.L_02008a0c:
	.4byte Data_020054fc
.L_02008a10:
	.4byte 0x000000e4
.L_02008a14:
	.4byte Data_02005508
.L_02008a18:
	.4byte 0x000000e5
.L_02008a1c:
	.4byte Data_02005520
.L_02008a20:
	.4byte 0x000000e7
.L_02008a24:
	.4byte Data_02005528
.L_02008a28:
	.4byte 0x000000e8
.L_02008a2c:
	.4byte Data_0200552e
	.section .text.x02008a30,"ax",%progbits
	.global Func_02000a30
	.thumb_func
Func_02000a30:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #188
	adds r3, r3, r1
	ldr r7, [r3]
	sub sp, #8
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	bl Func_02004ee8
	ldr r3, .L_02008b0c
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008b10
	cmp r2, r3
	bne .L_02008b3a
	ldr r0, .L_02008b14
	bl Func_020006dc
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #2
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	cmp r0, r3
	bne .L_02008a84
	b .L_02008bf4
.L_02008a84:
	movs r2, #34
	adds r2, r2, r7
	movs r3, #2
	strb r3, [r2]
	movs r3, #85
	adds r3, r3, r7
	mov r8, r3
	mov r1, r8
	movs r3, #3
	movs r0, #1
	strb r3, [r1]
	mov r10, r2
	bl WaitFrames
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008b22
	movs r1, #128
	lsls r1, r1, #5
	str r1, [r7, #72]
	adds r0, r7, #0
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #2
	orrs r3, r2
	strb r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r1, [r3]
	movs r6, #128
	ldr r3, .L_02008b08
	lsls r6, r6, #19
	adds r6, #80
	ldr r2, [r7, #16]
	strh r3, [r6]
	movs r3, #128
	lsls r3, r3, #13
	adds r2, r2, r3
	movs r3, #156
	lsls r3, r3, #1
	ldr r1, [r7, #20]
	ldr r0, [r7, #8]
	bl Func_02000080
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_0200063c
	movs r0, #213
	bl Func_02004f30
	adds r0, r5, #0
	bl Func_02004cb0
	adds r0, r7, #0
	movs r1, #4
	bl Func_02004c98
	movs r0, #1
	b .L_02008b18
	.2byte 0x0000
.L_02008b08:
	.4byte 0x00003f10
.L_02008b0c:
	.4byte gPartyState
.L_02008b10:
	.4byte 0x000000e7
.L_02008b14:
	.4byte Data_02005528
.L_02008b18:
	bl WaitFrames
	movs r3, #0
	strh r3, [r6]
	b .L_02008b2e
.L_02008b22:
	adds r0, r7, #0
	bl Func_0200063c
	movs r0, #188
	bl Func_02004f30
.L_02008b2e:
	movs r3, #0
	mov r1, r8
	mov r2, r10
	strb r3, [r1]
	strb r3, [r2]
	b .L_02008bf4
.L_02008b3a:
	ldr r3, .L_02008bd4
	cmp r2, r3
	bne .L_02008bf4
	ldr r0, .L_02008bd8
	bl Func_020006dc
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #2
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	cmp r0, r3
	beq .L_02008bf4
	movs r3, #34
	adds r3, r3, r7
	mov r9, r3
	mov r2, r9
	movs r3, #2
	strb r3, [r2]
	movs r3, #85
	adds r3, r3, r7
	mov r10, r3
	movs r1, #0
	movs r3, #3
	mov r8, r1
	mov r1, r10
	strb r3, [r1]
	movs r0, #1
	bl WaitFrames
	movs r2, #128
	lsls r2, r2, #5
	str r2, [r7, #72]
	adds r1, r7, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r5, #2
	orrs r5, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strb r5, [r1]
	movs r6, #128
	strh r2, [r3]
	ldr r3, .L_02008bd0
	lsls r6, r6, #19
	adds r6, #80
	ldr r2, [r7, #16]
	strh r3, [r6]
	movs r3, #128
	lsls r3, r3, #13
	adds r2, r2, r3
	movs r3, #156
	lsls r3, r3, #1
	ldr r1, [r7, #20]
	ldr r0, [r7, #8]
	bl Func_02000080
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_0200063c
	movs r0, #213
	bl Func_02004f30
	adds r0, r5, #0
	bl Func_02004cb0
	movs r1, #4
	adds r0, r7, #0
	bl Func_02004c98
	movs r0, #1
	b .L_02008bdc
.L_02008bd0:
	.4byte 0x00003f10
.L_02008bd4:
	.4byte 0x000000e8
.L_02008bd8:
	.4byte Data_0200552e
.L_02008bdc:
	bl WaitFrames
	ldr r5, .L_02008c14
	mov r1, r8
	strh r1, [r6]
	adds r0, r7, #0
	bl Func_0200063c
	mov r2, r10
	mov r3, r9
	strb r5, [r2]
	strb r5, [r3]
.L_02008bf4:
	ldr r3, [r7, #8]
	movs r1, #240
	asrs r6, r3, #20
	ldr r3, [r7, #16]
	lsls r1, r1, #1
	asrs r5, r3, #20
	ldr r3, .L_02008c18
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008c1c
	cmp r2, r3
	bne .L_02008c2a
	cmp r5, #16
	bne .L_02008cc2
	b .L_02008c20
.L_02008c14:
	.4byte 0x00000000
.L_02008c18:
	.4byte gPartyState
.L_02008c1c:
	.4byte 0x000000e3
.L_02008c20:
	movs r0, #159
	lsls r0, r0, #4
	bl GameFlag_SetBit
	b .L_02008cc2
.L_02008c2a:
	ldr r3, .L_02008cd4
	cmp r2, r3
	bne .L_02008c40
	cmp r5, #41
	bne .L_02008cc2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #241
	bl GameFlag_SetBit
	b .L_02008cc2
.L_02008c40:
	ldr r3, .L_02008cd8
	cmp r2, r3
	bne .L_02008c76
	cmp r6, #21
	bne .L_02008cc2
	cmp r5, #43
	bne .L_02008c58
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #242
	bl GameFlag_SetBit
.L_02008c58:
	cmp r5, #45
	bne .L_02008c66
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #243
	bl GameFlag_SetBit
.L_02008c66:
	cmp r5, #47
	bne .L_02008cc2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #244
	bl GameFlag_SetBit
	b .L_02008cc2
.L_02008c76:
	ldr r3, .L_02008cdc
	cmp r2, r3
	bne .L_02008cae
	cmp r6, #47
	bne .L_02008c9e
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #245
	bl GameFlag_SetBit
	movs r3, #46
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #74
	movs r2, #1
	movs r3, #1
	bl Func_02004ce8
.L_02008c9e:
	cmp r6, #57
	bne .L_02008cc2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #246
	bl GameFlag_SetBit
	b .L_02008cc2
.L_02008cae:
	ldr r3, .L_02008ce0
	cmp r2, r3
	bne .L_02008cc2
	cmp r6, #43
	bne .L_02008cc2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #247
	bl GameFlag_SetBit
.L_02008cc2:
	bl Func_02004d48
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008cd4:
	.4byte 0x000000e4
.L_02008cd8:
	.4byte 0x000000e5
.L_02008cdc:
	.4byte 0x000000e7
.L_02008ce0:
	.4byte 0x000000e8
	.section .text.x02008ce4,"ax",%progbits
	.global Func_02000ce4
	.thumb_func
Func_02000ce4:
	push {r5, lr}
	adds r5, r0, #0
	movs r1, #1
	movs r0, #151
	bl Func_02004e70
	lsls r5, r5, #16
	ldr r3, .L_02008d18
	asrs r5, r5, #16
	movs r2, #133
	lsls r2, r2, #2
	lsls r5, r5, #16
	adds r3, r3, r2
	lsrs r5, r5, #16
	ldr r0, [r3]
	adds r1, r5, #0
	bl Func_02004e78
	movs r0, #1
	bl Field_DispatchTypeHandler
	bl Func_02004e80
	bl Func_02004e88
	pop {r5, pc}
.L_02008d18:
	.4byte gPartyState
	.section .text.x02008d1c,"ax",%progbits
	.global Func_02000d1c
	.thumb_func
Func_02000d1c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02008d9c
	sub sp, #56
	ldr r2, [r3]
	movs r3, #3
	mov r8, r3
	mov r4, r8
	ands r4, r2
	adds r6, r0, #0
	mov r8, r4
	cmp r4, #0
	bne .L_02008d92
	movs r3, #148
	add r7, sp, #16
	adds r3, #255
	strh r3, [r7, #24]
	movs r3, #2
	str r3, [r7]
	ldr r3, .L_02008da0
	str r3, [r7, #28]
	movs r3, #15
	mov r10, r3
	ands r3, r2
	cmp r3, #0
	bne .L_02008d5a
	movs r1, #136
	bl Func_02004f28
.L_02008d5a:
	bl Random16Far
	mov r4, r10
	adds r5, r0, #0
	ands r5, r4
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
	mov r4, r8
	str r3, [sp, #8]
	ldr r0, [r6, #8]
	ldr r1, [r6, #12]
	adds r2, r2, r5
	movs r3, #0
	str r4, [sp, #0]
	str r7, [sp, #12]
	bl Func_0200015c
.L_02008d92:
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008d9c:
	.4byte Data_0300122c
.L_02008da0:
	.4byte Data_0200502c
	.section .text.x02008da4,"ax",%progbits
	.global Func_02000da4
	.thumb_func
Func_02000da4:
	push {lr}
	ldr r3, .L_02008dc8
	adds r2, r0, #0
	adds r2, #99
	ldrb r2, [r2]
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02008dbe
	movs r1, #10
	bl Object_SetPartAttribute
	b .L_02008dc4
.L_02008dbe:
	movs r1, #0
	bl Object_SetPartAttribute
.L_02008dc4:
	pop {pc}
	.2byte 0x0000
.L_02008dc8:
	.4byte Data_0300122c
	.section .text.x02008dcc,"ax",%progbits
	.global Func_02000dcc
	.thumb_func
Func_02000dcc:
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
	bl Func_02000658
	movs r7, #0
	asrs r2, r0, #8
	b .L_02008df0
.L_02008dec:
	ldrh r7, [r5]
	adds r5, #2
.L_02008df0:
	movs r1, #255
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	beq .L_02008e10
	adds r5, #2
	adds r0, r3, #0
	ldrh r3, [r5]
	adds r5, #2
	cmp r2, r3
	bne .L_02008dec
	bl Object_GetById
	ldrh r7, [r5]
	str r0, [r6, #20]
.L_02008e10:
	adds r0, r7, #0
	pop {r5, r6, r7, pc}
	.section .text.x02008e14,"ax",%progbits
	.global Func_02000e14
	.thumb_func
Func_02000e14:
	push {lr}
	movs r0, #0
	bl Func_02004e90
	pop {pc}
	.2byte 0x0000
	.section .text.x02008e20,"ax",%progbits
	.global Func_02000e20
	.thumb_func
Func_02000e20:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r6, r0, #0
	movs r0, #0
	ldr r7, [r3]
	ldr r5, .L_02008e88
	bl Func_02004e90
	ldr r3, .L_02008e8c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008e90
	cmp r2, r3
	beq .L_02008e54
	ldr r3, .L_02008e94
	cmp r2, r3
	beq .L_02008e54
	ldr r3, .L_02008e98
	cmp r2, r3
	bne .L_02008e54
	ldr r5, .L_02008e9c
.L_02008e54:
	cmp r6, #2
	bne .L_02008e6c
	adds r0, r5, #0
	bl Func_02000dcc
	ldr r3, [r7, #20]
	movs r2, #4
	adds r1, r3, #0
	adds r1, #99
	strb r2, [r1]
	ldr r2, .L_02008ea0
	str r2, [r3, #108]
.L_02008e6c:
	cmp r6, #3
	bne .L_02008e84
	adds r0, r5, #0
	bl Func_02000dcc
	ldr r3, [r7, #20]
	movs r2, #2
	adds r1, r3, #0
	adds r1, #99
	strb r2, [r1]
	ldr r2, .L_02008ea0
	str r2, [r3, #108]
.L_02008e84:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008e88:
	.4byte Data_02005500
.L_02008e8c:
	.4byte gPartyState
.L_02008e90:
	.4byte 0x000000e3
.L_02008e94:
	.4byte 0x000000e4
.L_02008e98:
	.4byte 0x000000e5
.L_02008e9c:
	.4byte Data_0200550c
.L_02008ea0:
	.4byte Func_02000da4
	.section .text.x02008ea4,"ax",%progbits
	.global Func_02000ea4
	.thumb_func
Func_02000ea4:
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
	.section .text.x02008ed0,"ax",%progbits
	.global Func_02000ed0
	.thumb_func
Func_02000ed0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	ldr r1, [r0, #8]
	mov r8, r0
	movs r0, #148
	adds r0, #255
	sub sp, #68
	bl Func_02004ca8
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
	bl Func_02004c98
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	ldr r3, [r6, #8]
	add r2, sp, #56
	str r3, [r2]
	ldr r3, [r6, #12]
	str r3, [r2, #4]
	ldr r3, [r6, #16]
	str r3, [r2, #8]
.L_02008f2e:
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
	bl Func_02000658
	asrs r0, r0, #8
	mov r11, r0
	cmp r0, #212
	beq .L_02008fc8
	cmp r0, #222
	beq .L_02008fc8
	movs r2, #8
	mov r9, r2
.L_02008f56:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	mov r0, r9
	lsls r3, r0, #2
	adds r3, #20
	ldr r5, [r2, r3]
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02008fbc
	mov r1, r8
	ldr r2, [r1, #8]
	ldr r3, [r5, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02008f90
	ldr r2, [r1, #12]
	ldr r3, [r5, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02008f90
	ldr r2, [r1, #16]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_02008fbc
.L_02008f90:
	adds r0, r5, #0
	adds r0, #8
	adds r1, r7, #0
	bl Func_020005b0
	cmp r0, #8
	bgt .L_02008fbc
	ldr r2, [r5, #12]
	ldr r3, [r7, #4]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02008fbc
	ldr r3, [r5, #80]
	movs r0, #136
	ldr r3, [r3, #40]
	lsls r0, r0, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, r0
	bne .L_020090b0
	b .L_02008fc8
.L_02008fbc:
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #64
	bne .L_02008f56
	b .L_02008f2e
.L_02008fc8:
	ldr r3, .L_02009134
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
	bl Func_02004cc8
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #7
	bl Func_02004e10
	ldr r1, [r7, #4]
	ldr r2, [r7, #8]
	movs r3, #1
	ldr r0, [r7]
	bl Motion_CamBounds
	adds r0, r6, #0
	bl Func_02004cd0
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	bl Func_02004cb0
	movs r3, #0
	mov r9, r3
.L_0200900c:
	ldr r3, .L_02009138
	movs r2, #1
	ldr r3, [r3]
	mov r10, r3
	mov r0, r10
	ands r0, r2
	mov r10, r0
	cmp r0, #0
	bne .L_0200906a
	add r1, sp, #16
	mov r8, r1
	movs r3, #148
	mov r0, r8
	adds r3, #255
	strh r3, [r0, #24]
	ldr r3, .L_0200913c
	str r2, [r0]
	str r3, [r0, #36]
	ldr r3, .L_02009140
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
.L_0200906a:
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #16
	bne .L_0200900c
	movs r0, #24
	bl WaitFrames
	mov r3, r11
	cmp r3, #222
	bne .L_02009090
	bl Func_02001350
	bl Func_02004d48
	b .L_02009126
.L_02009090:
	ldr r3, .L_02009144
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02004e20
	movs r0, #10
	bl WaitFrames
	bl Func_02004d48
	b .L_02009126
.L_020090b0:
	ldr r3, .L_02009134
	movs r5, #128
	lsls r5, r5, #10
	str r5, [r6, #52]
	str r5, [r6, #48]
	ldr r2, [r7, #4]
	str r3, [r6, #108]
	ldr r1, [r7]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl Func_02004cc8
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #7
	bl Func_02004e10
	ldr r2, [r7, #8]
	ldr r1, [r7, #4]
	movs r3, #1
	ldr r0, [r7]
	bl Motion_CamBounds
	adds r0, r6, #0
	bl Func_02004cd0
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	bl Func_02004cb0
	movs r0, #188
	lsls r0, r0, #2
	bl GameFlag_SetBit
	mov r1, r9
	lsls r0, r1, #16
	lsrs r0, r0, #16
	bl Func_02000ce4
	movs r0, #188
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	ldr r3, .L_02009144
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02004e20
	movs r0, #10
	bl WaitFrames
	bl Func_02004d48
.L_02009126:
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009134:
	.4byte Func_02000d1c
.L_02009138:
	.4byte Data_0300122c
.L_0200913c:
	.4byte Func_02000ea4
.L_02009140:
	.4byte Data_0200502c
.L_02009144:
	.4byte gPartyState
	.section .text.x02009148,"ax",%progbits
	.global Func_02001148
	.thumb_func
Func_02001148:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r5, [r3]
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, [r5, #20]
	movs r3, #0
	str r3, [r0, #108]
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #3
	bl Battle_WaitMode0
	movs r1, #0
	ldr r0, [r5, #20]
	bl Object_SetPartAttribute
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r5, #20]
	bl Func_02000ed0
	bl Func_02004d48
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009190,"ax",%progbits
	.global Func_02001190
	.thumb_func
Func_02001190:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r6, r1, #0
	adds r7, r0, #0
	ldr r5, [r3]
	cmp r6, #0
	bne .L_020091a6
	bl Func_02004b18
.L_020091a6:
	cmp r7, #2
	bne .L_020091be
	ldr r3, [r5, #20]
	movs r2, #240
	ldr r1, [r3, #12]
	lsls r2, r2, #12
	ldr r0, [r3, #8]
	adds r1, r1, r2
	ldr r2, [r3, #16]
	movs r3, #1
	bl Func_02004a5c
.L_020091be:
	cmp r7, #3
	bne .L_020091e0
	ldr r3, [r5, #20]
	movs r2, #240
	ldr r1, [r3, #12]
	lsls r2, r2, #12
	ldr r0, [r3, #8]
	adds r1, r1, r2
	ldr r2, [r3, #16]
	movs r3, #8
	bl Func_02004a5c
	ldr r2, [r5, #20]
	movs r3, #0
	str r3, [r2, #8]
	str r3, [r2, #12]
	str r3, [r2, #16]
.L_020091e0:
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	cmp r6, r3
	bne .L_020091ee
	bl Func_02004bd4
.L_020091ee:
	cmp r6, #178
	bne .L_020091fa
	ldr r0, [r5, #20]
	movs r1, #3
	bl Func_02004c98
.L_020091fa:
	pop {r5, r6, r7, pc}
	.section .text.x020091fc,"ax",%progbits
	.global Func_020011fc
	.thumb_func
Func_020011fc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009344
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	adds r5, r0, #0
	ldrh r1, [r5, #6]
	movs r3, #128
	lsls r3, r3, #6
	adds r1, r1, r3
	movs r3, #192
	lsls r3, r3, #8
	ldr r2, .L_02009348
	ands r1, r3
	ldr r3, [r5, #8]
	movs r0, #128
	lsls r0, r0, #12
	ands r3, r2
	mov r6, sp
	adds r3, r3, r0
	str r3, [r6]
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	ands r3, r2
	adds r3, r3, r0
	movs r0, #128
	lsls r0, r0, #14
	adds r2, r6, #0
	str r3, [r6, #8]
	bl Vector_AddPolarOffsetFar
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_02000374
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02009336
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	movs r1, #6
	adds r0, r5, #0
	bl Func_02004c98
	movs r0, #6
	bl WaitFrames
	movs r0, #152
	bl Func_02004f30
	adds r0, r5, #0
	movs r1, #7
	bl Func_02004c98
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	ldr r3, .L_0200934c
	movs r2, #85
	str r3, [r5, #40]
	adds r2, r2, r5
	mov r10, r2
	ldrb r2, [r2]
	movs r3, #126
	ands r3, r2
	mov r2, r10
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, [r5, #8]
	ldr r0, [r6]
	movs r1, #12
	subs r0, r0, r3
	bl Engine_MathDivide
	ldr r3, [r5, #16]
	mov r11, r0
	ldr r0, [r6, #8]
	movs r1, #12
	subs r0, r0, r3
	bl Engine_MathDivide
	movs r3, #0
	mov r9, r0
	mov r8, r3
.L_020092ce:
	ldr r2, [r7, #8]
	ldr r3, [r6]
	movs r0, #1
	subs r2, r2, r3
	ldr r3, [r5, #8]
	add r2, r11
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r2, [r7, #16]
	ldr r3, [r6, #8]
	subs r2, r2, r3
	ldr r3, [r5, #16]
	add r2, r9
	adds r3, r3, r2
	str r3, [r5, #16]
	ldr r3, [r7, #8]
	str r3, [r6]
	ldr r3, [r7, #16]
	str r3, [r6, #8]
	bl WaitFrames
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #12
	bne .L_020092ce
	ldr r3, [r7, #8]
	movs r2, #128
	str r3, [r5, #8]
	ldr r3, [r7, #12]
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r5, #20]
	str r3, [r5, #12]
	ldr r3, [r7, #16]
	adds r0, r5, #0
	str r3, [r5, #16]
	movs r1, #6
	bl Func_02004c98
	adds r0, r5, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	mov r3, r10
	ldrb r2, [r3]
	movs r3, #3
	orrs r3, r2
	mov r2, r10
	strb r3, [r2]
	bl Func_02004d48
.L_02009336:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009344:
	.4byte gPartyState
.L_02009348:
	.4byte 0xfff00000
.L_0200934c:
	.4byte 0x0004cccc
	.section .text.x02009350,"ax",%progbits
	.global Func_02001350
	.thumb_func
Func_02001350:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #0
	sub sp, #76
	str r1, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #160
	lsls r2, r2, #1
	adds r2, r3, r2
	add r3, sp, #36
	mov r8, r3
	movs r3, #148
	adds r3, #255
	mov r11, r1
	mov r9, r1
	mov r10, r1
	mov r1, r8
	str r2, [sp, #16]
	strh r3, [r1, #24]
	movs r3, #1
	str r3, [r1]
	ldr r3, .L_020096b4
	movs r2, #240
	str r3, [r1, #28]
	movs r3, #230
	lsls r3, r3, #9
	adds r3, #204
	str r3, [r1, #8]
	str r3, [r1, #12]
	ldr r3, .L_020096b8
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldr r3, .L_020096bc
	add r2, sp, #24
	cmp r1, r3
	bne .L_020093c6
	movs r3, #164
	lsls r3, r3, #17
	str r3, [sp, #20]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r2, #4]
	movs r3, #200
	lsls r3, r3, #16
	movs r0, #192
	str r3, [r2, #8]
	movs r1, #20
	movs r2, #69
	lsls r0, r0, #2
	b .L_0200941c
.L_020093c6:
	ldr r3, .L_020096c0
	cmp r1, r3
	bne .L_020093fa
	movs r1, #232
	movs r3, #128
	lsls r3, r3, #14
	lsls r1, r1, #16
	str r1, [sp, #20]
	movs r0, #192
	str r3, [r2, #4]
	movs r3, #248
	lsls r3, r3, #16
	lsls r0, r0, #2
	str r3, [r2, #8]
	adds r0, #1
	movs r2, #14
	movs r3, #72
	mov r11, r2
	mov r9, r3
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200942c
	movs r1, #212
	mov r10, r1
	b .L_0200942c
.L_020093fa:
	ldr r3, .L_020096c4
	cmp r1, r3
	bne .L_0200942c
	movs r3, #196
	lsls r3, r3, #17
	str r3, [sp, #20]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r2, #4]
	movs r0, #192
	movs r3, #134
	lsls r3, r3, #18
	lsls r0, r0, #2
	str r3, [r2, #8]
	movs r1, #24
	movs r2, #90
	adds r0, #2
.L_0200941c:
	mov r11, r1
	mov r9, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200942c
	movs r3, #212
	mov r10, r3
.L_0200942c:
	movs r7, #0
.L_0200942e:
	bl Random16Far
	movs r5, #31
	ldr r1, [sp, #20]
	ands r0, r5
	lsls r0, r0, #16
	add r6, sp, #24
	adds r0, r1, r0
	str r0, [r6]
	movs r0, #145
	bl Func_02004f30
	bl Random16Far
	movs r3, #0
	ldr r4, [r6]
	ldr r1, [r6, #4]
	ldr r2, [r6, #8]
	str r3, [sp, #4]
	movs r3, #232
	lsls r3, r3, #14
	adds r3, #1
	ands r0, r5
	str r3, [sp, #8]
	lsls r0, r0, #12
	mov r3, r8
	str r0, [sp, #0]
	str r3, [sp, #12]
	adds r0, r4, #0
	movs r3, #0
	bl Func_0200015c
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004d10
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02004d10
	adds r7, #1
	movs r0, #8
	bl Battle_WaitMode0
	cmp r7, #16
	bne .L_0200942e
	movs r0, #148
	bl Func_02004f30
	mov r1, r8
	movs r3, #1
	str r3, [r1]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r1, #8]
	str r3, [r1, #12]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r1, #16]
	str r3, [r1, #20]
	ldr r2, [sp, #20]
	ldr r3, .L_020096c8
	movs r7, #0
	adds r2, r2, r3
	str r2, [sp, #20]
	adds r5, r2, #0
.L_020094c4:
	str r5, [r6]
	bl Random16Far
	movs r3, #0
	movs r4, #31
	ldr r1, [r6, #4]
	ldr r2, [r6, #8]
	ands r4, r0
	ldr r0, [r6]
	str r3, [sp, #0]
	movs r3, #224
	lsls r3, r3, #12
	adds r3, #1
	str r3, [sp, #8]
	mov r3, r8
	lsls r4, r4, #10
	str r3, [sp, #12]
	movs r3, #0
	str r4, [sp, #4]
	bl Func_0200015c
	movs r1, #192
	lsls r1, r1, #11
	adds r7, #1
	adds r5, r5, r1
	cmp r7, #8
	bne .L_020094c4
	mov r3, r9
	movs r5, #3
	movs r7, #6
	movs r0, #68
	movs r1, #70
	mov r2, r11
	str r5, [sp, #0]
	str r7, [sp, #4]
	bl Func_02004cd8
	movs r0, #160
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004d10
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02004d10
	movs r0, #30
	bl Battle_WaitMode0
	mov r2, r11
	mov r3, r9
	movs r0, #71
	movs r1, #70
	str r5, [sp, #0]
	str r7, [sp, #4]
	bl Func_02004cd8
	movs r3, #148
	mov r2, r8
	adds r3, #255
	strh r3, [r2, #24]
	movs r3, #1
	str r3, [r2]
	movs r3, #10
	str r3, [r2, #4]
	ldr r3, .L_020096cc
	str r3, [r2, #28]
	mov r3, r10
	cmp r3, #0
	beq .L_02009580
	movs r1, #70
	mov r3, r9
	movs r0, #68
	mov r2, r11
	str r5, [sp, #0]
	str r7, [sp, #4]
	bl Func_02004cd8
	movs r0, #30
	bl Battle_WaitMode0
	ldr r3, [r6, #8]
	ldr r1, .L_020096d0
	adds r3, r3, r1
	str r3, [r6, #8]
	b .L_02009742
.L_02009580:
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_02004d10
	movs r0, #107
	bl Func_02004f30
	movs r5, #0
.L_02009598:
	movs r7, #0
.L_0200959a:
	bl Random16Far
	movs r1, #48
	bl Engine_MathModulo
	ldr r2, [sp, #20]
	lsls r0, r0, #16
	adds r0, r2, r0
	ldr r2, [r6, #8]
	lsls r3, r5, #18
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #12
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #9
	str r0, [r6]
	ldr r1, [r6, #4]
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #204
	lsls r3, r3, #14
	str r3, [sp, #8]
	mov r3, r8
	str r3, [sp, #12]
	adds r7, #1
	movs r3, #0
	bl Func_0200015c
	cmp r7, #6
	bne .L_0200959a
	movs r0, #3
	bl WaitFrames
	lsrs r3, r5, #2
	adds r3, #6
	str r3, [sp, #4]
	movs r7, #3
	movs r0, #74
	movs r1, #70
	mov r2, r11
	mov r3, r9
	adds r5, #1
	str r7, [sp, #0]
	bl Func_02004cd8
	cmp r5, #12
	bne .L_02009598
	movs r3, #8
	str r3, [sp, #4]
	movs r1, #70
	movs r0, #74
	mov r2, r11
	mov r3, r9
	str r7, [sp, #0]
	bl Func_02004cd8
	movs r1, #0
	mov r10, r1
.L_02009612:
	movs r3, #3
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	bne .L_0200965c
	movs r7, #0
.L_0200961e:
	bl Random16Far
	movs r1, #48
	add r5, sp, #24
	bl Engine_MathModulo
	ldr r3, [sp, #20]
	ldr r2, [r5, #8]
	lsls r0, r0, #16
	adds r0, r3, r0
	movs r3, #192
	lsls r3, r3, #14
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #9
	str r0, [r5]
	ldr r1, [r5, #4]
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #204
	lsls r3, r3, #14
	str r3, [sp, #8]
	mov r3, r8
	str r3, [sp, #12]
	adds r7, #1
	movs r3, #0
	bl Func_0200015c
	cmp r7, #4
	bne .L_0200961e
.L_0200965c:
	mov r1, r10
	cmp r1, #0
	bne .L_02009678
	ldr r2, [sp, #16]
	ldr r1, .L_020096d4
	ldr r3, [r2, #12]
	adds r3, r3, r1
	str r3, [r2, #12]
	bl Func_02004cc0
	movs r0, #1
	bl Battle_WaitMode0
	b .L_0200972c
.L_02009678:
	mov r2, r10
	cmp r2, #63
	bhi .L_02009696
	ldr r3, .L_020096a8
	lsrs r2, r2, #2
	subs r3, r3, r2
	movs r1, #128
	lsls r1, r1, #19
	lsls r3, r3, #8
	orrs r3, r2
	adds r1, #82
	strh r3, [r1]
	movs r2, #128
	ldr r3, .L_020096ac
	b .L_020096a0
.L_02009696:
	mov r3, r10
	cmp r3, #64
	bne .L_020096d8
	movs r2, #128
	ldr r3, .L_020096b0
.L_020096a0:
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	b .L_0200972c
.L_020096a8:
	.4byte 0x00000010
.L_020096ac:
	.4byte 0x00003f44
.L_020096b0:
	.4byte 0x00000000
.L_020096b4:
	.4byte Data_02005050
.L_020096b8:
	.4byte gPartyState
.L_020096bc:
	.4byte 0x000000e3
.L_020096c0:
	.4byte 0x000000e4
.L_020096c4:
	.4byte 0x000000e5
.L_020096c8:
	.4byte 0xfff80000
.L_020096cc:
	.4byte Data_0200502c
.L_020096d0:
	.4byte 0xffd80000
.L_020096d4:
	.4byte 0xffe00000
.L_020096d8:
	mov r1, r10
	cmp r1, #127
	bhi .L_02009714
	ldr r2, [sp, #16]
	movs r1, #128
	ldr r3, [r2, #12]
	lsls r1, r1, #8
	adds r3, r3, r1
	str r3, [r2, #12]
	add r2, sp, #24
	ldr r3, [r2, #8]
	ldr r1, .L_02009814
	movs r0, #71
	adds r3, r3, r1
	str r3, [r2, #8]
	mov r3, r10
	subs r3, #47
	mov r2, r9
	lsrs r3, r3, #5
	subs r3, r2, r3
	movs r1, #1
	movs r2, #3
	str r2, [sp, #0]
	str r1, [sp, #4]
	adds r3, #8
	movs r1, #76
	mov r2, r11
	bl Func_02004cd8
	b .L_0200972c
.L_02009714:
	mov r3, r10
	cmp r3, #128
	bne .L_0200972c
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02004d10
.L_0200972c:
	movs r1, #1
	add r10, r1
	movs r0, #1
	bl WaitFrames
	mov r2, r10
	cmp r2, #160
	beq .L_0200973e
	b .L_02009612
.L_0200973e:
	bl Func_02004d18
.L_02009742:
	movs r0, #144
	lsls r0, r0, #1
	bl Func_02004f30
	movs r0, #136
	bl Func_02004f30
	movs r3, #230
	lsls r3, r3, #9
	add r6, sp, #36
	adds r3, #204
	str r3, [r6, #12]
	str r3, [r6, #8]
	movs r7, #0
.L_0200975e:
	bl Random16Far
	movs r1, #48
	add r5, sp, #24
	bl Engine_MathModulo
	ldr r3, [sp, #20]
	ldr r2, [r5, #8]
	lsls r0, r0, #16
	adds r0, r3, r0
	movs r3, #192
	lsls r3, r3, #14
	adds r2, r2, r3
	movs r3, #236
	lsls r3, r3, #14
	str r0, [r5]
	ldr r1, [r5, #4]
	adds r7, #1
	movs r5, #0
	str r3, [sp, #8]
	movs r3, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	str r6, [sp, #12]
	bl Func_0200015c
	cmp r7, #6
	bne .L_0200975e
	movs r0, #160
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004d10
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02004d10
	movs r3, #3
	str r3, [sp, #0]
	movs r1, #64
	mov r2, r11
	mov r3, r9
	movs r0, #68
	str r7, [sp, #4]
	bl Func_02004cd8
	bl Func_02004d18
	movs r0, #30
	bl Battle_WaitMode0
	ldr r1, [sp, #16]
	ldr r3, .L_02009818
	str r5, [r1, #12]
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200981c
	cmp r2, r3
	bne .L_020097f0
	bl Func_020034bc
	b .L_02009806
.L_020097f0:
	ldr r3, .L_02009820
	cmp r2, r3
	bne .L_020097fc
	bl Func_020034f0
	b .L_02009806
.L_020097fc:
	ldr r3, .L_02009824
	cmp r2, r3
	bne .L_02009806
	bl Func_02003548
.L_02009806:
	add sp, #76
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009814:
	.4byte 0xffff8000
.L_02009818:
	.4byte gPartyState
.L_0200981c:
	.4byte 0x000000e3
.L_02009820:
	.4byte 0x000000e4
.L_02009824:
	.4byte 0x000000e5
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
	ldr r7, .L_02009a9c
	movs r0, #133
	lsls r0, r0, #2
	adds r5, r7, r0
	ldr r0, [r5]
	sub sp, #92
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r1, #128
	lsls r1, r1, #6
	adds r1, r3, r1
	movs r3, #192
	lsls r3, r3, #8
	movs r2, #0
	ands r1, r3
	str r1, [sp, #36]
	str r2, [sp, #32]
	str r2, [sp, #28]
	str r2, [sp, #24]
	mov r10, r0
	mov r11, r2
	mov r9, r2
	mov r8, r2
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	mov r3, r10
	ldr r1, [r3, #8]
	ldr r0, .L_02009aa0
	movs r3, #128
	lsls r3, r3, #12
	ands r1, r0
	add r6, sp, #80
	adds r1, r1, r3
	str r1, [r6]
	mov r4, r10
	ldr r2, [r4, #16]
	asrs r1, r1, #16
	ands r2, r0
	adds r2, r2, r3
	str r2, [r6, #8]
	asrs r2, r2, #16
	ldr r0, [r5]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #146
	lsls r0, r0, #2
	bl Func_02004f30
	mov r0, r10
	adds r0, #35
	str r0, [sp, #20]
	movs r3, #191
	ldrb r2, [r0]
	ands r3, r2
	strb r3, [r0]
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Func_02004d10
	movs r0, #107
	bl Func_02004f30
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldr r3, .L_02009aa4
	cmp r2, r3
	bne .L_02009948
	movs r0, #20
	movs r3, #26
	movs r4, #160
	str r3, [sp, #24]
	mov r9, r0
	movs r2, #84
	mov r0, r11
	lsls r4, r4, #17
	movs r3, #212
	str r2, [sp, #28]
	str r4, [sp, #32]
	lsls r3, r3, #17
	str r0, [r6, #4]
	movs r0, #192
	movs r1, #89
	str r3, [r6, #8]
	lsls r0, r0, #2
	mov r8, r1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009914
	movs r1, #212
	mov r11, r1
.L_02009914:
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r2, #0
	str r2, [sp, #36]
	movs r1, #15
	movs r2, #13
	movs r3, #64
	movs r0, #0
	bl Func_02000694
	movs r0, #15
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r6, #0
.L_02009936:
	adds r0, r6, #0
	adds r0, #9
	movs r1, #1
	adds r6, #1
	bl Object_SetModeById
	cmp r6, #7
	bne .L_02009936
	b .L_02009a46
.L_02009948:
	ldr r1, .L_02009a9c
	movs r4, #240
	lsls r4, r4, #1
	adds r3, r1, r4
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009aa8
	cmp r2, r3
	bne .L_020099b8
	movs r3, #92
	movs r0, #224
	str r3, [sp, #28]
	lsls r0, r0, #17
	movs r3, #0
	movs r4, #21
	str r0, [sp, #32]
	str r4, [sp, #24]
	movs r0, #192
	str r3, [r6, #4]
	movs r3, #172
	lsls r3, r3, #17
	lsls r0, r0, #2
	movs r1, #28
	movs r2, #83
	str r3, [r6, #8]
	adds r0, #1
	mov r9, r1
	mov r8, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200998c
	movs r1, #212
	mov r11, r1
.L_0200998c:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_ClearBit
	movs r1, #24
	movs r2, #15
	movs r3, #64
	movs r0, #0
	bl Func_02000694
	movs r1, #9
	movs r0, #0
	movs r2, #16
	movs r3, #64
	bl Func_02000694
	movs r0, #11
	movs r1, #4
	bl Object_SetModeById
	b .L_02009a46
.L_020099b8:
	ldr r3, .L_02009aac
	cmp r2, r3
	bne .L_02009a46
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #8
	bgt .L_02009a0a
	movs r3, #13
	movs r4, #198
	str r3, [sp, #24]
	movs r0, #49
	movs r3, #0
	movs r2, #113
	lsls r4, r4, #18
	str r2, [sp, #28]
	str r4, [sp, #32]
	mov r9, r0
	str r3, [r6, #4]
	movs r0, #192
	movs r3, #216
	lsls r3, r3, #16
	lsls r0, r0, #2
	movs r1, #75
	str r3, [r6, #8]
	adds r0, #2
	mov r8, r1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020099fe
	movs r0, #212
	mov r11, r0
.L_020099fe:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
	b .L_02009a46
.L_02009a0a:
	movs r3, #115
	movs r0, #206
	str r3, [sp, #28]
	lsls r0, r0, #18
	movs r3, #0
	movs r4, #42
	str r0, [sp, #32]
	str r4, [sp, #24]
	movs r0, #192
	str r3, [r6, #4]
	movs r3, #170
	lsls r3, r3, #18
	lsls r0, r0, #2
	movs r1, #51
	movs r2, #104
	str r3, [r6, #8]
	adds r0, #2
	mov r9, r1
	mov r8, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009a3c
	movs r1, #212
	mov r11, r1
.L_02009a3c:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
.L_02009a46:
	movs r0, #181
	bl Func_02004f30
	movs r0, #160
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004d10
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02004d10
	mov r2, r8
	subs r2, #1
	str r2, [sp, #16]
	movs r3, #3
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	ldr r3, [sp, #16]
	mov r0, r9
	mov r1, r8
	mov r2, r9
	bl Func_02004cd8
	mov r3, r11
	cmp r3, #0
	bne .L_02009ab0
	bl Func_02004d18
	movs r0, #20
	bl Battle_WaitMode0
	b .L_02009b90
	.2byte 0x0000
.L_02009a9c:
	.4byte gPartyState
.L_02009aa0:
	.4byte 0xfff00000
.L_02009aa4:
	.4byte 0x000000e3
.L_02009aa8:
	.4byte 0x000000e4
.L_02009aac:
	.4byte 0x000000e6
.L_02009ab0:
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_02004d10
	movs r3, #148
	add r7, sp, #40
	adds r3, #255
	strh r3, [r7, #24]
	movs r3, #2
	str r3, [r7]
	movs r3, #10
	str r3, [r7, #4]
	ldr r3, .L_02009b78
	movs r4, #0
	str r3, [r7, #28]
	mov r8, r4
.L_02009ad8:
	movs r6, #0
.L_02009ada:
	bl Random16Far
	movs r1, #48
	add r5, sp, #80
	bl Engine_MathModulo
	mov r2, r8
	lsls r3, r2, #18
	ldr r2, [r5, #8]
	ldr r1, [sp, #32]
	adds r2, r2, r3
	movs r3, #184
	lsls r3, r3, #13
	adds r2, r2, r3
	lsls r0, r0, #16
	movs r3, #128
	adds r0, r1, r0
	lsls r3, r3, #9
	str r0, [r5]
	ldr r1, [r5, #4]
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #204
	lsls r3, r3, #14
	str r3, [sp, #8]
	adds r6, #1
	movs r3, #0
	str r7, [sp, #12]
	bl Func_0200015c
	cmp r6, #6
	bne .L_02009ada
	movs r0, #3
	bl WaitFrames
	mov r4, r8
	lsrs r3, r4, #1
	movs r2, #3
	adds r3, #1
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #75
	movs r1, #64
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	bl Func_02004cd8
	movs r0, #1
	add r8, r0
	mov r1, r8
	cmp r1, #10
	bne .L_02009ad8
	movs r6, #0
.L_02009b46:
	ldr r3, .L_02009b70
	lsrs r1, r6, #2
	movs r0, #128
	subs r3, r3, r1
	lsls r2, r1, #8
	lsls r0, r0, #19
	orrs r2, r3
	adds r0, #82
	strh r2, [r0]
	ldr r3, .L_02009b74
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r0, #3
	adds r6, #1
	bl WaitFrames
	cmp r6, #64
	bne .L_02009b46
	b .L_02009b7c
.L_02009b70:
	.4byte 0x00000010
.L_02009b74:
	.4byte 0x00003f44
.L_02009b78:
	.4byte Data_0200502c
.L_02009b7c:
	movs r3, #3
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #78
	movs r1, #64
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	bl Func_02004cd8
.L_02009b90:
	ldr r5, .L_02009c94
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldr r3, .L_02009c98
	cmp r2, r3
	bne .L_02009bb2
	movs r0, #15
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
.L_02009bb2:
	movs r0, #144
	lsls r0, r0, #1
	bl Func_02004f30
	movs r0, #181
	bl Func_02004f30
	movs r0, #160
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004d10
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02004d10
	movs r3, #3
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	ldr r3, [sp, #16]
	movs r1, #63
	mov r2, r9
	movs r0, #72
	bl Func_02004cd8
	bl Func_02004d18
	movs r0, #146
	lsls r0, r0, #2
	bl Func_02004f30
	ldr r0, [sp, #20]
	movs r3, #64
	ldrb r2, [r0]
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #133
	lsls r1, r1, #2
	adds r6, r5, r1
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	mov r2, r10
	ldr r3, [r2, #8]
	ldr r1, .L_02009c9c
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	add r5, sp, #80
	adds r3, r3, r2
	str r3, [r5]
	mov r4, r10
	ldr r3, [r4, #12]
	str r3, [r5, #4]
	ldr r3, [r4, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r0, [sp, #36]
	movs r1, #128
	lsls r1, r1, #8
	eors r1, r0
	movs r0, #128
	adds r2, r5, #0
	lsls r0, r0, #13
	bl Vector_AddPolarOffsetFar
	add r1, sp, #36
	ldrh r1, [r1]
	mov r2, r10
	strh r1, [r2, #6]
	movs r2, #2
	ldrsh r1, [r5, r2]
	ldr r0, [r6]
	movs r3, #10
	ldrsh r2, [r5, r3]
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	bl Func_02004d48
	add sp, #92
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009c94:
	.4byte gPartyState
.L_02009c98:
	.4byte 0x000000e3
.L_02009c9c:
	.4byte 0xfff00000
	.section .text.x02009ca0,"ax",%progbits
	.global Func_02001ca0
	.thumb_func
Func_02001ca0:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r6, r0, #0
	movs r0, #9
	ldr r5, [r3]
	mov r8, r1
	bl Object_GetById
	mov r1, r8
	str r0, [r5, #20]
	adds r0, r6, #0
	bl Func_02001190
	movs r0, #212
	lsls r0, r0, #2
	bl GameFlag_SetBit
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.section .text.x02009cd0,"ax",%progbits
	.global Func_02001cd0
	.thumb_func
Func_02001cd0:
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
	bl Func_02001190
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #81
	bl GameFlag_SetBit
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02009d04,"ax",%progbits
	.global Func_02001d04
	.thumb_func
Func_02001d04:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #100
	ldrh r3, [r1]
	movs r2, #128
	adds r3, #1
	strh r3, [r1]
	lsls r2, r2, #15
	lsls r3, r3, #16
	sub sp, #56
	cmp r3, r2
	ble .L_02009d22
	movs r3, #0
	str r3, [r5, #108]
.L_02009d22:
	ldrh r3, [r1]
	movs r6, #1
	ands r6, r3
	cmp r6, #0
	bne .L_02009d5c
	movs r3, #148
	add r4, sp, #16
	adds r3, #255
	strh r3, [r4, #24]
	movs r3, #2
	str r3, [r4]
	movs r3, #10
	str r3, [r4, #4]
	ldr r3, .L_02009d60
	ldr r0, [r5, #8]
	str r3, [r4, #28]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [sp, #0]
	movs r3, #204
	lsls r3, r3, #14
	str r3, [sp, #8]
	ldr r1, [r5, #12]
	ldr r2, [r5, #16]
	movs r3, #0
	str r6, [sp, #4]
	str r4, [sp, #12]
	bl Func_0200015c
.L_02009d5c:
	add sp, #56
	pop {r5, r6, pc}
.L_02009d60:
	.4byte Data_0200502c
	.section .text.x02009d64,"ax",%progbits
	.global Func_02001d64
	.thumb_func
Func_02001d64:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #68
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	movs r0, #107
	bl Func_02004f30
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02004d10
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #194
	movs r1, #1
	movs r2, #136
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02004e20
	movs r3, #148
	add r6, sp, #16
	adds r3, #255
	strh r3, [r6, #24]
	movs r3, #2
	str r3, [r6]
	movs r3, #10
	str r3, [r6, #4]
	ldr r3, .L_0200a11c
	movs r2, #0
	str r3, [r6, #28]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r6, #12]
	str r3, [r6, #8]
	add r3, sp, #56
	str r2, [r3, #4]
	mov r8, r3
	movs r7, #0
.L_02009dce:
	bl Random16Far
	movs r5, #31
	ands r0, r5
	movs r2, #190
	lsls r2, r2, #18
	lsls r0, r0, #16
	adds r0, r0, r2
	mov r3, r8
	str r0, [r3]
	bl Random16Far
	adds r2, r0, #0
	ands r2, r5
	movs r3, #128
	lsls r3, r3, #17
	lsls r2, r2, #16
	adds r2, r2, r3
	mov r3, r8
	str r2, [r3, #8]
	ldr r0, [r3]
	ldr r1, [r3, #4]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [sp, #0]
	movs r3, #236
	lsls r3, r3, #14
	str r3, [sp, #8]
	movs r5, #0
	movs r3, #0
	str r5, [sp, #4]
	str r6, [sp, #12]
	bl Func_0200015c
	movs r3, #7
	ands r3, r7
	cmp r3, #0
	bne .L_02009e24
	movs r0, #156
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02004f30
.L_02009e24:
	cmp r7, #32
	bne .L_02009e68
	movs r1, #194
	movs r2, #248
	movs r0, #13
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #186
	lsls r1, r1, #2
	movs r2, #216
	movs r0, #13
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #13
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #13
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #13
	bl Object_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #13
	b .L_0200a04c
.L_02009e68:
	cmp r7, #40
	bne .L_02009eae
	movs r1, #194
	movs r2, #248
	movs r0, #20
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #194
	movs r2, #164
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #20
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #20
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #20
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #20
	bl Object_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #20
	b .L_0200a04c
.L_02009eae:
	cmp r7, #48
	bne .L_02009ef2
	movs r1, #194
	movs r2, #248
	movs r0, #14
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #214
	lsls r1, r1, #2
	movs r2, #232
	movs r0, #14
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #14
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #14
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #14
	bl Object_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #14
	b .L_0200a04c
.L_02009ef2:
	cmp r7, #56
	bne .L_02009f38
	movs r1, #194
	movs r2, #248
	movs r0, #19
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #174
	movs r2, #156
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #19
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #19
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #19
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #19
	bl Object_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #19
	b .L_0200a04c
.L_02009f38:
	cmp r7, #64
	bne .L_02009f7e
	movs r1, #194
	movs r2, #248
	movs r0, #15
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #182
	movs r2, #140
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #15
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #15
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #15
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #15
	bl Object_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #15
	b .L_0200a04c
.L_02009f7e:
	cmp r7, #72
	bne .L_02009fc4
	movs r1, #194
	movs r2, #248
	movs r0, #18
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #206
	movs r2, #140
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #18
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #18
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #18
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #18
	bl Object_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #18
	b .L_0200a04c
.L_02009fc4:
	cmp r7, #80
	bne .L_0200a00a
	movs r1, #194
	movs r2, #248
	movs r0, #16
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #206
	movs r2, #156
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #16
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #16
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #16
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #16
	bl Object_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #16
	b .L_0200a04c
.L_0200a00a:
	cmp r7, #88
	bne .L_0200a056
	movs r1, #194
	movs r2, #248
	movs r0, #17
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #194
	lsls r1, r1, #2
	movs r2, #232
	movs r0, #17
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #17
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #17
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #17
	bl Object_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #17
.L_0200a04c:
	bl Object_GetById
	ldr r3, .L_0200a120
	str r3, [r0, #108]
	b .L_0200a0c4
.L_0200a056:
	cmp r7, #112
	bne .L_0200a0c4
	movs r1, #194
	movs r2, #248
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004da0
	movs r1, #174
	movs r0, #12
	lsls r1, r1, #2
	movs r2, #232
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #174
	movs r1, #1
	movs r2, #232
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #18
	bl Motion_CamBounds
	movs r0, #12
	bl Object_GetById
	movs r3, #208
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r0, #12
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #72]
	movs r0, #12
	bl Object_GetById
	adds r0, #100
	strh r5, [r0]
	movs r0, #12
	bl Object_GetById
	ldr r3, .L_0200a120
	movs r2, #230
	str r3, [r0, #108]
	movs r1, #1
	movs r0, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02004d10
.L_0200a0c4:
	movs r0, #3
	adds r7, #1
	bl WaitFrames
	cmp r7, #128
	beq .L_0200a0d2
	b .L_02009dce
.L_0200a0d2:
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02004d18
	movs r0, #144
	lsls r0, r0, #1
	bl Func_02004f30
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #248
	bl GameFlag_SetBit
	ldr r3, .L_0200a124
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02004e20
	movs r0, #10
	bl WaitFrames
	bl Func_02004d48
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a11c:
	.4byte Data_02005050
.L_0200a120:
	.4byte Func_02001d04
.L_0200a124:
	.4byte gPartyState
	.section .text.x0200a128,"ax",%progbits
	.global Func_02002128
	.thumb_func
Func_02002128:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #56
	adds r3, r3, r2
	movs r0, #193
	movs r2, #1
	strb r2, [r3]
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a178
	movs r0, #22
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #158
	movs r2, #156
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r0, #22
	bl Func_02004da0
	movs r0, #22
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #22
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r0, #28]
	str r3, [r5, #24]
.L_0200a178:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200a17c,"ax",%progbits
	.global Func_0200217c
	.thumb_func
Func_0200217c:
	push {r5, lr}
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a1bc
	movs r0, #21
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #150
	movs r2, #156
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r0, #21
	bl Func_02004da0
	movs r0, #21
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #21
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r0, #28]
	str r3, [r5, #24]
.L_0200a1bc:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200a1c0,"ax",%progbits
	.global Func_020021c0
	.thumb_func
Func_020021c0:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	movs r2, #211
	lsls r2, r2, #4
	adds r3, r7, r2
	movs r0, #0
	ldrsb r0, [r3, r0]
	bl Object_GetById
	movs r1, #0
	adds r5, r0, #0
	bl Object_SetPartAttribute
	movs r0, #136
	bl Func_02004f30
	movs r6, #0
.L_0200a1e6:
	ldr r3, [r5, #24]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r0, #5
	adds r6, #1
	bl WaitFrames
	cmp r6, #16
	bne .L_0200a1e6
	movs r2, #211
	lsls r2, r2, #4
	adds r3, r7, r2
	movs r0, #0
	ldrsb r0, [r3, r0]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #238
	adds r0, r0, r3
	bl GameFlag_SetBit
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a224
	b .L_0200a314
.L_0200a224:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a314
	movs r0, #11
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02004d10
	movs r0, #141
	bl Func_02004f30
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #154
	movs r1, #1
	movs r2, #148
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02004e20
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #151
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02004f30
	ldr r3, .L_0200a2c0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200a2c4
	subs r2, #2
	strh r3, [r2]
	ldr r2, [r5, #16]
	movs r3, #192
	lsls r3, r3, #14
	adds r2, r2, r3
	movs r3, #156
	lsls r3, r3, #1
	ldr r0, [r5, #8]
	ldr r1, [r5, #20]
	bl Func_02000080
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r7, r0, #0
	movs r6, #0
.L_0200a2b4:
	ldr r3, [r5, #12]
	ldr r2, .L_0200a2c8
	movs r0, #5
	adds r3, r3, r2
	str r3, [r5, #12]
	b .L_0200a2cc
.L_0200a2c0:
	.4byte 0x00001000
.L_0200a2c4:
	.4byte 0x00003f10
.L_0200a2c8:
	.4byte 0xffff0000
.L_0200a2cc:
	adds r6, #1
	bl Battle_WaitMode0
	cmp r6, #32
	bne .L_0200a2b4
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #42
	bl Func_02004f30
	movs r1, #4
	adds r0, r5, #0
	bl Func_02004c98
	adds r0, r7, #0
	bl Func_02004cb0
	movs r0, #1
	bl Battle_WaitMode0
	ldr r3, .L_0200a310
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r1, #38
	movs r2, #18
	movs r3, #0
	movs r0, #0
	bl Func_02000694
	bl Func_02004d48
	b .L_0200a314
.L_0200a310:
	.4byte 0x00000000
.L_0200a314:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a318,"ax",%progbits
	.global Func_02002318
	.thumb_func
Func_02002318:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #211
	lsls r2, r2, #4
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_0200a362
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a34a
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
.L_0200a34a:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a362
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
.L_0200a362:
	pop {pc}
	.section .text.x0200a364,"ax",%progbits
	.global Func_02002364
	.thumb_func
Func_02002364:
	push {r5, lr}
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	ldr r5, .L_0200a3e0
	movs r1, #1
	adds r0, r5, #0
	adds r5, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r5, #0
	bl Func_02004dd0
	movs r1, #0
	movs r0, #12
	bl UiText_OpenMessageAtObject
	ldr r3, .L_0200a3e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200a3da
	movs r0, #230
	movs r1, #0
	lsls r0, r0, #1
	bl PartyInventory_GiveItem
	movs r0, #230
	lsls r0, r0, #1
	bl PartyInventory_FindOwner
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_0200a3be
	bl Func_02004d48
	b .L_0200a3de
.L_0200a3be:
	movs r0, #230
	lsls r0, r0, #1
	movs r1, #3
	bl Func_02004e58
	movs r0, #12
	movs r1, #2
	bl Object_SetModeById
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #249
	bl GameFlag_SetBit
.L_0200a3da:
	bl Func_02004d48
.L_0200a3de:
	pop {r5, pc}
.L_0200a3e0:
	.4byte 0x000028cc
.L_0200a3e4:
	.4byte gPartyState
	.section .text.x0200a3e8,"ax",%progbits
	.global Func_020023e8
	.thumb_func
Func_020023e8:
	push {r5, r6, lr}
	sub sp, #8
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	movs r0, #188
	bl Func_02004f30
	movs r5, #1
	movs r6, #2
	movs r1, #67
	movs r2, #15
	movs r3, #19
	movs r0, #72
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004cd8
	movs r0, #8
	bl Battle_WaitMode0
	movs r1, #67
	movs r2, #15
	movs r3, #19
	movs r0, #73
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004cd8
	movs r0, #8
	bl Battle_WaitMode0
	movs r1, #67
	movs r2, #15
	movs r3, #19
	movs r0, #74
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004cd8
	movs r0, #8
	bl Battle_WaitMode0
	movs r1, #67
	movs r2, #15
	movs r3, #19
	movs r0, #75
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004cd8
	movs r0, #15
	bl Battle_WaitMode0
	ldr r5, .L_0200a4a8
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #52]
	str r3, [r6, #48]
	movs r0, #123
	bl Func_02004f30
	movs r2, #164
	movs r1, #248
	lsls r2, r2, #1
	ldr r0, [r5]
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #1
	bl Func_02004e28
	bl Func_02004d48
	add sp, #8
	pop {r5, r6, pc}
.L_0200a4a8:
	.4byte gPartyState
	.section .text.x0200a4ac,"ax",%progbits
	.global Func_020024ac
	.thumb_func
Func_020024ac:
	push {r5, r6, r7, lr}
	sub sp, #8
	bl Func_02004f00
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r6, r3, #20
	ldr r3, [r0, #16]
	asrs r5, r3, #20
	lsls r3, r6, #16
	adds r1, r3, r5
	ldr r3, [r0, #36]
	cmp r3, #0
	bne .L_0200a53c
	ldr r3, [r0, #44]
	cmp r3, #0
	bne .L_0200a53c
	ldr r2, .L_0200a540
	movs r0, #1
	ldr r3, [r2]
	negs r0, r0
	cmp r3, r0
	beq .L_0200a53c
	cmp r1, r3
	beq .L_0200a53c
	str r0, [r2]
	ldr r1, .L_0200a544
	movs r5, #255
	lsls r5, r5, #8
	adds r5, #255
	ldr r0, [r1]
	asrs r6, r3, #16
	ldr r1, .L_0200a548
	ands r5, r3
	ldr r3, .L_0200a54c
	ldr r1, [r1]
	ldr r2, [r3]
	ldr r3, .L_0200a550
	ldr r3, [r3]
	str r0, [sp, #0]
	str r1, [sp, #4]
	movs r0, #73
	movs r1, #72
	bl Func_02004cf0
	ldr r3, .L_0200a554
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200a558
	cmp r2, r3
	bne .L_0200a53c
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #2
	adds r1, r6, #0
	adds r2, r5, #0
	ldr r7, [r3, #108]
	bl Func_02000658
	asrs r0, r0, #8
	cmp r0, #0
	beq .L_0200a53c
	movs r1, #181
	adds r3, r0, #0
	lsls r1, r1, #1
	adds r3, #200
	adds r2, r7, r1
	strh r3, [r2]
.L_0200a53c:
	add sp, #8
	pop {r5, r6, r7, pc}
.L_0200a540:
	.4byte Data_02006710
.L_0200a544:
	.4byte gOverlayArea + 0x67f4
.L_0200a548:
	.4byte gOverlayArea + 0x67f8
.L_0200a54c:
	.4byte gOverlayArea + 0x67ec
.L_0200a550:
	.4byte gOverlayArea + 0x67f0
.L_0200a554:
	.4byte gPartyState
.L_0200a558:
	.4byte 0x000000e9
	.section .text.x0200a55c,"ax",%progbits
	.global Func_0200255c
	.thumb_func
Func_0200255c:
	push {lr}
	ldr r3, .L_0200a574
	movs r2, #1
	negs r2, r2
	movs r1, #144
	str r2, [r3]
	lsls r1, r1, #3
	ldr r0, .L_0200a578
	bl Scheduler_AddOrUpdateCallback
	pop {pc}
	.2byte 0x0000
.L_0200a574:
	.4byte Data_02006710
.L_0200a578:
	.4byte Func_020024ac
	.section .text.x0200a57c,"ax",%progbits
	.global Func_0200257c
	.thumb_func
Func_0200257c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	bl Func_02004f00
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r4, r3, #20
	ldr r3, [r0, #12]
	asrs r7, r3, #20
	ldr r3, [r0, #16]
	ldr r0, .L_0200a5fc
	asrs r1, r3, #20
	lsls r3, r4, #16
	adds r2, r3, r1
	ldr r3, [r0]
	cmp r2, r3
	beq .L_0200a5f0
	str r2, [r0]
	ldr r2, .L_0200a600
	movs r3, #1
	str r3, [r2]
	ldr r3, .L_0200a604
	ldr r6, .L_0200a608
	mov r8, r3
	ldr r5, .L_0200a60c
	mov r10, r2
	movs r3, #2
	mov r2, r8
	adds r0, r4, #0
	str r3, [r2]
	adds r0, #64
	subs r1, r1, r7
	movs r3, #73
	movs r2, #72
	str r0, [r6]
	str r1, [r5]
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #2
	movs r2, #1
	bl Func_02004cf0
	mov r3, r10
	mov r1, r8
	ldr r2, [r3]
	ldr r0, [r5]
	ldr r3, [r1]
	ldr r1, [r6]
	str r0, [sp, #4]
	str r1, [sp, #0]
	movs r0, #70
	movs r1, #72
	bl Func_02004cf0
.L_0200a5f0:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a5fc:
	.4byte Data_02006710
.L_0200a600:
	.4byte gOverlayArea + 0x67ec
.L_0200a604:
	.4byte gOverlayArea + 0x67f0
.L_0200a608:
	.4byte gOverlayArea + 0x67f4
.L_0200a60c:
	.4byte gOverlayArea + 0x67f8
	.section .text.x0200a610,"ax",%progbits
	.global Func_02002610
	.thumb_func
Func_02002610:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	bl Func_02004f00
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r4, r3, #20
	ldr r3, [r0, #12]
	asrs r7, r3, #20
	ldr r3, [r0, #16]
	ldr r0, .L_0200a690
	asrs r1, r3, #20
	lsls r3, r4, #16
	adds r2, r3, r1
	ldr r3, [r0]
	cmp r2, r3
	beq .L_0200a684
	str r2, [r0]
	ldr r2, .L_0200a694
	movs r3, #1
	str r3, [r2]
	ldr r3, .L_0200a698
	ldr r6, .L_0200a69c
	mov r8, r3
	ldr r5, .L_0200a6a0
	mov r10, r2
	movs r3, #2
	mov r2, r8
	adds r0, r4, #0
	str r3, [r2]
	adds r0, #64
	subs r1, r1, r7
	movs r3, #73
	movs r2, #72
	str r0, [r6]
	str r1, [r5]
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #2
	movs r2, #1
	bl Func_02004cf0
	mov r3, r10
	mov r1, r8
	ldr r2, [r3]
	ldr r0, [r5]
	ldr r3, [r1]
	ldr r1, [r6]
	str r0, [sp, #4]
	str r1, [sp, #0]
	movs r0, #71
	movs r1, #72
	bl Func_02004cf0
.L_0200a684:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a690:
	.4byte Data_02006710
.L_0200a694:
	.4byte gOverlayArea + 0x67ec
.L_0200a698:
	.4byte gOverlayArea + 0x67f0
.L_0200a69c:
	.4byte gOverlayArea + 0x67f4
.L_0200a6a0:
	.4byte gOverlayArea + 0x67f8
	.section .text.x0200a6a4,"ax",%progbits
	.global Func_020026a4
	.thumb_func
Func_020026a4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r1
	adds r6, r0, #0
	mov r2, r8
	movs r0, #144
	lsls r3, r2, #16
	lsls r1, r6, #16
	movs r2, #0
	lsls r0, r0, #1
	sub sp, #8
	bl Func_02004ca8
	ldr r3, .L_0200a6f8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200a6fc
	subs r2, #2
	strh r3, [r2]
	adds r7, r0, #0
	movs r1, #0
	bl Object_SetSpritePriority
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, [r7, #80]
	movs r3, #13
	ldrb r2, [r1, #5]
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r1, #5]
	bl Func_02004d40
	movs r0, #0
	b .L_0200a700
.L_0200a6f8:
	.4byte 0x00000010
.L_0200a6fc:
	.4byte 0x00003f44
.L_0200a700:
	bl Func_02004e90
	ldr r3, .L_0200a73c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #74
	strh r3, [r2]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200a740
	movs r0, #106
	orrs r3, r2
	strh r3, [r1]
	bl Func_02004f30
	movs r5, #0
.L_0200a722:
	ldr r1, .L_0200a744
	movs r3, #128
	lsls r2, r5, #9
	lsls r3, r3, #19
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	b .L_0200a748
	.2byte 0x0000
.L_0200a73c:
	.4byte 0x00003f1f
.L_0200a740:
	.4byte 0x00008000
.L_0200a744:
	.4byte 0x00000010
.L_0200a748:
	cmp r5, #8
	bne .L_0200a722
	movs r5, #0
.L_0200a74e:
	ldr r2, .L_0200a76c
	ldr r1, .L_0200a770
	movs r3, #128
	subs r2, r2, r5
	lsls r3, r3, #19
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #16
	bne .L_0200a74e
	b .L_0200a774
.L_0200a76c:
	.4byte 0x00000010
.L_0200a770:
	.4byte 0x00001000
.L_0200a774:
	mov r3, r8
	asrs r6, r6, #4
	asrs r5, r3, #4
	subs r5, #1
	adds r2, r6, #0
	movs r3, #1
	movs r1, #2
	str r3, [sp, #0]
	str r1, [sp, #4]
	adds r3, r5, #0
	adds r2, #64
	movs r0, #65
	movs r1, #1
	bl Func_02004cd8
	movs r3, #255
	adds r1, r6, #0
	adds r2, r5, #0
	lsls r3, r3, #8
	movs r0, #0
	bl Func_02000694
	adds r0, r7, #0
	bl Func_02004cb0
	bl Func_02004d48
	ldr r3, .L_0200a7c8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	add sp, #8
	b .L_0200a7cc
.L_0200a7c8:
	.4byte 0x00000000
.L_0200a7cc:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a7d4,"ax",%progbits
	.global Func_020027d4
	.thumb_func
Func_020027d4:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	ldr r3, .L_0200a864
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #2
	bl Func_02000658
	movs r3, #181
	lsls r3, r3, #1
	adds r5, r5, r3
	movs r2, #0
	ldrsh r3, [r5, r2]
	movs r6, #54
	subs r3, #201
	lsls r3, r3, #2
	asrs r0, r0, #8
	subs r6, r6, r3
	cmp r0, #1
	beq .L_0200a848
	cmp r0, #7
	beq .L_0200a848
	bl Func_0200257c
	movs r0, #132
	lsls r1, r6, #3
	lsls r0, r0, #1
	adds r1, #10
	bl Func_020026a4
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a838
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_0200a838:
	movs r3, #0
	ldrsh r0, [r5, r3]
	movs r2, #156
	lsls r2, r2, #1
	adds r0, r0, r2
	bl GameFlag_SetBit
	b .L_0200a860
.L_0200a848:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a860
	movs r0, #132
	lsls r1, r6, #3
	lsls r0, r0, #1
	adds r1, #10
	bl Func_020026a4
.L_0200a860:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a864:
	.4byte gPartyState
	.section .text.x0200a868,"ax",%progbits
	.global Func_02002868
	.thumb_func
Func_02002868:
	push {r5, lr}
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a8a8
	movs r0, #11
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #136
	movs r2, #180
	lsls r1, r1, #16
	lsls r2, r2, #17
	movs r0, #11
	bl Func_02004da0
	movs r0, #11
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #11
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r0, #28]
	str r3, [r5, #24]
.L_0200a8a8:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200a8ac,"ax",%progbits
	.global Func_020028ac
	.thumb_func
Func_020028ac:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #211
	lsls r2, r2, #4
	adds r3, r3, r2
	movs r0, #0
	ldrsb r0, [r3, r0]
	bl Object_GetById
	movs r1, #0
	adds r5, r0, #0
	bl Object_SetPartAttribute
	movs r0, #136
	bl Func_02004f30
	movs r6, #0
.L_0200a8d2:
	ldr r3, [r5, #24]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r0, #5
	adds r6, #1
	bl WaitFrames
	cmp r6, #16
	bne .L_0200a8d2
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_SetBit
	movs r0, #9
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02004d10
	movs r0, #141
	bl Func_02004f30
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #240
	movs r1, #1
	movs r2, #172
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #15
	bl Motion_CamBounds
	bl Func_02004e20
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #151
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02004f30
	ldr r3, .L_0200a984
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200a988
	subs r2, #2
	strh r3, [r2]
	ldr r2, [r5, #16]
	movs r3, #192
	lsls r3, r3, #14
	adds r2, r2, r3
	movs r3, #156
	lsls r3, r3, #1
	ldr r0, [r5, #8]
	ldr r1, [r5, #20]
	bl Func_02000080
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r7, r0, #0
	movs r6, #0
.L_0200a978:
	ldr r3, [r5, #12]
	ldr r2, .L_0200a98c
	movs r0, #5
	adds r3, r3, r2
	str r3, [r5, #12]
	b .L_0200a990
.L_0200a984:
	.4byte 0x00001000
.L_0200a988:
	.4byte 0x00003f10
.L_0200a98c:
	.4byte 0xffff0000
.L_0200a990:
	adds r6, #1
	bl Battle_WaitMode0
	cmp r6, #32
	bne .L_0200a978
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #42
	bl Func_02004f30
	movs r1, #4
	adds r0, r5, #0
	bl Func_02004c98
	adds r0, r7, #0
	bl Func_02004cb0
	movs r0, #1
	bl Battle_WaitMode0
	ldr r3, .L_0200a9d4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r1, #7
	movs r2, #21
	movs r3, #128
	movs r0, #0
	bl Func_02000694
	bl Func_02004d48
	b .L_0200a9d8
.L_0200a9d4:
	.4byte 0x00000000
.L_0200a9d8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a9dc,"ax",%progbits
	.global Func_020029dc
	.thumb_func
Func_020029dc:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #211
	lsls r2, r2, #4
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_0200aa10
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200aa10
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
.L_0200aa10:
	pop {pc}
	.2byte 0x0000
	.section .text.x0200aa14,"ax",%progbits
	.global Func_02002a14
	.thumb_func
Func_02002a14:
	push {r5, lr}
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	movs r0, #168
	lsls r0, r0, #2
	bl Func_02004ca8
	movs r3, #204
	adds r5, r0, #0
	lsls r3, r3, #7
	adds r3, #102
	adds r2, r5, #0
	adds r2, #85
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r3, #0
	strb r3, [r2]
	movs r1, #10
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetSpritePriority
	adds r0, r5, #0
	movs r1, #1
	bl Func_02004c98
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	ldr r1, .L_0200aa68
	adds r0, r5, #0
	bl Func_02004ca0
	pop {r5, pc}
.L_0200aa68:
	.4byte Data_02005534
	.section .text.x0200aa6c,"ax",%progbits
	.global Func_02002a6c
	.thumb_func
Func_02002a6c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	movs r2, #102
	adds r2, r2, r6
	adds r5, r6, #0
	mov r10, r2
	ldrh r2, [r2]
	adds r5, #100
	ldrh r3, [r5]
	lsls r2, r2, #16
	ldr r7, [r6, #104]
	asrs r2, r2, #17
	adds r3, r3, r2
	strh r3, [r5]
	ldr r3, [r7, #8]
	movs r2, #128
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	movs r2, #8
	str r3, [r6, #16]
	adds r2, r2, r6
	movs r3, #0
	ldrsh r1, [r5, r3]
	ldr r0, [r6, #76]
	mov r8, r2
	bl Vector_AddPolarOffsetFar
	adds r2, r6, #0
	adds r2, #98
	ldrb r3, [r2]
	movs r0, #0
	adds r3, #255
	strb r3, [r2]
	lsls r3, r3, #24
	cmp r3, #0
	beq .L_0200ab1c
	ldr r3, [r6, #76]
	movs r2, #128
	lsls r2, r2, #10
	movs r0, #1
	cmp r3, r2
	beq .L_0200ab1c
	adds r0, r6, #0
	bl Func_02002a14
	ldr r2, .L_0200ab28
	ldr r3, [r6, #76]
	mov r9, r2
	add r3, r9
	str r3, [r6, #76]
	mov r3, r10
	ldrh r2, [r3]
	ldrh r3, [r5]
	lsls r2, r2, #16
	asrs r2, r2, #17
	adds r3, r3, r2
	strh r3, [r5]
	ldr r3, [r7, #8]
	mov r2, r8
	str r3, [r2]
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	ldr r0, [r6, #76]
	str r3, [r6, #16]
	mov r2, r8
	movs r3, #0
	ldrsh r1, [r5, r3]
	bl Vector_AddPolarOffsetFar
	adds r0, r6, #0
	bl Func_02002a14
	ldr r3, [r6, #76]
	movs r0, #1
	add r3, r9
	str r3, [r6, #76]
.L_0200ab1c:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ab28:
	.4byte 0xfffc0000
	.section .text.x0200ab2c,"ax",%progbits
	.global Func_02002b2c
	.thumb_func
Func_02002b2c:
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
	ldr r6, .L_0200abc8
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
	beq .L_0200aba4
	ldr r3, .L_0200abcc
	movs r2, #1
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200aba4
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
.L_0200aba4:
	mov r3, r8
	ldrh r2, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	movs r0, #0
	cmp r3, #0
	beq .L_0200abc2
	ldrb r3, [r5]
	cmp r3, #141
	bne .L_0200abc0
	adds r3, r2, #0
	subs r3, #128
	mov r1, r8
	strh r3, [r1]
.L_0200abc0:
	movs r0, #1
.L_0200abc2:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200abc8:
	.4byte IwramMulQ16
.L_0200abcc:
	.4byte Data_0300122c
	.section .text.x0200abd0,"ax",%progbits
	.global Func_02002bd0
	.thumb_func
Func_02002bd0:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r5, r0, #0
	ldr r6, [r5, #104]
	ldr r3, [r5, #8]
	ldr r0, [r6, #8]
	movs r1, #10
	subs r0, r0, r3
	movs r3, #10
	mov r8, r3
	bl Engine_MathDivide
	str r0, [r5, #68]
	ldr r3, [r5, #12]
	ldr r0, [r6, #12]
	movs r1, #10
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #13
	adds r0, r0, r3
	bl Engine_MathDivide
	str r0, [r5, #76]
	ldr r3, [r5, #16]
	ldr r0, [r6, #16]
	movs r1, #10
	subs r0, r0, r3
	bl Engine_MathDivide
	mov r3, r8
	str r0, [r5, #72]
	adds r5, #98
	strb r3, [r5]
	movs r0, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.section .text.x0200ac1c,"ax",%progbits
	.global Func_02002c1c
	.thumb_func
Func_02002c1c:
	ldr r3, [r0, #8]
	ldr r2, [r0, #68]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, [r0, #76]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [r0, #72]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	adds r0, #98
	ldrb r3, [r0]
	adds r3, #255
	strb r3, [r0]
	lsls r3, r3, #24
	lsrs r3, r3, #24
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
	bx lr
	.section .text.x0200ac48,"ax",%progbits
	.global Func_02002c48
	.thumb_func
Func_02002c48:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200accc
	sub sp, #68
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_0200acc2
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
	bl Func_0200015c
.L_0200acc2:
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200accc:
	.4byte Data_0300122c
	.section .text.x0200acd0,"ax",%progbits
	.global Func_02002cd0
	.thumb_func
Func_02002cd0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	mov r11, r1
	mov r9, r0
	bl Object_GetById
	adds r7, r0, #0
	mov r0, r11
	bl Object_GetById
	mov r10, r0
	movs r0, #78
	bl Func_02004f30
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl Func_02004e10
	movs r2, #10
	ldrsh r0, [r7, r2]
	movs r3, #14
	ldrsh r1, [r7, r3]
	movs r3, #18
	ldrsh r2, [r7, r3]
	lsls r1, r1, #16
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl Motion_CamBounds
	movs r0, #141
	bl Func_02004f30
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl Func_02004d10
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02004e48
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_02004e40
	movs r0, #60
	bl Func_02004e50
	adds r2, r7, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	str r3, [r7, #52]
	str r3, [r7, #48]
	ldr r1, .L_0200ada4
	mov r0, r9
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #194
	bl Func_02004f30
	movs r0, #45
	bl Battle_WaitMode0
	movs r1, #128
	mov r0, r9
	lsls r1, r1, #1
	bl Func_02004dc0
	ldr r3, .L_0200ad9c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200ada0
	subs r2, #2
	strh r3, [r2]
	movs r0, #0
	mov r8, r0
	b .L_0200ada8
	.2byte 0x0000
.L_0200ad9c:
	.4byte 0x00001008
.L_0200ada0:
	.4byte 0x00003f10
.L_0200ada4:
	.4byte Data_02005590
.L_0200ada8:
	movs r0, #168
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	lsls r0, r0, #2
	bl Func_02004ca8
	adds r5, r0, #0
	movs r0, #246
	bl Func_02004f30
	bl Random16Far
	movs r3, #31
	ldr r2, [r5, #8]
	ands r3, r0
	subs r3, #16
	lsls r3, r3, #16
	adds r2, r2, r3
	str r2, [r5, #8]
	bl Random16Far
	movs r3, #15
	ldr r2, [r5, #12]
	ands r3, r0
	subs r3, #8
	lsls r3, r3, #16
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r5, #48]
	movs r3, #204
	lsls r3, r3, #6
	str r2, [r5, #12]
	adds r3, #51
	adds r2, r5, #0
	adds r2, #85
	str r3, [r5, #52]
	movs r3, #0
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #10
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetSpritePriority
	adds r0, r5, #0
	movs r1, #7
	bl Func_02004c98
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	ldr r1, .L_0200ae68
	adds r0, r5, #0
	bl Func_02004ca0
	movs r6, #128
	ldr r3, .L_0200ae5c
	ldr r5, .L_0200ae60
	lsls r6, r6, #19
	adds r6, #82
	strh r3, [r6]
	movs r0, #2
	bl WaitFrames
	movs r0, #2
	strh r5, [r6]
	bl WaitFrames
	ldr r3, .L_0200ae64
	movs r0, #2
	strh r3, [r6]
	bl WaitFrames
	strh r5, [r6]
	movs r0, #2
	bl WaitFrames
	movs r2, #1
	add r8, r2
	mov r3, r8
	b .L_0200ae6c
.L_0200ae5c:
	.4byte 0x00001004
.L_0200ae60:
	.4byte 0x0000100a
.L_0200ae64:
	.4byte 0x00001010
.L_0200ae68:
	.4byte Data_02005544
.L_0200ae6c:
	cmp r3, #16
	bne .L_0200ada8
	ldr r3, .L_0200aeac
	movs r0, #30
	strh r3, [r6]
	bl WaitFrames
	movs r0, #0
	mov r8, r0
.L_0200ae7e:
	mov r0, r10
	ldr r3, [r0, #16]
	mov r2, r10
	movs r0, #168
	ldr r1, [r2, #8]
	lsls r0, r0, #2
	ldr r2, [r2, #12]
	bl Func_02004ca8
	adds r5, r0, #0
	movs r0, #195
	bl Func_02004f30
	bl Random16Far
	ldr r3, .L_0200aeb0
	movs r2, #128
	ands r0, r3
	lsls r2, r2, #8
	adds r6, r5, #0
	adds r0, r0, r2
	adds r6, #100
	b .L_0200aeb4
.L_0200aeac:
	.4byte 0x00001008
.L_0200aeb0:
	.4byte 0x00007fff
.L_0200aeb4:
	strh r0, [r6]
	bl Random16Far
	mov r3, r8
	movs r2, #1
	ands r2, r3
	movs r3, #3
	ands r3, r0
	lsls r2, r2, #1
	adds r3, #9
	subs r2, #1
	lsls r2, r3
	ldr r7, .L_0200af08
	adds r3, r5, #0
	adds r3, #102
	strh r2, [r3]
	subs r3, #17
	strb r7, [r3]
	movs r3, #244
	lsls r3, r3, #15
	str r3, [r5, #76]
	mov r2, r8
	movs r3, #16
	subs r3, r3, r2
	lsls r3, r3, #3
	adds r2, r5, #0
	adds r3, #15
	adds r2, #98
	mov r0, r10
	str r0, [r5, #104]
	movs r1, #10
	strb r3, [r2]
	adds r0, r5, #0
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetSpritePriority
	adds r0, r5, #0
	movs r1, #7
	b .L_0200af0c
.L_0200af08:
	.4byte 0x00000000
.L_0200af0c:
	bl Func_02004c98
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	adds r2, r5, #0
	movs r3, #0
	ldrsh r1, [r6, r3]
	ldr r0, [r5, #76]
	adds r2, #8
	bl Vector_AddPolarOffsetFar
	adds r0, r5, #0
	ldr r1, .L_0200b004
	bl Func_02004ca0
	mov r0, r8
	cmp r0, #3
	bne .L_0200af60
	movs r1, #128
	mov r0, r11
	lsls r1, r1, #1
	bl Func_02004dc0
	mov r3, r10
	adds r3, #85
	strb r7, [r3]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	mov r2, r10
	str r3, [r2, #52]
	str r3, [r2, #48]
	mov r0, r11
	ldr r1, .L_0200b008
	bl ObjectMotion_EnableActionAndSetCallback
.L_0200af60:
	movs r0, #8
	bl WaitFrames
	movs r3, #1
	add r8, r3
	mov r0, r8
	cmp r0, #16
	beq .L_0200af72
	b .L_0200ae7e
.L_0200af72:
	movs r0, #220
	bl Func_02004f30
	movs r0, #16
	bl WaitFrames
	movs r2, #2
	mov r8, r2
.L_0200af82:
	mov r3, r10
	ldr r1, [r3, #8]
	ldr r3, [r3, #12]
	mov r0, r8
	lsls r2, r0, #16
	adds r2, r2, r3
	ldr r3, .L_0200b00c
	mov r0, r10
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #168
	lsls r0, r0, #2
	bl Func_02004ca8
	adds r5, r0, #0
	bl Random16Far
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	adds r2, r5, #0
	movs r3, #128
	adds r2, #102
	lsls r3, r3, #4
	strh r3, [r2]
	adds r3, r5, #0
	mov r2, r8
	adds r3, #98
	strb r2, [r3]
	movs r2, #1
	adds r3, #1
	strb r2, [r3]
	ldr r1, .L_0200b000
	ldr r3, [r5, #8]
	movs r6, #0
	str r3, [r5, #68]
	ldr r3, [r5, #16]
	str r6, [r5, #76]
	str r3, [r5, #72]
	mov r3, r10
	str r3, [r5, #104]
	adds r3, r5, #0
	adds r3, #85
	strb r1, [r3]
	subs r3, #50
	strb r2, [r3]
	adds r0, r5, #0
	movs r1, #10
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #7
	bl Func_02004c98
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	b .L_0200b010
.L_0200b000:
	.4byte 0x00000000
.L_0200b004:
	.4byte Data_02005560
.L_0200b008:
	.4byte Data_020055b4
.L_0200b00c:
	.4byte 0xfff80000
.L_0200b010:
	adds r0, r5, #0
	ldr r1, .L_0200b0bc
	bl Func_02004ca0
	movs r0, #2
	add r8, r0
	mov r2, r8
	cmp r2, #32
	bne .L_0200af82
	movs r0, #220
	bl Func_02004f30
	movs r0, #50
	bl WaitFrames
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02004e40
	movs r0, #8
	bl Func_02004e50
	movs r0, #16
	bl WaitFrames
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Func_02004d10
	mov r0, r9
	movs r1, #0
	bl Func_02004dc0
	mov r0, r11
	movs r1, #0
	bl Func_02004dc0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #62
	bl Func_02004f30
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02004e40
	movs r0, #80
	bl Func_02004e50
	mov r0, r9
	ldr r1, .L_0200b0c0
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200b0c4
	mov r0, r11
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r3, .L_0200b0c8
	mov r0, r10
	str r3, [r0, #108]
	movs r0, #120
	bl WaitFrames
	mov r2, r10
	movs r0, #195
	str r6, [r2, #108]
	lsls r0, r0, #1
	bl Func_02004f30
	bl Func_02004ea0
	movs r0, #1
	bl WaitFrames
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b0bc:
	.4byte Data_02005570
.L_0200b0c0:
	.4byte Data_020055d8
.L_0200b0c4:
	.4byte Data_02005608
.L_0200b0c8:
	.4byte Func_02002c48
	.section .text.x0200b0cc,"ax",%progbits
	.global Func_020030cc
	.thumb_func
Func_020030cc:
	push {r5, r6, lr}
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	ldr r6, .L_0200b2cc
	movs r1, #1
	adds r0, r6, #0
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #250
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b0f6
	bl Func_02004d48
	b .L_0200b2c8
.L_0200b0f6:
	adds r0, r6, #0
	bl Func_02004dd0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	ldr r5, .L_0200b2d0
	adds r3, #1
	strh r3, [r2]
	movs r3, #133
	lsls r3, r3, #2
	movs r1, #179
	movs r2, #178
	adds r5, r5, r3
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #153
	movs r0, #5
	adds r1, #51
	bl ObjectMotion_SetSpeedParameters
	ldr r1, [r5]
	movs r0, #5
	bl Func_02004da8
	movs r0, #1
	bl Battle_WaitMode0
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #136
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #136
	lsls r1, r1, #1
	movs r2, #136
	movs r0, #5
	bl ObjectMotion_ResetAndSetPositionInMode2
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02004de0
	movs r1, #3
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #152
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	ldr r0, [r5]
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #5
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #137
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r6, #3
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #160
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004d10
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02004d10
	bl Func_02004d18
	movs r0, #8
	movs r1, #5
	bl Func_02002cd0
	movs r1, #154
	movs r0, #5
	bl UiText_DrawQuantityPairWithCue
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #250
	bl GameFlag_SetBit
	movs r2, #0
	movs r1, #5
	ldr r0, [r5]
	bl Object_LinkPair
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_0200b2d4
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200b2a8
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_0200b2a8:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #5
	bl Func_02004da0
	movs r0, #8
	bl Object_GetById
	movs r1, #9
	bl Object_SetActionCallback
	bl Func_02004d48
.L_0200b2c8:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b2cc:
	.4byte 0x000028cf
.L_0200b2d0:
	.4byte gPartyState
.L_0200b2d4:
	.4byte 0x00013333
	.section .text.x0200b2d8,"ax",%progbits
	.global Func_020032d8
	.thumb_func
Func_020032d8:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #250
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b300
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	ldr r0, .L_0200b340
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02004d48
	b .L_0200b33e
.L_0200b300:
	ldr r5, .L_0200b344
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #248
	movs r2, #107
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #248
	movs r2, #136
	bl ObjectMotion_SetPositionAndReset
	movs r1, #132
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #136
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_020030cc
.L_0200b33e:
	pop {r5, pc}
.L_0200b340:
	.4byte 0x000028cf
.L_0200b344:
	.4byte gPartyState
	.section .text.x0200b348,"ax",%progbits
	.global Func_02003348
	.thumb_func
Func_02003348:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #250
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b370
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	ldr r0, .L_0200b3a8
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02004d48
	b .L_0200b3a6
.L_0200b370:
	ldr r5, .L_0200b3ac
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #140
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #136
	bl ObjectMotion_SetPositionAndReset
	movs r1, #132
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #136
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_020030cc
.L_0200b3a6:
	pop {r5, pc}
.L_0200b3a8:
	.4byte 0x000028cf
.L_0200b3ac:
	.4byte gPartyState
	.section .text.x0200b3b0,"ax",%progbits
	.global Func_020033b0
	.thumb_func
Func_020033b0:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #250
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b3d8
	bl Func_02004d40
	movs r0, #0
	bl Func_02004e90
	ldr r0, .L_0200b410
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02004d48
	b .L_0200b40c
.L_0200b3d8:
	ldr r5, .L_0200b414
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #248
	movs r2, #136
	bl ObjectMotion_SetPositionAndReset
	movs r1, #132
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #136
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_020030cc
.L_0200b40c:
	pop {r5, pc}
	.2byte 0x0000
.L_0200b410:
	.4byte 0x000028cf
.L_0200b414:
	.4byte gPartyState
	.section .text.x0200b418,"ax",%progbits
	.global Func_02003418
	.thumb_func
Func_02003418:
	push {lr}
	ldr r3, .L_0200b478
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200b47c
	cmp r2, r3
	beq .L_0200b472
	ldr r3, .L_0200b480
	cmp r2, r3
	bne .L_0200b436
	ldr r0, .L_0200b484
	b .L_0200b474
.L_0200b436:
	ldr r3, .L_0200b488
	cmp r2, r3
	bne .L_0200b440
	ldr r0, .L_0200b48c
	b .L_0200b474
.L_0200b440:
	ldr r3, .L_0200b490
	cmp r2, r3
	bne .L_0200b44a
	ldr r0, .L_0200b494
	b .L_0200b474
.L_0200b44a:
	ldr r3, .L_0200b498
	cmp r2, r3
	bne .L_0200b454
	ldr r0, .L_0200b49c
	b .L_0200b474
.L_0200b454:
	ldr r3, .L_0200b4a0
	cmp r2, r3
	bne .L_0200b45e
	ldr r0, .L_0200b4a4
	b .L_0200b474
.L_0200b45e:
	ldr r3, .L_0200b4a8
	cmp r2, r3
	bne .L_0200b468
	ldr r0, .L_0200b4ac
	b .L_0200b474
.L_0200b468:
	ldr r3, .L_0200b4b0
	cmp r2, r3
	bne .L_0200b472
	ldr r0, .L_0200b4b4
	b .L_0200b474
.L_0200b472:
	ldr r0, .L_0200b4b8
.L_0200b474:
	pop {pc}
	.2byte 0x0000
.L_0200b478:
	.4byte gPartyState
.L_0200b47c:
	.4byte 0x000000e2
.L_0200b480:
	.4byte 0x000000e3
.L_0200b484:
	.4byte Data_02005fe4
.L_0200b488:
	.4byte 0x000000e4
.L_0200b48c:
	.4byte Data_020060bc
.L_0200b490:
	.4byte 0x000000e5
.L_0200b494:
	.4byte Data_020061c4
.L_0200b498:
	.4byte 0x000000e6
.L_0200b49c:
	.4byte Data_020063a4
.L_0200b4a0:
	.4byte 0x000000e7
.L_0200b4a4:
	.4byte Data_02006458
.L_0200b4a8:
	.4byte 0x000000e8
.L_0200b4ac:
	.4byte Data_0200659c
.L_0200b4b0:
	.4byte 0x000000e9
.L_0200b4b4:
	.4byte Data_02006714
.L_0200b4b8:
	.4byte Data_02005f60
	.section .text.x0200b4bc,"ax",%progbits
	.global Func_020034bc
	.thumb_func
Func_020034bc:
	push {r5, lr}
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #0
	movs r1, #15
	movs r2, #13
	movs r3, #0
	bl Func_02000694
	movs r5, #0
.L_0200b4d4:
	adds r0, r5, #0
	adds r0, #9
	movs r1, #2
	adds r5, #1
	bl Object_SetModeById
	cmp r5, #6
	bne .L_0200b4d4
	movs r0, #15
	movs r1, #4
	bl Object_SetModeById
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200b4f0,"ax",%progbits
	.global Func_020034f0
	.thumb_func
Func_020034f0:
	push {lr}
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_SetBit
	ldr r3, .L_0200b540
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200b544
	cmp r2, r3
	bne .L_0200b530
	movs r1, #24
	movs r2, #15
	movs r3, #0
	movs r0, #0
	bl Func_02000694
	movs r1, #9
	movs r0, #0
	movs r2, #16
	movs r3, #0
	bl Func_02000694
	movs r0, #11
	movs r1, #4
	bl Object_SetModeById
	b .L_0200b53c
.L_0200b530:
	movs r0, #0
	movs r1, #26
	movs r2, #14
	movs r3, #0
	bl Func_02000694
.L_0200b53c:
	pop {pc}
	.2byte 0x0000
.L_0200b540:
	.4byte gPartyState
.L_0200b544:
	.4byte 0x000000e4
	.section .text.x0200b548,"ax",%progbits
	.global Func_02003548
	.thumb_func
Func_02003548:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	ldr r3, .L_0200b618
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200b61c
	cmp r2, r3
	bne .L_0200b614
	ldr r6, .L_0200b620
	movs r5, #192
	lsls r5, r5, #2
	adds r5, #2
	adds r1, r6, #0
	movs r0, #15
	adds r2, r5, #0
	bl Func_02000800
	adds r1, r6, #0
	movs r0, #16
	adds r2, r5, #0
	bl Func_02000800
	ldr r6, .L_0200b624
	movs r0, #17
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_02000800
	adds r1, r6, #0
	movs r0, #18
	adds r2, r5, #0
	bl Func_02000800
	ldr r6, .L_0200b628
	movs r0, #19
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_02000800
	adds r1, r6, #0
	movs r0, #20
	adds r2, r5, #0
	bl Func_02000800
	ldr r6, .L_0200b62c
	movs r0, #21
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_02000800
	adds r2, r5, #0
	movs r0, #22
	adds r1, r6, #0
	bl Func_02000800
	movs r0, #16
	bl Object_GetById
	movs r2, #4
	ldrsh r3, [r0, r2]
	cmp r3, #0
	bne .L_0200b5f4
	movs r0, #16
	bl Object_GetById
	movs r5, #10
	strh r5, [r0, #4]
	movs r0, #18
	bl Object_GetById
	strh r5, [r0, #4]
	movs r0, #20
	bl Object_GetById
	strh r5, [r0, #4]
	movs r0, #22
	bl Object_GetById
	strh r5, [r0, #4]
.L_0200b5f4:
	movs r0, #23
	movs r1, #4
	bl Object_SetModeById
	movs r0, #24
	movs r1, #4
	bl Object_SetModeById
	movs r0, #25
	movs r1, #4
	bl Object_SetModeById
	movs r0, #26
	movs r1, #2
	bl Object_SetModeById
.L_0200b614:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b618:
	.4byte gPartyState
.L_0200b61c:
	.4byte 0x000000e5
.L_0200b620:
	.4byte Data_0200525c
.L_0200b624:
	.4byte Data_020052bc
.L_0200b628:
	.4byte Data_0200531c
.L_0200b62c:
	.4byte Data_0200537c
	.section .text.x0200b630,"ax",%progbits
	.global Func_02003630
	.thumb_func
Func_02003630:
	push {r5, lr}
	ldr r3, .L_0200b674
	adds r5, r0, #0
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #14
	ldrsh r3, [r5, r1]
	movs r1, #14
	ldrsh r2, [r0, r1]
	adds r1, r5, #0
	adds r1, #35
	cmp r2, r3
	bne .L_0200b65e
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #18
	ldrsh r3, [r5, r0]
	cmp r2, r3
	bge .L_0200b664
.L_0200b65e:
	movs r3, #1
	strb r3, [r1]
	b .L_0200b670
.L_0200b664:
	movs r3, #0
	strb r3, [r1]
	adds r0, r5, #0
	movs r1, #3
	bl Object_SetSpritePriority
.L_0200b670:
	pop {r5, pc}
	.2byte 0x0000
.L_0200b674:
	.4byte gPartyState
	.section .text.x0200b678,"ax",%progbits
	.global Func_02003678
	.thumb_func
Func_02003678:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	ldr r5, .L_0200b700
	str r2, [r3]
	subs r2, #36
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200b704
	sub sp, #8
	cmp r2, r3
	bne .L_0200b70c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	movs r5, #192
	orrs r3, r2
	lsls r5, r5, #2
	strb r3, [r0]
	ldr r1, .L_0200b708
	movs r0, #8
	adds r2, r5, #0
	bl Func_02000800
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b6d4
	bl .L_0200bec6
.L_0200b6d4:
	ldr r3, .L_0200b6f8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200b6fc
	subs r2, #2
	strh r3, [r2]
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #71
	movs r1, #64
	movs r2, #47
	movs r3, #98
	bl Func_02004cd8
	b .L_0200bec6
.L_0200b6f8:
	.4byte 0x00001000
.L_0200b6fc:
	.4byte 0x00003f44
.L_0200b700:
	.4byte gPartyState
.L_0200b704:
	.4byte 0x000000e2
.L_0200b708:
	.4byte Data_02005074
.L_0200b70c:
	ldr r3, .L_0200b8dc
	cmp r2, r3
	bne .L_0200b7ae
	movs r0, #0
	bl Func_02004e60
	movs r0, #15
	bl Object_GetById
	ldr r3, .L_0200b8e0
	movs r5, #0
	str r3, [r0, #108]
.L_0200b724:
	adds r0, r5, #0
	adds r0, #9
	bl Object_GetById
	adds r2, r0, #0
	movs r3, #0
	adds r2, #85
	adds r5, #1
	strb r3, [r2]
	str r3, [r0, #20]
	str r3, [r0, #12]
	cmp r5, #6
	bne .L_0200b724
	movs r0, #159
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b756
	movs r0, #15
	bl Object_GetById
	movs r3, #132
	lsls r3, r3, #17
	str r3, [r0, #16]
.L_0200b756:
	ldr r0, .L_0200b8e4
	bl Func_02004ed8
	ldr r0, .L_0200b8e8
	bl Func_02000880
	ldr r3, .L_0200b8ec
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #16
	bl Object_GetById
	adds r2, r0, #0
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r0, #20]
	str r3, [r0, #12]
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b7a8
	b .L_0200bb24
.L_0200b7a8:
	bl Func_020034bc
	b .L_0200bec6
.L_0200b7ae:
	ldr r3, .L_0200b8f0
	cmp r2, r3
	bne .L_0200b84c
	movs r0, #0
	bl Func_02004e60
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #241
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b7d4
	movs r0, #11
	bl Object_GetById
	movs r3, #166
	lsls r3, r3, #18
	str r3, [r0, #16]
.L_0200b7d4:
	ldr r0, .L_0200b8f4
	bl Func_02004ed8
	bl Func_02004ec0
	movs r1, #13
	movs r2, #14
	movs r0, #0
	bl Func_02004ec8
	ldr r0, .L_0200b8e8
	bl Func_02000880
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r2, r0, #0
	adds r2, #85
	movs r3, #2
	strb r3, [r2]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r0, #20]
	str r3, [r0, #12]
	adds r0, #35
	ldrb r2, [r0]
	movs r5, #192
	movs r3, #128
	lsls r5, r5, #2
	orrs r3, r2
	adds r5, #1
	strb r3, [r0]
	ldr r1, .L_0200b8f8
	movs r0, #10
	adds r2, r5, #0
	bl Func_02000800
	movs r0, #12
	ldr r1, .L_0200b8fc
	adds r2, r5, #0
	bl Func_02000800
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b84a
	b .L_0200bb24
.L_0200b84a:
	b .L_0200b8aa
.L_0200b84c:
	ldr r3, .L_0200b900
	cmp r2, r3
	beq .L_0200b854
	b .L_0200ba84
.L_0200b854:
	movs r0, #0
	bl Func_02004e60
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	ldr r0, .L_0200b904
	bl Func_02000880
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	movs r6, #0
	cmp r3, #7
	bgt .L_0200b910
	movs r5, #192
	lsls r5, r5, #2
	adds r5, #1
	ldr r1, .L_0200b908
	movs r0, #10
	adds r2, r5, #0
	bl Func_02000800
	movs r0, #12
	ldr r1, .L_0200b90c
	adds r2, r5, #0
	bl Func_02000800
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b8b0
.L_0200b8aa:
	bl Func_020034f0
	b .L_0200bec6
.L_0200b8b0:
	ldr r3, .L_0200b8d4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200b8d8
	subs r2, #2
	strh r3, [r2]
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #71
	movs r1, #64
	movs r2, #35
	movs r3, #82
	bl Func_02004cd8
	b .L_0200bec6
.L_0200b8d4:
	.4byte 0x00001000
.L_0200b8d8:
	.4byte 0x00003f44
.L_0200b8dc:
	.4byte 0x000000e3
.L_0200b8e0:
	.4byte Func_02003630
.L_0200b8e4:
	.4byte Data_020054fc
.L_0200b8e8:
	.4byte Data_02005500
.L_0200b8ec:
	.4byte gPartyState
.L_0200b8f0:
	.4byte 0x000000e4
.L_0200b8f4:
	.4byte Data_02005508
.L_0200b8f8:
	.4byte Data_020050d4
.L_0200b8fc:
	.4byte Data_02005134
.L_0200b900:
	.4byte 0x000000e5
.L_0200b904:
	.4byte Data_0200550c
.L_0200b908:
	.4byte Data_0200519c
.L_0200b90c:
	.4byte Data_020051fc
.L_0200b910:
	movs r0, #23
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #32
	orrs r3, r5
	strb r3, [r0]
	movs r0, #24
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #25
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #3
	orrs r3, r5
	strb r3, [r0]
	movs r0, #23
	bl ObjectMotion_SetActionVariant
	movs r0, #24
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #25
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #242
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b96e
	movs r0, #23
	bl Object_GetById
	movs r3, #172
	lsls r3, r3, #17
	str r3, [r0, #8]
.L_0200b96e:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #243
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b988
	movs r0, #24
	bl Object_GetById
	movs r3, #172
	lsls r3, r3, #17
	str r3, [r0, #8]
.L_0200b988:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #244
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b9a2
	movs r0, #25
	bl Object_GetById
	movs r3, #172
	lsls r3, r3, #17
	str r3, [r0, #8]
.L_0200b9a2:
	ldr r0, .L_0200bb40
	bl Func_02004ed8
	movs r0, #26
	bl Object_GetById
	adds r0, #85
	strb r6, [r0]
	movs r0, #26
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #26
	bl Object_GetById
	str r6, [r0, #20]
	str r6, [r5, #12]
	movs r0, #15
	bl Object_GetById
	movs r5, #3
	adds r0, #92
	strb r5, [r0]
	movs r0, #16
	bl Object_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #17
	bl Object_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #18
	bl Object_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #19
	bl Object_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #20
	bl Object_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #21
	bl Object_GetById
	adds r0, #92
	strb r5, [r0]
	movs r0, #22
	bl Object_GetById
	adds r0, #92
	ldr r6, .L_0200bb44
	strb r5, [r0]
	movs r5, #192
	lsls r5, r5, #2
	adds r5, #2
	adds r1, r6, #0
	movs r0, #15
	adds r2, r5, #0
	bl Func_02000800
	adds r1, r6, #0
	movs r0, #16
	adds r2, r5, #0
	bl Func_02000800
	ldr r6, .L_0200bb48
	movs r0, #17
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_02000800
	adds r1, r6, #0
	movs r0, #18
	adds r2, r5, #0
	bl Func_02000800
	ldr r6, .L_0200bb4c
	movs r0, #19
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_02000800
	adds r1, r6, #0
	movs r0, #20
	adds r2, r5, #0
	bl Func_02000800
	ldr r6, .L_0200bb50
	movs r0, #21
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_02000800
	movs r0, #22
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_02000800
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bb24
	bl Func_02003548
	b .L_0200bec6
.L_0200ba84:
	ldr r3, .L_0200bb54
	cmp r2, r3
	bne .L_0200bb58
	movs r0, #0
	bl Func_02004e60
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	movs r0, #10
	bl Func_02004f08
	movs r1, #1
	movs r0, #11
	bl Func_02004f08
	movs r0, #8
	bl Object_GetById
	movs r1, #2
	mov r8, r1
	adds r3, r0, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	movs r6, #160
	lsls r6, r6, #13
	str r6, [r0, #20]
	str r6, [r0, #12]
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #128
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #85
	mov r1, r8
	strb r1, [r3]
	str r6, [r0, #20]
	str r6, [r0, #12]
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bb24
	movs r1, #22
	movs r2, #26
	movs r3, #0
	movs r0, #0
	bl Func_02000694
	movs r1, #25
	movs r0, #0
	movs r2, #26
	movs r3, #0
	bl Func_02000694
	movs r0, #64
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	b .L_0200bec6
.L_0200bb24:
	ldr r3, .L_0200bb38
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200bb3c
	subs r2, #2
	strh r3, [r2]
	b .L_0200bec6
	.2byte 0x0000
.L_0200bb38:
	.4byte 0x00001000
.L_0200bb3c:
	.4byte 0x00003f44
.L_0200bb40:
	.4byte Data_02005520
.L_0200bb44:
	.4byte Data_0200525c
.L_0200bb48:
	.4byte Data_020052bc
.L_0200bb4c:
	.4byte Data_0200531c
.L_0200bb50:
	.4byte Data_0200537c
.L_0200bb54:
	.4byte 0x000000e6
.L_0200bb58:
	ldr r3, .L_0200bc7c
	cmp r2, r3
	beq .L_0200bb60
	b .L_0200bc90
.L_0200bb60:
	movs r0, #0
	bl Func_02004e60
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #245
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bbaa
	movs r3, #46
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #46
	movs r1, #74
	movs r2, #1
	bl Func_02004ce8
	movs r0, #9
	bl Object_GetById
	movs r3, #190
	lsls r3, r3, #18
	str r3, [r0, #8]
.L_0200bbaa:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #246
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bbc6
	movs r1, #230
	movs r2, #154
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02004da0
.L_0200bbc6:
	movs r5, #192
	lsls r5, r5, #2
	adds r5, #2
	ldr r0, .L_0200bc80
	bl Func_02004ed8
	ldr r1, .L_0200bc84
	movs r0, #8
	adds r2, r5, #0
	bl Func_02000800
	ldr r1, .L_0200bc88
	movs r0, #10
	adds r2, r5, #0
	bl Func_02000800
	movs r0, #11
	ldr r1, .L_0200bc8c
	adds r2, r5, #0
	bl Func_02000800
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bc4e
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #246
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bc18
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
.L_0200bc18:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #245
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bc34
	movs r3, #255
	lsls r3, r3, #8
	movs r0, #2
	movs r1, #46
	movs r2, #8
	bl Func_02000694
.L_0200bc34:
	movs r1, #10
	movs r2, #45
	movs r3, #0
	movs r0, #0
	bl Func_02000694
	movs r0, #0
	movs r1, #53
	movs r2, #40
	movs r3, #0
	bl Func_02000694
	b .L_0200bec6
.L_0200bc4e:
	ldr r3, .L_0200bc74
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200bc78
	subs r2, #2
	strh r3, [r2]
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #71
	movs r1, #64
	movs r2, #50
	movs r3, #102
	bl Func_02004cd8
	b .L_0200bec6
	.2byte 0x0000
.L_0200bc74:
	.4byte 0x00001000
.L_0200bc78:
	.4byte 0x00003f44
.L_0200bc7c:
	.4byte 0x000000e7
.L_0200bc80:
	.4byte Data_02005528
.L_0200bc84:
	.4byte Data_020053dc
.L_0200bc88:
	.4byte Data_0200543c
.L_0200bc8c:
	.4byte Data_0200549c
.L_0200bc90:
	ldr r3, .L_0200bed0
	cmp r2, r3
	beq .L_0200bc98
	b .L_0200bdd4
.L_0200bc98:
	movs r6, #0
.L_0200bc9a:
	adds r0, r6, #0
	adds r0, #8
	bl Object_GetById
	movs r1, #4
	adds r5, r0, #0
	ldr r7, .L_0200bed4
	bl Func_02004c98
	movs r1, #0
	adds r3, r5, #0
	mov r8, r1
	adds r3, #85
	mov r2, r8
	adds r6, #1
	strb r2, [r3]
	str r7, [r5, #20]
	str r7, [r5, #12]
	cmp r6, #3
	bne .L_0200bc9a
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #247
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bce6
	movs r0, #23
	movs r1, #4
	bl Object_SetModeById
	movs r1, #174
	movs r2, #162
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02004da0
.L_0200bce6:
	ldr r0, .L_0200bed8
	bl Func_02004ed8
	movs r0, #0
	bl Func_02004e60
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bd0a
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
.L_0200bd0a:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bd20
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
.L_0200bd20:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bd62
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bd62
	movs r0, #11
	bl Object_GetById
	mov r3, r8
	adds r0, #85
	strb r3, [r0]
	movs r0, #11
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #11
	bl Object_GetById
	movs r1, #4
	str r7, [r0, #20]
	str r7, [r5, #12]
	movs r0, #11
	bl Object_SetModeById
	b .L_0200bd70
.L_0200bd62:
	movs r3, #255
	lsls r3, r3, #8
	movs r0, #0
	movs r1, #38
	movs r2, #18
	bl Func_02000694
.L_0200bd70:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #249
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bd86
	movs r0, #12
	movs r1, #0
	bl Object_SetModeById
.L_0200bd86:
	movs r6, #0
.L_0200bd88:
	adds r0, r6, #0
	adds r0, #13
	movs r1, #2
	adds r6, #1
	bl Object_SetModeById
	cmp r6, #4
	bne .L_0200bd88
	movs r6, #0
.L_0200bd9a:
	adds r0, r6, #0
	adds r0, #12
	bl Object_GetById
	adds r6, #1
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	cmp r6, #11
	bne .L_0200bd9a
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #248
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bdbe
	b .L_0200bec6
.L_0200bdbe:
	movs r6, #0
.L_0200bdc0:
	adds r0, r6, #0
	adds r0, #12
	movs r1, #0
	movs r2, #0
	adds r6, #1
	bl Func_02004da0
	cmp r6, #9
	bne .L_0200bdc0
	b .L_0200bec6
.L_0200bdd4:
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200bedc
	cmp r2, r3
	bne .L_0200bec6
	bl Func_0200255c
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r6, #0
.L_0200bdf2:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #2
	adds r0, r6, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200be2a
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
	bl Func_02004cd8
	movs r3, #255
	movs r0, #0
	movs r1, #16
	adds r2, r5, #0
	lsls r3, r3, #8
	bl Func_02000694
.L_0200be2a:
	adds r6, #1
	cmp r6, #6
	bne .L_0200bdf2
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200be80
	movs r0, #9
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #9
	bl Object_GetById
	ldr r3, .L_0200bed4
	movs r1, #4
	str r3, [r0, #20]
	str r3, [r5, #12]
	movs r0, #9
	bl Object_SetModeById
	b .L_0200be98
.L_0200be80:
	movs r3, #255
	movs r1, #7
	movs r2, #21
	lsls r3, r3, #8
	movs r0, #0
	bl Func_02000694
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
.L_0200be98:
	movs r0, #8
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #8
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r0, #20]
	str r3, [r5, #12]
	movs r0, #8
	bl Object_GetById
	movs r1, #9
	bl Object_SetActionCallback
.L_0200bec6:
	movs r0, #0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200bed0:
	.4byte 0x000000e8
.L_0200bed4:
	.4byte 0xffe00000
.L_0200bed8:
	.4byte Data_0200552e
.L_0200bedc:
	.4byte 0x000000e9
	.section .text.x0200bee0,"ax",%progbits
	.global Func_02003ee0
	.thumb_func
Func_02003ee0:
	push {r5, r6, lr}
	ldr r2, .L_0200bf38
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r6, [r3, r1]
	movs r1, #241
	lsls r1, r1, #1
	movs r0, #10
	adds r3, r2, r1
	adds r0, #255
	movs r2, #0
	ldrsh r5, [r3, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bf34
	ldr r3, .L_0200bf3c
	cmp r6, r3
	bne .L_0200bf0e
	cmp r5, #8
	beq .L_0200bf18
.L_0200bf0e:
	ldr r3, .L_0200bf40
	cmp r6, r3
	bne .L_0200bf34
	cmp r5, #11
	bne .L_0200bf34
.L_0200bf18:
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_ClearBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
.L_0200bf34:
	movs r0, #0
	pop {r5, r6, pc}
.L_0200bf38:
	.4byte gPartyState
.L_0200bf3c:
	.4byte 0x000000e3
.L_0200bf40:
	.4byte 0x000000e8
	.section .text.x0200bf44,"ax",%progbits
	.global Func_02003f44
	.thumb_func
Func_02003f44:
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
	beq .L_0200c02a
.L_0200bf6a:
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
	ldr r2, .L_0200c060
	adds r3, r5, r2
	ldr r2, .L_0200c064
	asrs r3, r3, #2
	adds r6, r3, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bfb8
	ldr r0, [sp, #12]
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
	b .L_0200c018
.L_0200bfb8:
	adds r0, r7, #0
	bl Func_02004ed0
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r5, r5, r3
	str r5, [sp, #4]
	mov r2, r9
	ldrb r0, [r2]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	bl Func_02004d20
	mov r3, r9
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	mov r10, r0
	ldrb r0, [r3]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	ldr r0, [sp, #12]
	bl Func_02004df8
	ldr r2, [sp, #4]
	movs r3, #128
	asrs r5, r5, #19
	strb r3, [r2, #3]
	adds r5, #4
	mov r3, r9
	adds r2, r5, #0
	ldrb r0, [r3]
	mov r1, r10
	bl Func_02004ef0
	add r8, r6
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c018
	adds r0, r7, #0
	movs r1, #0
	bl Func_02004c98
.L_0200c018:
	movs r3, #4
	add r11, r3
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200bf6a
.L_0200c02a:
	ldr r3, .L_0200c068
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
	bge .L_0200c052
	str r0, [r5, #20]
	str r0, [r5, #12]
.L_0200c052:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c060:
	.4byte 0xfdff0000
.L_0200c064:
	.4byte Data_02024000
.L_0200c068:
	.4byte gPartyState
	.section .text.x0200c06c,"ax",%progbits
	.global Func_0200406c
	.thumb_func
Func_0200406c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #12
	adds r5, r0, #0
	bl Func_02004e38
	cmp r0, #0
	beq .L_0200c082
	b .L_0200c1f2
.L_0200c082:
	ldr r3, .L_0200c1fc
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
	bl Func_02004ef8
	mov r8, r0
	cmp r0, #0
	bne .L_0200c0ae
	b .L_0200c1f2
.L_0200c0ae:
	b .L_0200c1e4
.L_0200c0b0:
	ldrh r7, [r5]
	adds r0, r7, #0
	bl Object_GetById
	cmp r0, r8
	beq .L_0200c0c0
	adds r5, #4
	b .L_0200c1e4
.L_0200c0c0:
	ldrh r5, [r5, #2]
	bl Func_02004d40
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c12e
	movs r0, #125
	bl Func_02004f30
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	movs r1, #0
	mov r0, r8
	bl Func_02004c98
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #4
	bl WaitFrames
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #0
	bl Func_020046e8
	adds r0, r5, #0
	bl GameFlag_SetBit
	b .L_0200c1de
.L_0200c12e:
	adds r5, #1
	mov r10, r5
	mov r0, r10
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c1de
	adds r6, #85
	strb r0, [r6]
	movs r0, #185
	bl Func_02004f30
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02004d10
	movs r0, #0
	bl Func_020046e8
	movs r5, #2
	movs r0, #8
	mov r7, r8
	bl WaitFrames
	negs r5, r5
	mov r0, r8
	movs r1, #2
	adds r7, #34
	bl Func_02004c98
	ldrb r1, [r7]
	adds r0, r5, #0
	bl Func_020049c0
	movs r0, #1
	bl Func_020046e8
	movs r0, #16
	bl WaitFrames
	ldrb r1, [r7]
	adds r0, r5, #0
	bl Func_020049c0
	movs r0, #4
	bl WaitFrames
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02004d10
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
	bl Func_02004f30
	bl Func_0200481c
	movs r0, #20
	bl WaitFrames
	mov r0, r10
	bl GameFlag_SetBit
.L_0200c1de:
	bl Func_02004d48
	b .L_0200c1f2
.L_0200c1e4:
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_0200c1f2
	b .L_0200c0b0
.L_0200c1f2:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c1fc:
	.4byte gPartyState
	.section .text.x0200c200,"ax",%progbits
	.global Func_02004200
	.thumb_func
Func_02004200:
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
	beq .L_0200c2da
.L_0200c21e:
	ldrh r3, [r5]
	cmp r3, r6
	beq .L_0200c228
	adds r5, #4
	b .L_0200c2ce
.L_0200c228:
	ldrh r5, [r5, #2]
	bl Func_02004d40
	adds r3, r5, #1
	mov r8, r3
	mov r0, r8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c2c8
	movs r0, #185
	bl Func_02004f30
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02004d10
	movs r0, #0
	bl Func_020046e8
	movs r0, #8
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #2
	bl Func_02004c98
	adds r3, r7, #0
	adds r3, #34
	movs r0, #4
	ldrb r1, [r3]
	adds r2, r6, #0
	negs r0, r0
	bl Func_02004934
	movs r0, #1
	bl Func_020046e8
	movs r0, #16
	bl WaitFrames
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02004d10
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
	bl Func_02004f30
	bl Func_0200481c
	movs r0, #20
	bl WaitFrames
	adds r0, r5, #0
	bl GameFlag_SetBit
	mov r0, r8
	bl GameFlag_SetBit
.L_0200c2c8:
	bl Func_02004d48
	b .L_0200c2da
.L_0200c2ce:
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200c21e
.L_0200c2da:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x0200c2e0,"ax",%progbits
	.global Func_020042e0
	.thumb_func
Func_020042e0:
	push {r5, lr}
	adds r5, r0, #0
	bl Object_GetById
	movs r3, #3
	adds r0, #92
	strb r3, [r0]
	adds r0, r5, #0
	bl Func_02004df8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200c2f8,"ax",%progbits
	.global Func_020042f8
	.thumb_func
Func_020042f8:
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
	ldr r5, .L_0200c454
	str r3, [sp, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	mov r11, r0
	ldr r1, [r5]
	movs r0, #8
	bl Func_02004da8
	ldr r1, [r5]
	movs r0, #9
	bl Func_02004da8
	ldr r1, [r5]
	movs r0, #10
	bl Func_02004da8
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	bl Func_020042e0
	movs r0, #9
	bl Func_020042e0
	movs r0, #10
	bl Func_020042e0
	movs r1, #0
	movs r0, #9
	bl Object_SetModeById
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_02004da0
	movs r0, #1
	bl WaitFrames
	b .L_0200c432
.L_0200c37e:
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
	ldr r2, .L_0200c458
	adds r3, r6, r2
	ldr r2, .L_0200c45c
	asrs r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	adds r2, r5, #0
	adds r2, #92
	movs r3, #3
	strb r3, [r2]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c3d4
	mov r0, r9
	movs r1, #0
	movs r2, #0
	bl Func_02004da0
	b .L_0200c42e
.L_0200c3d4:
	adds r0, r5, #0
	bl Func_02004ed0
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r6, r6, r3
	str r6, [sp, #4]
	add r8, r10
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r7]
	bl Func_02004d20
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	mov r10, r0
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	mov r0, r9
	bl Func_02004df8
	ldr r6, [sp, #4]
	asrs r5, r5, #19
	movs r3, #128
	adds r5, #4
	adds r2, r5, #0
	strb r3, [r6, #3]
	ldrb r0, [r7]
	mov r1, r10
	bl Func_02004ef0
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c42e
	mov r0, r9
	movs r1, #9
	bl ObjectVisual_CopyAttributes
.L_0200c42e:
	movs r3, #4
	add r11, r3
.L_0200c432:
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200c37e
	movs r0, #10
	bl WaitFrames
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c454:
	.4byte gPartyState
.L_0200c458:
	.4byte 0xfdff0000
.L_0200c45c:
	.4byte Data_02024000
	.section .text.x0200c460,"ax",%progbits
	.global Func_02004460
	.thumb_func
Func_02004460:
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
	.section .text.x0200c47c,"ax",%progbits
	.global Func_0200447c
	.thumb_func
Func_0200447c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #12
	adds r6, r0, #0
	bl Func_02004e38
	cmp r0, #0
	beq .L_0200c494
	b .L_0200c656
.L_0200c494:
	ldr r3, .L_0200c664
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
	bl Func_02004ef8
	mov r10, r0
	cmp r0, #0
	bne .L_0200c4c8
	b .L_0200c656
.L_0200c4c8:
	b .L_0200c648
.L_0200c4ca:
	ldrh r3, [r6]
	mov r8, r3
	mov r0, r8
	bl Object_GetById
	cmp r0, r10
	beq .L_0200c4dc
	adds r6, #4
	b .L_0200c648
.L_0200c4dc:
	ldrh r6, [r6, #2]
	bl Func_02004d40
	adds r0, r6, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c576
	adds r0, r7, #0
	movs r1, #1
	bl Func_02004c98
	mov r1, r10
	adds r0, r7, #0
	bl Func_02004460
	movs r0, #1
	bl WaitFrames
	movs r0, #125
	bl Func_02004f30
	movs r0, #8
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #0
	bl Func_02004c98
	movs r1, #9
	mov r0, r8
	bl ObjectVisual_CopyAttributes
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	movs r0, #8
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #4
	bl WaitFrames
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #0
	bl Func_020046e8
	mov r0, r10
	adds r1, r7, #0
	bl Func_02004460
	movs r0, #1
	bl WaitFrames
	adds r0, r6, #0
	bl GameFlag_SetBit
	b .L_0200c642
.L_0200c576:
	adds r6, #1
	mov r9, r6
	mov r0, r9
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200c642
	adds r0, r7, #0
	movs r1, #0
	bl Func_02004c98
	mov r1, r10
	adds r0, r7, #0
	bl Func_02004460
	adds r5, #85
	movs r0, #1
	bl WaitFrames
	strb r6, [r5]
	movs r0, #185
	bl Func_02004f30
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02004d10
	movs r0, #0
	bl Func_020046e8
	mov r8, r5
	movs r0, #8
	movs r6, #2
	mov r5, r10
	bl WaitFrames
	negs r6, r6
	adds r0, r7, #0
	movs r1, #2
	adds r5, #34
	bl Func_02004c98
	ldrb r1, [r5]
	adds r0, r6, #0
	bl Func_020049c0
	movs r0, #1
	bl Func_020046e8
	movs r0, #16
	bl WaitFrames
	ldrb r1, [r5]
	adds r0, r6, #0
	bl Func_020049c0
	movs r0, #4
	bl WaitFrames
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02004d10
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
	bl Func_02004f30
	bl Func_0200481c
	movs r0, #20
	bl WaitFrames
	mov r0, r9
	bl GameFlag_SetBit
.L_0200c642:
	bl Func_02004d48
	b .L_0200c656
.L_0200c648:
	movs r2, #255
	ldrh r3, [r6]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_0200c656
	b .L_0200c4ca
.L_0200c656:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c664:
	.4byte gPartyState
	.section .text.x0200c668,"ax",%progbits
	.global Func_02004668
	.thumb_func
Func_02004668:
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
	bge .L_0200c698
	adds r3, #15
.L_0200c698:
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
	ldr r3, .L_0200c6e4
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
.L_0200c6e4:
	.4byte gPartyState
	.section .text.x0200c6e8,"ax",%progbits
	.global Func_020046e8
	.thumb_func
Func_020046e8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0200c7f4
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
.L_0200c70a:
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
	ldr r0, .L_0200c7f8
	adds r1, r1, r3
	ldr r3, .L_0200c7fc
	adds r1, r1, r0
	movs r0, #30
	adds r2, r2, r3
	adds r0, #255
	ldr r3, [r7, #16]
	bl Func_02004ca8
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200c7de
	mov r1, r9
	ldr r0, [r6, #80]
	bl Func_02004e98
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
	bl Func_02004c98
	adds r0, r6, #0
	ldr r1, .L_0200c800
	bl Func_02004ca0
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
	beq .L_0200c7a8
	mov r3, r10
	lsls r5, r3, #13
	adds r0, r5, #0
	bl Math_Cosine
	ldr r3, .L_0200c804
	ldr r1, .L_0200c808
	mov lr, r3
	.2byte 0xf800
	str r0, [r6, #68]
	adds r0, r5, #0
	bl Math_Sine
	b .L_0200c7ac
.L_0200c7a8:
	mov r0, r8
	str r0, [r6, #68]
.L_0200c7ac:
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
	ldr r3, .L_0200c80c
	lsls r0, r0, #9
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r6, #0
	adds r3, #100
	strh r0, [r3]
	ldr r3, .L_0200c810
	str r3, [r6, #48]
	ldr r3, .L_0200c814
	str r3, [r6, #52]
	ldr r3, .L_0200c818
	str r3, [r6, #108]
.L_0200c7de:
	movs r0, #1
	add r10, r0
	mov r2, r10
	cmp r2, #7
	bls .L_0200c70a
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c7f4:
	.4byte gPartyState
.L_0200c7f8:
	.4byte 0xfff80000
.L_0200c7fc:
	.4byte 0xfffe0000
.L_0200c800:
	.4byte Data_02005638
.L_0200c804:
	.4byte IwramMulQ16
.L_0200c808:
	.4byte 0x00013333
.L_0200c80c:
	.4byte 0xffffff00
.L_0200c810:
	.4byte 0xfffff800
.L_0200c814:
	.4byte 0xfffffa00
.L_0200c818:
	.4byte Func_02004668
	.section .text.x0200c81c,"ax",%progbits
	.global Func_0200481c
	.thumb_func
Func_0200481c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0200c91c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200c910
	movs r3, #0
	mov r9, r3
	mov r10, r3
.L_0200c840:
	movs r0, #30
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, #255
	bl Func_02004ca8
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200c906
	mov r1, r9
	ldr r0, [r7, #80]
	bl Func_02004e98
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
	bl Func_02004c98
	ldr r1, .L_0200c920
	adds r0, r7, #0
	bl Func_02004ca0
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
	ldr r3, .L_0200c924
	adds r2, r2, r3
	str r2, [r7, #68]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r7, #76]
	adds r3, r3, r0
	ldr r4, .L_0200c928
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r2, r2, r3
	adds r2, r2, r4
	str r2, [r7, #76]
	bl Random16Far
	ldr r2, .L_0200c92c
	lsls r0, r0, #12
	lsrs r0, r0, #16
	adds r3, r7, #0
	adds r0, r0, r2
	adds r3, #100
	strh r0, [r3]
	mov r3, r8
	str r3, [r7, #48]
	str r3, [r7, #52]
	ldr r3, .L_0200c930
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
.L_0200c906:
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200c840
.L_0200c910:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c91c:
	.4byte gPartyState
.L_0200c920:
	.4byte Data_02005668
.L_0200c924:
	.4byte 0xffffa000
.L_0200c928:
	.4byte 0xffffd000
.L_0200c92c:
	.4byte 0xfffff800
.L_0200c930:
	.4byte Func_02004668
	.section .text.x0200c934,"ax",%progbits
	.global Func_02004934
	.thumb_func
Func_02004934:
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
	ldr r2, .L_0200c9b4
	adds r7, r0, #0
	ldr r1, .L_0200c9b8
	adds r3, r3, r2
	adds r5, r7, #0
	asrs r3, r3, #2
	adds r5, #34
	adds r6, r3, r1
	ldr r2, [r7, #16]
	ldr r1, [r7, #8]
	ldrb r0, [r5]
	bl Func_02004d20
	ldr r2, [r7, #16]
	mov r8, r0
	ldr r1, [r7, #8]
	ldrb r0, [r5]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #8]
	asrs r2, r0, #19
	add r2, r10
	cmp r3, #0
	bge .L_0200c98e
	ldr r1, .L_0200c9bc
	adds r3, r3, r1
.L_0200c98e:
	ldr r0, [r7, #16]
	asrs r1, r3, #20
	cmp r0, #0
	bge .L_0200c99a
	ldr r3, .L_0200c9bc
	adds r0, r0, r3
.L_0200c99a:
	asrs r3, r0, #20
	lsls r3, r3, #7
	adds r3, r1, r3
	ldrb r0, [r5]
	mov r1, r8
	adds r6, r6, r3
	bl Func_02004ef0
	strb r0, [r6]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c9b4:
	.4byte 0xfdff0000
.L_0200c9b8:
	.4byte Data_02024000
.L_0200c9bc:
	.4byte 0x000fffff
	.section .text.x0200c9c0,"ax",%progbits
	.global Func_020049c0
	.thumb_func
Func_020049c0:
	push {lr}
	ldr r3, .L_0200c9d4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r2, [r3]
	bl Func_02004934
	pop {pc}
	.2byte 0x0000
.L_0200c9d4:
	.4byte gPartyState
	.section .text.x0200c9d8,"ax",%progbits
	.global Func_020049d8
	.thumb_func
Func_020049d8:
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
.L_0200c9f6:
	ldr r3, [r5, #24]
	cmp r3, #19
	bhi .L_0200ca44
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
	ldr r3, .L_0200ca38
	ldr r2, .L_0200ca3c
	ands r1, r3
	ldrh r3, [r6, #8]
	adds r0, r6, #0
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #8]
	adds r1, r5, #0
	bl Func_02004f18
	adds r0, r5, #0
	movs r1, #63
	ldr r2, .L_0200ca40
	bl Func_02004f20
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	b .L_0200ca44
.L_0200ca38:
	.4byte 0x000003ff
.L_0200ca3c:
	.4byte 0xfffffc00
.L_0200ca40:
	.4byte 0xffff8000
.L_0200ca44:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r2, r8
	adds r6, #40
	adds r5, #28
	cmp r2, #0
	bge .L_0200c9f6
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200ca5c,"ax",%progbits
	.global Func_02004a5c
	.thumb_func
Func_02004a5c:
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
	ble .L_0200cb08
	adds r7, r2, #0
.L_0200ca84:
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
	bne .L_0200ca84
.L_0200cb08:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200cb18,"ax",%progbits
	.global Func_02004b18
	.thumb_func
Func_02004b18:
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
	ldr r0, .L_0200cbcc
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Func_02004c58
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
.L_0200cb70:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	bl Func_02004f10
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
	bge .L_0200cb70
	movs r1, #176
	lsls r1, r1, #5
	adds r2, r6, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200cbd0
	bl Scheduler_AddOrUpdateCallback
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200cbcc:
	.4byte 0x000001f0
.L_0200cbd0:
	.4byte Func_020049d8
	.section .text.x0200cbd4,"ax",%progbits
	.global Func_02004bd4
	.thumb_func
Func_02004bd4:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r0, .L_0200cbfc
	ldr r5, [r3]
	bl Scheduler_RemoveCallbackFar
	movs r3, #176
	lsls r3, r3, #5
	adds r3, #4
	adds r5, r5, r3
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl Resource_ResetEntry
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	pop {r5, pc}
.L_0200cbfc:
	.4byte Func_020049d8
	.section .rodata.x0200cf38,"a",%progbits
.L_0200cf38:
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
.L_0200cf74:
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
.L_0200cfb0:
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
	.global Data_02004fec
Data_02004fec:
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
	.global Data_0200502c
Data_0200502c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000012
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02005050
Data_02005050:
	.4byte 0x00000027
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02005074
Data_02005074:
	.4byte 0x00000004
	.4byte 0x03080000
	.4byte 0x00100000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02e80000
	.4byte 0x00100000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02e80000
	.4byte 0x00100000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03080000
	.4byte 0x00100000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_020050d4
Data_020050d4:
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00100000
	.4byte 0x02380000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00100000
	.4byte 0x02480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00100000
	.4byte 0x02480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00100000
	.4byte 0x02380000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02005134
Data_02005134:
	.4byte 0x00000004
	.4byte 0x02580000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02d80000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02d80000
	.4byte 0x00100000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02f80000
	.4byte 0x00100000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02f80000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_0200519c
Data_0200519c:
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00100000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00100000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00100000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00100000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_020051fc
Data_020051fc:
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00100000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01a80000
	.4byte 0x00100000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01a80000
	.4byte 0x00100000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00100000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_0200525c
Data_0200525c:
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00100000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00100000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_020052bc
Data_020052bc:
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00100000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00100000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_0200531c
Data_0200531c:
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00100000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01f80000
	.4byte 0x00100000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01f80000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_0200537c
Data_0200537c:
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02580000
	.4byte 0x00100000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02580000
	.4byte 0x00100000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00100000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_020053dc
Data_020053dc:
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00100000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00100000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00100000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00100000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_0200543c
Data_0200543c:
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00100000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00100000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00100000
	.4byte 0x02c80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00100000
	.4byte 0x02c80000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_0200549c
Data_0200549c:
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00100000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00100000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00100000
	.4byte 0x02c80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00100000
	.4byte 0x02c80000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_020054fc
Data_020054fc:
	.4byte 0xffff000f
	.global Data_02005500
Data_02005500:
	.4byte 0x00320008
	.4byte 0xffffffff
	.global Data_02005508
Data_02005508:
	.4byte 0xffff000b
	.global Data_0200550c
Data_0200550c:
	.4byte 0x00320008
	.4byte 0x000dffff
	.4byte 0xffff0033
	.4byte 0x0034000e
	.4byte 0xffffffff
	.global Data_02005520
Data_02005520:
	.4byte 0x00180017
	.4byte 0xffff0019
	.global Data_02005528
Data_02005528:
	.4byte 0x000c0009
	.2byte 0xffff
	.global Data_0200552e
Data_0200552e:
	.2byte 0x0017
	.4byte 0x0000ffff
	.global Data_02005534
Data_02005534:
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02005544
Data_02005544:
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02005560
Data_02005560:
	.4byte 0x0000002e
	.4byte Func_02002a6c
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02005570
Data_02005570:
	.4byte 0x0000002e
	.4byte Func_02002b2c
	.4byte 0x0000002e
	.4byte Func_02002bd0
	.4byte 0x0000002e
	.4byte Func_02002c1c
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02005590
Data_02005590:
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000004
	.4byte 0x00000011
	.global Data_020055b4
Data_020055b4:
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000004
	.4byte 0x00000011
	.global Data_020055d8
Data_020055d8:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0xffe80000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02005608
Data_02005608:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0xfff00000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000003
	.4byte 0x00000011
	.global Data_02005638
Data_02005638:
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
	.global Data_02005668
Data_02005668:
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
	.global Data_02005698
Data_02005698:
	.4byte .L_0200cf38
	.4byte .L_0200cf74
	.4byte .L_0200cfb0
	.global gSceneEntrances
gSceneEntrances:
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
	.global gSceneExits
gSceneExits:
	.4byte 0x000000e2
	.4byte 0x001040e2
	.4byte 0x002010e3
	.4byte 0x003050e3
	.4byte 0x004010e2
	.4byte 0x005020e3
	.4byte 0x006030e3
	.4byte 0x007040e3
	.4byte 0x008010e4
	.4byte 0x000000e3
	.4byte 0x001020e2
	.4byte 0x002050e2
	.4byte 0x003060e2
	.4byte 0x004070e2
	.4byte 0x005030e2
	.4byte 0x006070e3
	.4byte 0x007060e3
	.4byte 0x008030e1
	.4byte 0x000000e4
	.4byte 0x001080e2
	.4byte 0x002060e4
	.4byte 0x003010e5
	.4byte 0x004020e5
	.4byte 0x005030e5
	.4byte 0x006020e4
	.4byte 0x007040e5
	.4byte 0x008090e4
	.4byte 0x009080e4
	.4byte 0x00a050e5
	.4byte 0x00b060e5
	.4byte 0x000000e5
	.4byte 0x001030e4
	.4byte 0x002040e4
	.4byte 0x003050e4
	.4byte 0x004070e4
	.4byte 0x0050a0e4
	.4byte 0x0060b0e4
	.4byte 0x007070e7
	.4byte 0x008070e6
	.4byte 0x009080e6
	.4byte 0x00a010e6
	.4byte 0x00b010e7
	.4byte 0x00c020e7
	.4byte 0x00d050e7
	.4byte 0x00e060e7
	.4byte 0x000000e6
	.4byte 0x0010a0e5
	.4byte 0x002030e7
	.4byte 0x003090e6
	.4byte 0x0040a0e6
	.4byte 0x0050b0e7
	.4byte 0x006040e7
	.4byte 0x007080e5
	.4byte 0x008090e5
	.4byte 0x009030e6
	.4byte 0x00a040e6
	.4byte 0x000000e7
	.4byte 0x0010b0e5
	.4byte 0x0020c0e5
	.4byte 0x003020e6
	.4byte 0x004060e6
	.4byte 0x0050d0e5
	.4byte 0x0060e0e5
	.4byte 0x007070e5
	.4byte 0x0080e0e7
	.4byte 0x009100e7
	.4byte 0x00a110e7
	.4byte 0x00b050e6
	.4byte 0x00c120e7
	.4byte 0x00d130e7
	.4byte 0x00e080e7
	.4byte 0x00f0a0e8
	.4byte 0x010090e7
	.4byte 0x0110a0e7
	.4byte 0x0120c0e7
	.4byte 0x0130d0e7
	.4byte 0x000000e8
	.4byte 0x001010e9
	.4byte 0x002050e8
	.4byte 0x003060e8
	.4byte 0x004070e8
	.4byte 0x005020e8
	.4byte 0x006030e8
	.4byte 0x007040e8
	.4byte 0x008090e8
	.4byte 0x009080e8
	.4byte 0x00a0f0e7
	.4byte 0x00b020df
	.4byte 0x000000e9
	.4byte 0x001010e8
	.4byte 0x000001ff
	.global Data_02005840
Data_02005840:
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005870
Data_02005870:
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff014e
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff014e
	.4byte 0x00000007
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff014e
	.4byte 0x00000007
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff014e
	.4byte 0x00000007
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff014e
	.4byte 0x00000007
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff014e
	.4byte 0x00000007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0x006b00f5
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005978
Data_02005978:
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0x006800f5
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005a50
Data_02005a50:
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0x03500194
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0x03510194
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02028000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x0000000e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x0000000f
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000012
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff014e
	.4byte 0x00000007
	.4byte 0x00000013
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005c30
Data_02005c30:
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005ca8
Data_02005ca8:
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff013f
	.4byte 0x00000007
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005d38
Data_02005d38:
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0145
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0145
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff0145
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff0145
	.4byte 0x00000001
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff016c
	.4byte 0x00000001
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff016c
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff016c
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff016c
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0302c000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x0000000e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x0000000f
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005ed0
Data_02005ed0:
	.4byte 0xffff0143
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0138
	.4byte 0x00000007
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0202c000
	.4byte 0xffff0005
	.4byte 0x00000001
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
	.global Data_02005f60
Data_02005f60:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000202
	.4byte 0xffff0046
	.4byte Func_020011fc
	.4byte 0x00000202
	.4byte 0xffff004e
	.4byte Func_020011fc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005fe4
Data_02005fe4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte Func_02001828
	.4byte 0x0000c400
	.4byte 0xffff0011
	.4byte Func_020005a0
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_0200099c
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000a30
	.4byte 0x10008c15
	.4byte 0x09f0000f
	.4byte Func_0200099c
	.4byte 0x00008c15
	.4byte 0x09f0000f
	.4byte Func_02000a30
	.4byte 0x80009705
	.4byte 0xffff0032
	.4byte Func_02000e14
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte Func_02000e20
	.4byte 0x00009705
	.4byte 0xffff0032
	.4byte Func_02001148
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020060bc
Data_020060bc:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte Func_02001828
	.4byte 0x00000202
	.4byte 0xffff0046
	.4byte Func_020011fc
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_02000594
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_0200099c
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000a30
	.4byte 0x10008c15
	.4byte 0x09f1000b
	.4byte Func_0200099c
	.4byte 0x00008c15
	.4byte 0x09f1000b
	.4byte Func_02000a30
	.4byte 0x80009705
	.4byte 0xffff0032
	.4byte Func_02000e14
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte Func_02000e20
	.4byte 0x00009705
	.4byte 0xffff0032
	.4byte Func_02001148
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020061c4
Data_020061c4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000202
	.4byte 0xffff0046
	.4byte Func_020011fc
	.4byte 0x00000202
	.4byte 0x13010047
	.4byte Func_020011fc
	.4byte 0x00000202
	.4byte 0xffff0048
	.4byte Func_020011fc
	.4byte 0x0000c602
	.4byte 0xffff0049
	.4byte Func_020011fc
	.4byte 0x00008602
	.4byte 0xffff004b
	.4byte Func_020011fc
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_0200099c
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000a30
	.4byte 0x10008c15
	.4byte 0x09f20017
	.4byte Func_0200099c
	.4byte 0x00008c15
	.4byte 0x09f20017
	.4byte Func_02000a30
	.4byte 0x10008c15
	.4byte 0x09f30018
	.4byte Func_0200099c
	.4byte 0x00008c15
	.4byte 0x09f30018
	.4byte Func_02000a30
	.4byte 0x10008c15
	.4byte 0x09f40019
	.4byte Func_0200099c
	.4byte 0x00008c15
	.4byte 0x09f40019
	.4byte Func_02000a30
	.4byte 0x50009705
	.4byte 0x03500047
	.4byte Func_02001ca0
	.4byte 0x50009705
	.4byte 0x03510048
	.4byte Func_02001cd0
	.4byte 0x80009705
	.4byte 0xffff0032
	.4byte Func_02000e14
	.4byte 0x50009705
	.4byte 0x02f00032
	.4byte Func_02000e20
	.4byte 0x50009705
	.4byte 0xffff0032
	.4byte Func_02001ca0
	.4byte 0x00009705
	.4byte 0xffff0032
	.4byte Func_02001148
	.4byte 0x80009705
	.4byte 0xffff0033
	.4byte Func_02000e14
	.4byte 0x50009705
	.4byte 0xffff0033
	.4byte Func_02000e20
	.4byte 0x00009705
	.4byte 0xffff0033
	.4byte Func_02001148
	.4byte 0x80009705
	.4byte 0xffff0034
	.4byte Func_02000e14
	.4byte 0x50009705
	.4byte 0xffff0034
	.4byte Func_02000e20
	.4byte 0x00009705
	.4byte 0xffff0034
	.4byte Func_02001148
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020063a4
Data_020063a4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte Func_02001828
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte Func_02001828
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x00004e15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006458
Data_02006458:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000031
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000001
	.4byte 0xffff0013
	.4byte 0x00000013
	.4byte 0x00000202
	.4byte 0x13020046
	.4byte Func_020011fc
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_0200099c
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000a30
	.4byte 0x10008c15
	.4byte 0x09f50009
	.4byte Func_0200099c
	.4byte 0x00008c15
	.4byte 0x09f50009
	.4byte Func_02000a30
	.4byte 0x10008c15
	.4byte 0x09f6000c
	.4byte Func_0200099c
	.4byte 0x00008c15
	.4byte 0x09f6000c
	.4byte Func_02000a30
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200659c
Data_0200659c:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_020023e8
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000002
	.4byte 0x09f8001e
	.4byte Func_02001d64
	.4byte 0x00000003
	.4byte 0x09f9001f
	.4byte Func_02002364
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_0200099c
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000a30
	.4byte 0x10008c15
	.4byte 0x09f70017
	.4byte Func_0200099c
	.4byte 0x00008c15
	.4byte 0x09f70017
	.4byte Func_02000a30
	.4byte 0x10009a15
	.4byte 0xffff0011
	.4byte Func_02002128
	.4byte 0x10009a15
	.4byte 0xffff0012
	.4byte Func_02002128
	.4byte 0x10009a15
	.4byte 0xffff0013
	.4byte Func_02002128
	.4byte 0x10009a15
	.4byte 0xffff0014
	.4byte Func_02002128
	.4byte 0x10009a15
	.4byte 0xffff0015
	.4byte 0x00000000
	.4byte 0x10009a15
	.4byte 0xffff0016
	.4byte Func_0200217c
	.4byte 0x20009a15
	.4byte 0x03040016
	.4byte Func_020021c0
	.4byte 0x20009a15
	.4byte 0x03030015
	.4byte Func_020021c0
	.4byte 0x00009a15
	.4byte 0xffff0011
	.4byte Func_02002318
	.4byte 0x00009a15
	.4byte 0xffff0012
	.4byte Func_02002318
	.4byte 0x00009a15
	.4byte 0xffff0013
	.4byte Func_02002318
	.4byte 0x00009a15
	.4byte 0xffff0014
	.4byte Func_02002318
	.4byte 0x00009a15
	.4byte 0xffff0016
	.4byte Func_02002318
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006710
Data_02006710:
	.4byte 0xffffffff
	.global Data_02006714
Data_02006714:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte Func_0200257c
	.4byte 0x00000002
	.4byte 0xffff0051
	.4byte Func_02002610
	.4byte 0x0000c403
	.4byte 0xffff001e
	.4byte Func_020030cc
	.4byte 0x00004403
	.4byte 0xffff001e
	.4byte Func_020032d8
	.4byte 0x00008403
	.4byte 0xffff001e
	.4byte Func_02003348
	.4byte 0x00000403
	.4byte 0xffff001e
	.4byte Func_020033b0
	.4byte 0x00000006
	.4byte 0xffff00ca
	.4byte Func_020027d4
	.4byte 0x00000006
	.4byte 0xffff00cb
	.4byte Func_020027d4
	.4byte 0x00000006
	.4byte 0xffff00cc
	.4byte Func_020027d4
	.4byte 0x00000006
	.4byte 0xffff00cd
	.4byte Func_020027d4
	.4byte 0x00000006
	.4byte 0xffff00ce
	.4byte Func_020027d4
	.4byte 0x10009a15
	.4byte 0xffff000a
	.4byte Func_02002868
	.4byte 0x10009a15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0x20009a15
	.4byte 0x0305000b
	.4byte Func_020028ac
	.4byte 0x00009a15
	.4byte 0xffff000a
	.4byte Func_020029dc
	.4byte 0x00009a15
	.4byte 0xffff000b
	.4byte Func_020029dc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
