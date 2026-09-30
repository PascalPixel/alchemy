.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	sub sp, #4
	cmp r3, r2
	beq .L_0200809c
	adds r7, r0, #0
.L_0200804e:
	ldrh r3, [r7]
	mov r8, r3
	mov r0, r8
	bl Object_GetById
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #89
	movs r2, #2
	ldrsh r6, [r7, r2]
	ldrb r2, [r1]
	movs r3, #4
	ldrsh r4, [r7, r3]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r1, #0
	str r4, [sp, #0]
	bl ObjectDispatch_SetSingleChildField26
	ldr r4, [sp, #0]
	lsls r6, r6, #16
	lsls r4, r4, #16
	lsrs r4, r4, #16
	lsrs r6, r6, #16
	adds r5, #34
	ldrb r3, [r5]
	adds r2, r4, #0
	mov r0, r8
	adds r1, r6, #0
	adds r7, #6
	bl Func_02000128
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200804e
.L_0200809c:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x020080a4,"ax",%progbits
	.global Func_020000a4
	.thumb_func
Func_020000a4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r5, r0, #0
	mov r9, r1
	mov r10, r2
	movs r1, #255
	ldr r2, [r3]
	b .L_02008110
.L_020080c0:
	ldrh r3, [r5]
	movs r1, #26
	ldrsh r7, [r2, r1]
	cmp r7, r3
	bne .L_0200810c
	adds r0, r7, #0
	bl Object_GetById
	adds r5, #2
	ldrh r2, [r5]
	mov r3, r10
	adds r6, r0, #0
	mov r8, r2
	ldrh r5, [r5, #2]
	cmp r3, #7
	bgt .L_020080e8
	ldr r3, [r6, #28]
	ldr r1, .L_02008124
	adds r3, r3, r1
	str r3, [r6, #28]
.L_020080e8:
	mov r2, r9
	cmp r2, #1
	bne .L_0200811a
	adds r0, r5, #0
	bl GameFlag_SetBit
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	adds r0, r7, #0
	mov r1, r8
	adds r2, r5, #0
	bl Func_02000128
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	b .L_0200811a
.L_0200810c:
	adds r5, #6
	movs r1, #255
.L_02008110:
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	bne .L_020080c0
.L_0200811a:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008124:
	.4byte 0xffffe100
	.section .text.x02008128,"ax",%progbits
	.global Func_02000128
	.thumb_func
Func_02000128:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r3, #0
	mov r8, r2
	adds r6, r1, #0
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
	ldr r5, [r2, r3]
	adds r7, r0, #0
	bl Func_02005f64
	lsls r0, r0, #2
	adds r5, r5, r0
	mov r0, r8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200819c
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #0
	strb r3, [r5, #2]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	cmp r6, #1
	beq .L_02008188
	cmp r6, #1
	bcc .L_0200817e
	cmp r6, #2
	beq .L_02008192
	b .L_020081ca
.L_0200817e:
	adds r0, r7, #0
	movs r1, #2
	bl Func_02005d5c
	b .L_020081ca
.L_02008188:
	adds r0, r7, #0
	movs r1, #4
	bl Func_02005d5c
	b .L_020081ca
.L_02008192:
	adds r0, r7, #0
	movs r1, #6
	bl Func_02005d5c
	b .L_020081ca
.L_0200819c:
	movs r3, #255
	strb r3, [r5, #2]
	cmp r6, #1
	beq .L_020081b8
	cmp r6, #1
	bcc .L_020081ae
	cmp r6, #2
	beq .L_020081c2
	b .L_020081ca
.L_020081ae:
	adds r0, r7, #0
	movs r1, #1
	bl Func_02005d5c
	b .L_020081ca
.L_020081b8:
	adds r0, r7, #0
	movs r1, #3
	bl Func_02005d5c
	b .L_020081ca
.L_020081c2:
	adds r0, r7, #0
	movs r1, #5
	bl Func_02005d5c
.L_020081ca:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x02008216,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008218,"ax",%progbits
	.global Func_02000218
	.thumb_func
Func_02000218:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl Func_02005d74
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008262
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
	b .L_02008264
.L_02008262:
	movs r0, #0
.L_02008264:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008268,"ax",%progbits
	.global Func_02000268
	.thumb_func
Func_02000268:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl Func_02005d74
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020082b6
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
	b .L_020082b8
.L_020082b6:
	movs r0, #0
.L_020082b8:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020082bc,"ax",%progbits
	.global Func_020002bc
	.thumb_func
Func_020002bc:
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
	.section .text.x020082f4,"ax",%progbits
	.global Func_020002f4
	.thumb_func
Func_020002f4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	str r3, [sp, #0]
	ldr r3, .L_020084c4
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
	beq .L_0200833c
	cmp r7, #0
	beq .L_0200833c
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_02008344
.L_0200833c:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_02008344:
	mov r3, r8
	bl Func_02005d74
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02008352
	b .L_020084b6
.L_02008352:
	ldr r3, [r6, #80]
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	mov r8, r3
	bl Func_02005d5c
	ldr r2, .L_020084c8
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_02005d6c
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_020084cc
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
	ldr r3, .L_020084d0
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_020084b6
	cmp r7, #0
	beq .L_020084b6
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_020083d4
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_020083d4:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200840c
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
.L_0200840c:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_02008420
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02008420:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_02008466
	ldr r3, .L_020084c8
	mov r1, r11
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_0200844e
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_02008460
.L_0200844e:
	ldr r2, .L_020084d0
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_020084d0
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_02008460:
	bl Engine_MathDivide
	str r0, [r6, #52]
.L_02008466:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_02008482
	adds r0, r6, #0
	movs r1, #1
	bl Func_02005d5c
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02005d6c
.L_02008482:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02008494
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #18]
.L_02008494:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_020084a6
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_020084a6:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_020084b6
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_020084b6:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020084c4:
	.4byte gPartyState
.L_020084c8:
	.4byte Data_02006188
.L_020084cc:
	.4byte Func_020002bc
.L_020084d0:
	.4byte 0xffff0000
	.section .text.x020084d4,"ax",%progbits
	.global Func_020004d4
	.thumb_func
Func_020004d4:
	push {r5, r6, r7, lr}
	ldr r3, .L_0200854c
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	adds r7, r0, #0
	bl Random16Far
	movs r6, #255
	ands r0, r6
	cmp r0, #0
	beq .L_02008524
	bl Random16Far
	movs r2, #176
	ands r0, r6
	lsls r2, r2, #16
	lsls r0, r0, #16
	adds r0, r0, r2
	str r0, [r5, #8]
	bl Random16Far
	ldr r3, [r7, #12]
	movs r2, #31
	ands r2, r0
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5, #12]
	bl Random16Far
	ldr r3, [r7, #16]
	movs r2, #127
	ands r2, r0
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5, #16]
.L_02008524:
	adds r2, r5, #0
	adds r2, #100
	ldrh r3, [r2]
	adds r1, r5, #0
	adds r1, #102
	strh r3, [r1]
	movs r1, #0
	ldrsh r3, [r2, r1]
	cmp r3, #212
	bne .L_02008542
	adds r0, r5, #0
	movs r1, #9
	bl Func_02005d5c
	b .L_0200854a
.L_02008542:
	adds r0, r5, #0
	movs r1, #10
	bl Func_02005d5c
.L_0200854a:
	pop {r5, r6, r7, pc}
.L_0200854c:
	.4byte gPartyState
	.section .text.x02008550,"ax",%progbits
	.global Func_02000550
	.thumb_func
Func_02000550:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	adds r6, r0, #0
	cmp r3, #0
	bne .L_02008592
	movs r1, #173
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_02008592
	movs r1, #175
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_02008592
	movs r1, #180
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_020085a0
.L_02008592:
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildren
	bl Map_EnableUpdateCallback
	b .L_02008608
.L_020085a0:
	adds r0, r6, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildren
	bl Map_DisableUpdateCallback
	adds r0, r6, #0
	adds r0, #99
	ldrb r1, [r0]
	adds r3, r1, #0
	cmp r3, #0
	beq .L_020085cc
	ldr r3, .L_0200860c
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02008608
	adds r3, r1, #0
	adds r3, #255
	strb r3, [r0]
	b .L_02008608
.L_020085cc:
	adds r5, r6, #0
	adds r5, #102
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #0
	bne .L_020085de
	adds r0, r6, #0
	bl Func_020004d4
.L_020085de:
	ldrh r3, [r5]
	subs r3, #1
	strh r3, [r5]
	ldr r0, [r6, #16]
	ldr r3, [r6, #12]
	asrs r0, r0, #14
	asrs r3, r3, #15
	adds r0, r0, r3
	lsls r0, r0, #8
	bl Math_Cosine
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, [r6, #8]
	asrs r0, r0, #1
	adds r3, r3, r0
	str r3, [r6, #8]
	ldr r2, [r6, #48]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
.L_02008608:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200860c:
	.4byte Data_0300122c
	.section .text.x02008610,"ax",%progbits
	.global Func_02000610
	.thumb_func
Func_02000610:
	push {lr}
	movs r0, #0
	bl Func_02005f2c
	pop {pc}
	.2byte 0x0000
	.section .text.x0200861c,"ax",%progbits
	.global Func_0200061c
	.thumb_func
Func_0200061c:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r2, #1
	adds r3, #52
	strb r2, [r3]
	bx lr
	.section .text.x0200862c,"ax",%progbits
	.global Func_0200062c
	.thumb_func
Func_0200062c:
	push {r5, lr}
	adds r5, r1, #0
	adds r1, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_02008640
	movs r0, #0
	b .L_0200866c
.L_02008640:
	cmp r0, #2
	bhi .L_02008654
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_02008656
.L_02008654:
	ldr r4, .L_02008670
.L_02008656:
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
.L_0200866c:
	pop {r5, pc}
	.2byte 0x0000
.L_02008670:
	.4byte gMapCellBuffer
	.section .text.x02008674,"ax",%progbits
	.global Func_02000674
	.thumb_func
Func_02000674:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r5, #60
.L_0200867a:
	cmp r5, #0
	beq .L_0200868c
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #40]
	subs r5, #1
	cmp r3, #0
	bne .L_0200867a
.L_0200868c:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008690,"ax",%progbits
	.global Func_02000690
	.thumb_func
Func_02000690:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_020086a0
	movs r0, #0
	b .L_020086c6
.L_020086a0:
	cmp r0, #2
	bhi .L_020086b4
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_020086b6
.L_020086b4:
	ldr r4, .L_020086c8
.L_020086b6:
	lsls r3, r2, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	ldrb r0, [r4, #2]
	ldrb r3, [r4, #3]
	lsls r0, r0, #8
	orrs r0, r3
.L_020086c6:
	pop {pc}
.L_020086c8:
	.4byte gMapCellBuffer
	.section .text.x020086cc,"ax",%progbits
	.global Func_020006cc
	.thumb_func
Func_020006cc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	ldr r3, [r5, #8]
	movs r0, #0
	asrs r3, r3, #20
	mov r10, r3
	ldr r3, [r5, #16]
	mov r1, r10
	asrs r3, r3, #20
	mov r8, r3
	mov r2, r8
	sub sp, #8
	bl Func_02000690
	mov r1, r10
	mov r2, r8
	mov r9, r0
	movs r0, #2
	bl Func_02000690
	movs r2, #34
	adds r2, r2, r5
	ldr r1, [r5, #8]
	adds r7, r0, #0
	mov r11, r2
	ldrb r0, [r2]
	ldr r2, [r5, #16]
	bl Map_GetTerrainHeight
	adds r3, r5, #0
	adds r3, #100
	asrs r6, r0, #19
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl GameFlag_Test
	mov r3, r10
	mov r2, r8
	adds r1, r5, #0
	lsls r3, r3, #20
	lsls r2, r2, #20
	adds r1, #35
	str r3, [sp, #4]
	str r2, [sp, #0]
	cmp r0, #0
	beq .L_02008758
	ldr r3, .L_020087b0
	ands r7, r3
	movs r3, #0
	mov r9, r3
	ldr r3, [r5, #20]
	asrs r3, r3, #19
	cmp r3, r6
	beq .L_02008746
	subs r6, #4
.L_02008746:
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	movs r1, #3
	bl Object_SetSpritePriority
	b .L_0200877c
.L_02008758:
	movs r3, #255
	lsls r3, r3, #8
	orrs r7, r3
	movs r3, #212
	lsls r3, r3, #8
	adds r3, #128
	mov r2, r9
	orrs r2, r3
	mov r9, r2
	ldrb r2, [r1]
	movs r3, #253
	ands r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	movs r1, #1
	adds r6, #4
	bl Object_SetSpritePriority
.L_0200877c:
	mov r1, r10
	mov r2, r8
	mov r3, r9
	movs r0, #0
	bl Func_0200062c
	mov r1, r10
	mov r2, r8
	adds r3, r7, #0
	movs r0, #2
	bl Func_0200062c
	mov r3, r11
	ldrb r2, [r3]
	ldr r0, [sp, #4]
	ldr r1, [sp, #0]
	adds r3, r6, #0
	bl Func_02005df4
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020087b0:
	.4byte 0xffff00ff
	.section .text.x020087b4,"ax",%progbits
	.global Func_020007b4
	.thumb_func
Func_020007b4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	mov r8, r1
	ldrh r2, [r3, #26]
	mov r9, r0
	movs r5, #1
	movs r1, #26
	ldrsh r0, [r3, r1]
	ands r5, r2
	lsls r5, r5, #1
	sub sp, #4
	subs r5, r0, r5
	bl Object_GetById
	adds r5, #1
	adds r6, r0, #0
	adds r0, r5, #0
	bl Object_GetById
	mov r2, r8
	adds r7, r0, #0
	cmp r2, #0
	bne .L_0200885a
	ldr r3, [r6, #80]
	ldr r5, [r7, #80]
	mov r10, r3
	adds r3, r7, #0
	adds r3, #85
	strb r2, [r3]
	ldr r3, [r7, #12]
	ldr r0, .L_020088f4
	adds r3, r3, r0
	str r3, [r7, #12]
	movs r0, #1
	bl WaitFrames
	ldrb r3, [r5, #16]
	ldr r1, .L_020088f8
	lsls r3, r3, #2
	adds r4, r3, r1
	ldrh r3, [r4, #2]
	ldr r2, .L_020088fc
	mov r0, r10
	adds r3, r3, r2
	str r3, [r7, #104]
	ldrb r3, [r0, #16]
	mov r0, sp
	lsls r3, r3, #2
	adds r4, r3, r1
	ldrh r3, [r4, #2]
	mov r1, r8
	adds r3, r3, r2
	ldrh r2, [r4]
	str r3, [r6, #104]
	movs r4, #133
	movs r3, #128
	lsls r4, r4, #24
	lsrs r2, r2, #2
	lsls r3, r3, #19
	str r1, [r0]
	adds r3, #212
	ldr r1, [r7, #104]
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, [r6, #104]
	ldr r1, [r7, #104]
	adds r2, #192
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [r6, #104]
	movs r2, #192
	lsls r2, r2, #2
	b .L_02008882
.L_0200885a:
	mov r2, r8
	cmp r2, #5
	bgt .L_0200888c
	ldr r3, [r7, #12]
	ldr r0, .L_02008900
	movs r2, #132
	adds r3, r3, r0
	str r3, [r7, #12]
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, [r6, #104]
	ldr r1, [r7, #104]
	adds r2, #48
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [r6, #104]
	movs r2, #128
	lsls r2, r2, #1
.L_02008882:
	adds r3, r3, r2
	str r3, [r6, #104]
	ldr r3, [r7, #104]
	adds r3, r3, r2
	str r3, [r7, #104]
.L_0200888c:
	mov r1, r8
	cmp r1, #7
	bgt .L_0200889a
	ldr r3, [r6, #28]
	ldr r2, .L_02008904
	adds r3, r3, r2
	str r3, [r6, #28]
.L_0200889a:
	mov r3, r9
	cmp r3, #1
	bne .L_020088e6
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	ldr r3, [r7, #20]
	str r3, [r7, #12]
	adds r3, r6, #0
	adds r3, #100
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl GameFlag_SetBit
	adds r3, r7, #0
	adds r3, #100
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl GameFlag_ClearBit
	ldr r3, [r6, #80]
	adds r0, r6, #0
	ldrb r1, [r3, #24]
	adds r1, #1
	bl Func_02005d5c
	ldr r3, [r7, #80]
	adds r0, r7, #0
	ldrb r1, [r3, #24]
	subs r1, #1
	bl Func_02005d5c
	adds r0, r6, #0
	bl Func_020006cc
	adds r0, r7, #0
	bl Func_020006cc
.L_020088e6:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020088f4:
	.4byte 0xffe40000
.L_020088f8:
	.4byte ResourceTableEntries
.L_020088fc:
	.4byte 0x06010000
.L_02008900:
	.4byte 0x00053333
.L_02008904:
	.4byte 0xffffe100
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
	bl Object_GetById
	movs r3, #4
	ldrsh r2, [r7, r3]
	movs r1, #0
	mov r8, r2
	adds r6, r0, #0
	movs r3, #2
	ldrsh r5, [r7, r3]
	bl ObjectDispatch_SetSingleChildField26
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
	.section .text.x02008988,"ax",%progbits
	.global Func_02000988
	.thumb_func
Func_02000988:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008cd4
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #4
	bl Object_GetById
	adds r7, r0, #0
	ldr r6, [r7, #104]
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	movs r2, #85
	adds r2, r2, r7
	movs r3, #4
	strb r3, [r2]
	movs r1, #0
	adds r0, r7, #0
	mov r11, r2
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #137
	bl Func_02005f94
	movs r3, #99
	adds r3, r3, r6
	mov r8, r3
	ldrb r3, [r3]
	movs r5, #0
	cmp r3, #0
	beq .L_02008a28
.L_020089e2:
	ldr r3, [r6, #8]
	ldr r1, .L_02008cd8
	str r3, [r7, #8]
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r7, #12]
	ldr r3, [r6, #16]
	str r3, [r7, #16]
	cmp r5, r1
	bgt .L_020089fe
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #153
	adds r5, r5, r2
.L_020089fe:
	ldr r3, .L_02008cdc
	adds r1, r7, #0
	ldr r2, [r3]
	ldrb r3, [r3]
	adds r1, #35
	lsls r3, r3, #12
	strh r3, [r7, #6]
	movs r3, #1
	ands r2, r3
	movs r3, #2
	lsls r3, r2
	ldrb r2, [r1]
	movs r0, #1
	eors r3, r2
	strb r3, [r1]
	bl WaitFrames
	mov r1, r8
	ldrb r3, [r1]
	cmp r3, #0
	bne .L_020089e2
.L_02008a28:
	movs r0, #144
	lsls r0, r0, #1
	bl Func_02005f94
	ldr r3, .L_02008cd4
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008ce0
	cmp r2, r3
	bne .L_02008a56
	ldr r3, [r7, #8]
	asrs r3, r3, #19
	cmp r3, #39
	bne .L_02008a56
	movs r0, #16
	bl Func_02005efc
	bl Func_02005e1c
	b .L_02008cc6
.L_02008a56:
	ldrh r3, [r6, #6]
	movs r2, #35
	movs r1, #192
	adds r2, r2, r7
	lsls r1, r1, #8
	mov r9, r2
	cmp r3, r1
	beq .L_02008a68
	b .L_02008c00
.L_02008a68:
	ldr r2, [r6, #104]
	movs r3, #0
	mov r10, r3
	mov r8, r3
	mov r1, r9
	movs r3, #4
	str r2, [sp, #0]
	strb r3, [r1]
	ldr r3, [r7, #8]
	movs r2, #2
	negs r2, r2
	asrs r3, r3, #19
	ands r3, r2
	adds r3, #1
	lsls r3, r3, #19
	str r3, [r7, #8]
	movs r5, #0
	ldr r3, [r6, #12]
	asrs r3, r3, #19
	ands r3, r2
	adds r3, #3
	lsls r3, r3, #19
	str r3, [r7, #12]
	ldr r3, [r7, #16]
	asrs r3, r3, #19
	ands r3, r2
	adds r3, #1
	lsls r3, r3, #19
	str r3, [r7, #16]
.L_02008aa2:
	ldr r0, .L_02008ce4
	movs r2, #64
	ldr r3, [r0]
	movs r1, #0
	ands r3, r2
	cmp r3, #0
	beq .L_02008abe
	movs r3, #192
	ldr r2, .L_02008ce8
	lsls r3, r3, #8
	strh r3, [r7, #6]
	mov r10, r1
	mov r8, r2
	movs r1, #1
.L_02008abe:
	ldr r3, [r0]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_02008ad8
	movs r1, #0
	mov r8, r1
	mov r2, r8
	movs r3, #128
	strh r2, [r7, #6]
	lsls r3, r3, #13
	mov r10, r3
	movs r1, #1
.L_02008ad8:
	ldr r3, [r0]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_02008af2
	ldr r3, .L_02008ce8
	movs r1, #0
	mov r10, r3
	movs r3, #128
	lsls r3, r3, #8
	strh r3, [r7, #6]
	mov r8, r1
	movs r1, #1
.L_02008af2:
	cmp r1, #0
	beq .L_02008b12
	ldr r2, [r6, #16]
	ldr r3, [r6, #12]
	ldr r1, [r6, #8]
	subs r2, r2, r3
	add r1, r10
	add r2, r8
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #2
	bl Func_02000690
	asrs r0, r0, #8
	cmp r0, #255
	bne .L_02008b7c
.L_02008b12:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #60
	bne .L_02008aa2
	ldr r3, .L_02008ce8
	movs r2, #0
	mov r8, r3
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r7, #6]
	mov r10, r2
	ldr r3, [r6, #12]
	ldr r2, [r6, #16]
	ldr r1, [r6, #8]
	subs r2, r2, r3
	add r2, r8
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #2
	bl Func_02000690
	lsls r0, r0, #8
	lsrs r0, r0, #16
	cmp r0, #255
	bne .L_02008b7c
	movs r2, #0
	mov r8, r2
	mov r3, r8
	strh r3, [r7, #6]
	movs r1, #128
	lsls r1, r1, #13
	mov r10, r1
	ldr r2, [r6, #16]
	ldr r1, [r6, #8]
	ldr r3, [r6, #12]
	add r1, r10
	subs r2, r2, r3
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #2
	bl Func_02000690
	lsls r0, r0, #8
	lsrs r0, r0, #16
	cmp r0, #255
	bne .L_02008b7c
	movs r3, #128
	ldr r1, .L_02008ce8
	lsls r3, r3, #8
	strh r3, [r7, #6]
	mov r10, r1
.L_02008b7c:
	ldr r5, [sp, #0]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	adds r5, #98
	add r2, r8
	add r1, r10
	ldrb r0, [r5]
	bl Map_GetTerrainHeight
	movs r1, #6
	str r0, [r7, #12]
	str r0, [r7, #20]
	adds r0, r7, #0
	bl Func_02005d5c
	movs r0, #6
	bl WaitFrames
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
	mov r2, r11
	movs r3, #2
	strb r3, [r2]
	mov r1, r9
	movs r3, #1
	strb r3, [r1]
	adds r2, r7, #0
	ldrb r3, [r5]
	adds r2, #34
	strb r3, [r2]
	ldr r1, [r7, #8]
	ldr r3, [r7, #16]
	ldr r2, [r7, #12]
	add r3, r8
	add r1, r10
	adds r0, r7, #0
	bl Func_02005d8c
	adds r0, r7, #0
	bl Func_02000674
	adds r0, r7, #0
	bl Func_02005d94
	adds r0, r7, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #3
	mov r2, r11
	strb r3, [r2]
	b .L_02008c90
.L_02008c00:
	ldr r3, [r6, #8]
	ldrh r1, [r6, #6]
	movs r2, #2
	negs r2, r2
	asrs r3, r3, #19
	ands r3, r2
	asrs r1, r1, #13
	adds r3, r3, r1
	subs r3, #1
	lsls r3, r3, #19
	str r3, [r7, #8]
	adds r0, r7, #0
	ldr r3, [r6, #16]
	movs r6, #0
	asrs r3, r3, #19
	ands r3, r2
	movs r2, #2
	ands r1, r2
	subs r3, r3, r1
	adds r3, #1
	lsls r3, r3, #19
	str r3, [r7, #16]
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r7, #40]
	mov r1, r9
	movs r3, #33
	strb r3, [r1]
	mov r2, r11
	movs r3, #3
	strb r3, [r2]
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02005ef4
	movs r3, #128
	adds r5, r0, #0
	lsls r3, r3, #12
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #52]
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	ldr r1, [r7, #8]
	adds r2, r0, #0
	ldr r3, [r7, #16]
	adds r0, r5, #0
	bl Func_02005d8c
	adds r0, r7, #0
	bl Func_02000674
	adds r0, r5, #0
	bl Func_02005d94
.L_02008c90:
	ldr r5, .L_02008cd4
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	ldr r0, [r5]
	bl Object_AttachWorkTargetToObject
	bl Func_02005ee4
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #10
	bl WaitFrames
	bl Func_02005e1c
.L_02008cc6:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008cd4:
	.4byte gPartyState
.L_02008cd8:
	.4byte 0x000bffff
.L_02008cdc:
	.4byte Data_0300122c
.L_02008ce0:
	.4byte 0x00000055
.L_02008ce4:
	.4byte gInput
.L_02008ce8:
	.4byte 0xfff00000
	.section .text.x02008cec,"ax",%progbits
	.global Func_02000cec
	.thumb_func
Func_02000cec:
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
	ldr r3, .L_02008d14
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_02008d14:
	.4byte IwramFillWords + 0x74
	.section .text.x02008d18,"ax",%progbits
	.global Func_02000d18
	.thumb_func
Func_02000d18:
	push {lr}
	ldmia r1!, {r3}
	ldmia r0!, {r4}
	ldr r2, [r1]
	subs r4, r4, r3
	ldr r3, [r0]
	asrs r4, r4, #16
	subs r3, r3, r2
	movs r2, #192
	lsls r2, r2, #11
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r2, r3, #0
	muls r2, r3
	adds r0, r4, #0
	muls r0, r4
	adds r3, r2, #0
	adds r0, r0, r3
	ldr r3, .L_02008d44
	mov lr, r3
	.2byte 0xf800
	pop {pc}
.L_02008d44:
	.4byte IwramFillWords + 0x74
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
	bl Object_GetById
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
	bl ObjectDispatch_ApplyValueToChildren
	b .L_02008f24
.L_02008dc0:
	adds r0, r6, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildren
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
	.4byte gPartyState
.L_02008e3c:
	.4byte Data_020061ac
.L_02008e40:
	.4byte Data_02006194
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
	.4byte gPartyState
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
	bl Func_02005d74
	movs r1, #2
	adds r5, r0, #0
	bl Func_02005d5c
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
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
	.section .text.x02009084,"ax",%progbits
	.global Func_02001084
	.thumb_func
Func_02001084:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #24
	adds r0, #255
	asrs r6, r1, #16
	lsrs r7, r2, #24
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020090ae
	adds r3, r5, #0
	adds r3, #90
	strb r0, [r3]
	adds r3, #10
	strh r6, [r3]
	subs r3, #2
	strb r7, [r3]
	ldr r3, .L_020090b0
	str r3, [r5, #108]
.L_020090ae:
	pop {r5, r6, r7, pc}
.L_020090b0:
	.4byte Func_02000f34
	.section .text.x020090b4,"ax",%progbits
	.global Func_020010b4
	.thumb_func
Func_020010b4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r2, .L_02009198
	movs r3, #133
	mov r9, r2
	lsls r3, r3, #2
	add r3, r9
	adds r6, r0, #0
	ldr r0, [r3]
	bl Object_GetById
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
	bne .L_0200911e
	movs r3, #173
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200911e
	movs r3, #175
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200911e
	movs r3, #180
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r7, [r3, r2]
	cmp r7, #0
	bne .L_0200911e
	movs r3, #217
	lsls r3, r3, #1
	add r3, r8
	ldrh r3, [r3]
	cmp r3, #0
	bne .L_02009128
.L_0200911e:
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildren
	b .L_02009280
.L_02009128:
	adds r0, r6, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildren
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
	bne .L_02009166
	movs r1, #142
	lsls r1, r1, #1
	adds r0, r6, #0
	adds r1, #255
	bl Func_02005f8c
	adds r3, r6, #0
	adds r3, #99
	strb r7, [r3]
	ldr r1, .L_0200919c
	adds r0, r6, #0
	str r7, [r6, #108]
	bl Func_02005d6c
	b .L_02009280
.L_02009166:
	movs r3, #98
	adds r3, r3, r6
	mov r10, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_02009212
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r9
	ldrb r3, [r3]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r7, #0
	adds r0, #8
	adds r1, #8
	cmp r3, #2
	bne .L_020091a0
	bl Func_02000d18
	cmp r0, #16
	bgt .L_020091b6
	ldr r2, [r5, #16]
	ldr r3, [r6, #16]
	b .L_020091ac
.L_02009198:
	.4byte gPartyState
.L_0200919c:
	.4byte Data_020061cc
.L_020091a0:
	bl Func_02000cec
	cmp r0, #8
	bgt .L_020091b6
	ldr r2, [r5, #12]
	ldr r3, [r6, #12]
.L_020091ac:
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020091b6
	movs r7, #1
.L_020091b6:
	cmp r7, #0
	beq .L_02009212
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009212
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
	ldr r3, .L_02009238
	movs r2, #128
	ldr r4, .L_02009234
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	strb r4, [r3]
	mov r2, r10
	movs r3, #1
	strb r3, [r2]
	adds r2, r6, #0
	adds r2, #99
	strb r3, [r2]
.L_02009212:
	ldrh r3, [r6, #6]
	movs r2, #192
	lsls r2, r2, #8
	adds r0, r3, #0
	cmp r3, r2
	bne .L_02009258
	bl Math_Cosine
	movs r1, #192
	lsls r1, r1, #9
	ldr r5, .L_0200923c
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r3, r3, r0
	b .L_02009240
	.2byte 0x0000
.L_02009234:
	.4byte 0x00000000
.L_02009238:
	.4byte gPartyState
.L_0200923c:
	.4byte IwramMulQ16
.L_02009240:
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
	b .L_02009280
.L_02009258:
	bl Math_Cosine
	movs r1, #192
	lsls r1, r1, #9
	ldr r5, .L_0200928c
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
.L_02009280:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200928c:
	.4byte IwramMulQ16
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
	bl Object_GetById
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
	bl Object_GetById
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
	bl ObjectDispatch_SetSingleChildField26
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
	.section .text.x02009420,"ax",%progbits
	.global Func_02001420
	.thumb_func
Func_02001420:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #24
	adds r0, #255
	asrs r6, r1, #16
	lsrs r7, r2, #24
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009452
	adds r3, r5, #0
	adds r3, #90
	strb r0, [r3]
	adds r3, #10
	strh r6, [r3]
	subs r3, #2
	strb r7, [r3]
	ldr r3, .L_02009454
	adds r0, r5, #0
	movs r1, #0
	str r3, [r5, #108]
	bl ObjectDispatch_SetSingleChildField26
.L_02009452:
	pop {r5, r6, r7, pc}
.L_02009454:
	.4byte Func_02001290
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
	bl Object_GetById
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
	bl ObjectDispatch_ApplyValueToChildren
	b .L_020095f0
.L_020094dc:
	movs r1, #16
	adds r0, r6, #0
	bl ObjectDispatch_ApplyValueToChildren
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
	bl Object_GetById
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
	bl Object_GetById
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
	.4byte gPartyState
.L_02009600:
	.4byte 0xfff80000
.L_02009604:
	.4byte 0x00000057
.L_02009608:
	.4byte 0x0000005c
	.section .text.x0200960c,"ax",%progbits
	.global Func_0200160c
	.thumb_func
Func_0200160c:
	push {r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	movs r0, #30
	adds r1, r4, #0
	adds r3, r2, #0
	adds r0, #255
	adds r2, r5, #0
	bl Func_02005d74
	movs r1, #1
	adds r5, r0, #0
	bl Func_02005d5c
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #6
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	adds r2, r5, #0
	ldr r1, .L_02009680
	adds r2, #100
	movs r3, #27
	strh r3, [r2]
	adds r3, r5, #0
	adds r3, #85
	strb r1, [r3]
	adds r3, #5
	strb r1, [r3]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #24]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #48]
	bl Random16Far
	movs r3, #254
	lsls r3, r3, #7
	ldr r2, .L_02009684
	adds r3, #255
	ands r3, r0
	lsls r3, r3, #1
	adds r3, r3, r2
	str r3, [r5, #52]
	movs r3, #224
	lsls r3, r3, #6
	str r3, [r5, #76]
	ldr r3, .L_02009688
	str r3, [r5, #108]
	b .L_0200968c
.L_02009680:
	.4byte 0x00000000
.L_02009684:
	.4byte 0xffff8001
.L_02009688:
	.4byte Func_02001458
.L_0200968c:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009690,"ax",%progbits
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
	bl Object_GetById
	ldr r3, [r0, #16]
	asrs r3, r3, #19
	cmp r3, #41
	ble .L_02009766
	ldr r0, [r5]
	bl Object_GetById
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
	bl Object_GetById
	ldr r3, [r0, #16]
	asrs r3, r3, #19
	cmp r3, #41
	ble .L_020097be
	ldr r0, [r5]
	bl Object_GetById
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
	.4byte Data_0300122c
.L_020097e0:
	.4byte gPartyState
.L_020097e4:
	.4byte 0x00000057
.L_020097e8:
	.4byte 0x0000005c
.L_020097ec:
	.4byte 0x025b0000
.L_020097f0:
	.4byte 0xffe00000
	.section .text.x020097f4,"ax",%progbits
	.global Func_020017f4
	.thumb_func
Func_020017f4:
	ldr r3, .L_02009824
	movs r2, #35
	adds r2, r2, r0
	mov r12, r2
	ldr r2, [r3]
	movs r3, #1
	ands r2, r3
	mov r4, r12
	movs r3, #2
	lsls r3, r2
	ldrb r2, [r4]
	ldr r1, [r0, #104]
	eors r3, r2
	strb r3, [r4]
	movs r2, #128
	ldr r3, [r1, #8]
	lsls r2, r2, #12
	str r3, [r0, #8]
	ldr r3, [r1, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r3, [r1, #16]
	str r3, [r0, #16]
	bx lr
.L_02009824:
	.4byte Data_0300122c
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
	bl Object_GetById
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
	bl Func_02005d74
	movs r1, #2
	adds r7, r0, #0
	bl Func_02005d5c
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
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
	bl Battle_WaitMode0
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
	bl Func_02005d74
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
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetPartAttribute
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
	bl Object_SetPartAttribute
	b .L_020099ca
.L_020099c0:
	mov r4, r8
	ldr r0, [r4, #20]
	movs r1, #4
	bl Object_SetPartAttribute
.L_020099ca:
	movs r0, #1
	bl Battle_WaitMode0
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
	bl Object_SetPartAttribute
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
	bl Motion_CamBounds
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
	bl Object_GetById
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
	bl ObjectDispatch_SetSingleChildField26
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
	bl Object_GetById
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
	bl ObjectDispatch_SetSingleChildField26
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
	.4byte gPartyState
.L_02009be0:
	.4byte 0x0001b333
.L_02009be4:
	.4byte 0xffff0000
.L_02009be8:
	.4byte Data_0300122c
.L_02009bec:
	.4byte Data_02005fd8
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
	bl Object_GetById
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
	bl ObjectDispatch_SetSingleChildField26
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
	bl Battle_WaitMode0
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
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetSpritePriority
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
	bl Func_02000674
	movs r0, #1
	bl WaitFrames
	adds r0, r5, #0
	bl Func_02000674
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
	bl Func_02000674
	movs r0, #1
	bl WaitFrames
	adds r0, r5, #0
	bl Func_02000674
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
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
	.4byte Data_0300122c
.L_02009f74:
	.4byte gPartyState
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
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
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
	.4byte gPartyState
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
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #19
	cmp r3, #65
	bne .L_0200a3b6
	movs r0, #8
	bl Object_GetById
	ldr r5, .L_0200a5c0
	ldr r3, [r0, #8]
	add r7, sp, #120
	adds r3, r3, r5
	str r3, [r7]
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r0, #8
	str r3, [r7, #4]
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r0, #128
	lsls r0, r0, #12
	adds r3, r3, r0
	movs r1, #2
	str r3, [r7, #8]
	movs r0, #8
	bl ObjectMotion_SetActionVariant
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
	bl WaitFrames
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
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	b .L_0200a590
.L_0200a3b6:
	movs r0, #30
	bl Battle_WaitMode0
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
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #19
	cmp r3, #65
	bne .L_0200a4f2
	movs r0, #8
	bl Object_GetById
	ldr r5, .L_0200a5c0
	ldr r3, [r0, #8]
	add r7, sp, #120
	adds r3, r3, r5
	str r3, [r7]
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r0, #8
	str r3, [r7, #4]
	bl Object_GetById
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
	bl WaitFrames
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
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
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
	.4byte gPartyState
	.section .text.x0200a5cc,"ax",%progbits
	.global Func_020025cc
	.thumb_func
Func_020025cc:
	push {lr}
	ldr r3, .L_0200a5e8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200a5ec
	cmp r2, r3
	bne .L_0200a5e4
	ldr r0, .L_0200a5f0
	b .L_0200a5e6
.L_0200a5e4:
	ldr r0, .L_0200a5f4
.L_0200a5e6:
	pop {pc}
.L_0200a5e8:
	.4byte gPartyState
.L_0200a5ec:
	.4byte 0x00000055
.L_0200a5f0:
	.4byte Data_02006268
.L_0200a5f4:
	.4byte Data_02006238
	.section .text.x0200a5f8,"ax",%progbits
	.global Func_020025f8
	.thumb_func
Func_020025f8:
	movs r0, #0
	bx lr
	.section .text.x0200a5fc,"ax",%progbits
	.global Func_020025fc
	.thumb_func
Func_020025fc:
	ldr r0, .L_0200a600
	bx lr
.L_0200a600:
	.4byte Data_02006400
	.section .text.x0200a604,"ax",%progbits
	.global Func_02002604
	.thumb_func
Func_02002604:
	push {lr}
	ldr r3, .L_0200a670
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200a674
	cmp r2, r3
	bne .L_0200a61c
	ldr r0, .L_0200a678
	b .L_0200a66e
.L_0200a61c:
	ldr r3, .L_0200a67c
	cmp r2, r3
	bne .L_0200a626
	ldr r0, .L_0200a680
	b .L_0200a66e
.L_0200a626:
	ldr r3, .L_0200a684
	cmp r2, r3
	bne .L_0200a630
	ldr r0, .L_0200a688
	b .L_0200a66e
.L_0200a630:
	ldr r3, .L_0200a68c
	cmp r2, r3
	bne .L_0200a63a
	ldr r0, .L_0200a690
	b .L_0200a66e
.L_0200a63a:
	ldr r3, .L_0200a694
	cmp r2, r3
	bne .L_0200a644
	ldr r0, .L_0200a698
	b .L_0200a66e
.L_0200a644:
	ldr r3, .L_0200a69c
	cmp r2, r3
	bne .L_0200a64e
	ldr r0, .L_0200a6a0
	b .L_0200a66e
.L_0200a64e:
	ldr r3, .L_0200a6a4
	cmp r2, r3
	bne .L_0200a658
	ldr r0, .L_0200a6a8
	b .L_0200a66e
.L_0200a658:
	ldr r3, .L_0200a6ac
	cmp r2, r3
	bne .L_0200a662
	ldr r0, .L_0200a6b0
	b .L_0200a66e
.L_0200a662:
	ldr r3, .L_0200a6b4
	cmp r2, r3
	bne .L_0200a66c
	ldr r0, .L_0200a6b8
	b .L_0200a66e
.L_0200a66c:
	ldr r0, .L_0200a6bc
.L_0200a66e:
	pop {pc}
.L_0200a670:
	.4byte gPartyState
.L_0200a674:
	.4byte 0x00000055
.L_0200a678:
	.4byte Data_02006564
.L_0200a67c:
	.4byte 0x00000056
.L_0200a680:
	.4byte Data_0200663c
.L_0200a684:
	.4byte 0x00000057
.L_0200a688:
	.4byte Data_020066b4
.L_0200a68c:
	.4byte 0x00000058
.L_0200a690:
	.4byte Data_0200675c
.L_0200a694:
	.4byte 0x00000059
.L_0200a698:
	.4byte Data_020068ac
.L_0200a69c:
	.4byte 0x0000005a
.L_0200a6a0:
	.4byte Data_020068f4
.L_0200a6a4:
	.4byte 0x0000005b
.L_0200a6a8:
	.4byte Data_020069e4
.L_0200a6ac:
	.4byte 0x0000005c
.L_0200a6b0:
	.4byte Data_02006aec
.L_0200a6b4:
	.4byte 0x0000005d
.L_0200a6b8:
	.4byte Data_02006bac
.L_0200a6bc:
	.4byte Data_0200654c
	.section .text.x0200a6c0,"ax",%progbits
	.global Func_020026c0
	.thumb_func
Func_020026c0:
	push {r5, r6, r7, lr}
	sub sp, #8
	bl Func_02005f84
	bl Object_GetById
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
	.4byte Data_02006c0c
.L_0200a758:
	.4byte gOverlayArea + 0x73e4
.L_0200a75c:
	.4byte gOverlayArea + 0x73e8
.L_0200a760:
	.4byte gOverlayArea + 0x73dc
.L_0200a764:
	.4byte gOverlayArea + 0x73e0
.L_0200a768:
	.4byte gPartyState
.L_0200a76c:
	.4byte 0x00000059
	.section .text.x0200a770,"ax",%progbits
	.global Func_02002770
	.thumb_func
Func_02002770:
	push {lr}
	ldr r3, .L_0200a788
	movs r2, #1
	negs r2, r2
	movs r1, #144
	str r2, [r3]
	lsls r1, r1, #3
	ldr r0, .L_0200a78c
	bl Scheduler_AddOrUpdateCallback
	pop {pc}
	.2byte 0x0000
.L_0200a788:
	.4byte Data_02006c0c
.L_0200a78c:
	.4byte Func_020026c0
	.section .text.x0200a790,"ax",%progbits
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
	bl Object_GetById
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
	.4byte Data_02006c0c
.L_0200a868:
	.4byte gOverlayArea + 0x73dc
.L_0200a86c:
	.4byte gOverlayArea + 0x73e0
.L_0200a870:
	.4byte gPartyState
.L_0200a874:
	.4byte 0x00000055
.L_0200a878:
	.4byte gOverlayArea + 0x73e4
.L_0200a87c:
	.4byte 0x00000056
.L_0200a880:
	.4byte 0x00000057
.L_0200a884:
	.4byte gOverlayArea + 0x73e8
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
	bl Object_GetById
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
	.4byte Data_02006c0c
.L_0200a95c:
	.4byte gOverlayArea + 0x73dc
.L_0200a960:
	.4byte gOverlayArea + 0x73e0
.L_0200a964:
	.4byte gPartyState
.L_0200a968:
	.4byte 0x00000055
.L_0200a96c:
	.4byte gOverlayArea + 0x73e4
.L_0200a970:
	.4byte 0x00000056
.L_0200a974:
	.4byte gOverlayArea + 0x73e8
.L_0200a978:
	.4byte 0x00000059
.L_0200a97c:
	.4byte 0x0000005a
	.section .text.x0200a980,"ax",%progbits
	.global Func_02002980
	.thumb_func
Func_02002980:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	bl Func_02005f84
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r4, r3, #20
	ldr r3, [r0, #12]
	asrs r7, r3, #20
	ldr r3, [r0, #16]
	ldr r0, .L_0200aa00
	asrs r1, r3, #20
	lsls r3, r4, #16
	adds r2, r3, r1
	ldr r3, [r0]
	cmp r2, r3
	beq .L_0200a9f4
	str r2, [r0]
	ldr r2, .L_0200aa04
	movs r3, #1
	str r3, [r2]
	ldr r3, .L_0200aa08
	ldr r6, .L_0200aa0c
	mov r8, r3
	ldr r5, .L_0200aa10
	mov r10, r2
	movs r3, #3
	mov r2, r8
	subs r1, r1, r7
	str r3, [r2]
	adds r1, #64
	movs r3, #73
	movs r2, #72
	str r4, [r6]
	adds r0, r4, #0
	str r1, [r5]
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #3
	movs r2, #1
	bl Func_02005db4
	mov r3, r10
	mov r1, r8
	ldr r2, [r3]
	ldr r0, [r5]
	ldr r3, [r1]
	ldr r1, [r6]
	str r0, [sp, #4]
	str r1, [sp, #0]
	movs r0, #72
	movs r1, #72
	bl Func_02005db4
.L_0200a9f4:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200aa00:
	.4byte Data_02006c0c
.L_0200aa04:
	.4byte gOverlayArea + 0x73dc
.L_0200aa08:
	.4byte gOverlayArea + 0x73e0
.L_0200aa0c:
	.4byte gOverlayArea + 0x73e4
.L_0200aa10:
	.4byte gOverlayArea + 0x73e8
	.section .text.x0200aa14,"ax",%progbits
	.global Func_02002a14
	.thumb_func
Func_02002a14:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	bl Func_02005f84
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r4, r3, #20
	ldr r3, [r0, #12]
	asrs r7, r3, #20
	ldr r3, [r0, #16]
	ldr r0, .L_0200aa94
	asrs r1, r3, #20
	lsls r3, r4, #16
	adds r2, r3, r1
	ldr r3, [r0]
	cmp r2, r3
	beq .L_0200aa88
	str r2, [r0]
	ldr r2, .L_0200aa98
	movs r3, #1
	str r3, [r2]
	ldr r3, .L_0200aa9c
	ldr r6, .L_0200aaa0
	mov r8, r3
	ldr r5, .L_0200aaa4
	mov r10, r2
	movs r3, #3
	mov r2, r8
	subs r1, r1, r7
	str r3, [r2]
	adds r1, #63
	movs r3, #73
	movs r2, #72
	str r4, [r6]
	adds r0, r4, #0
	str r1, [r5]
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #3
	movs r2, #1
	bl Func_02005db4
	mov r3, r10
	mov r1, r8
	ldr r2, [r3]
	ldr r0, [r5]
	ldr r3, [r1]
	ldr r1, [r6]
	str r0, [sp, #4]
	str r1, [sp, #0]
	movs r0, #72
	movs r1, #72
	bl Func_02005db4
.L_0200aa88:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200aa94:
	.4byte Data_02006c0c
.L_0200aa98:
	.4byte gOverlayArea + 0x73dc
.L_0200aa9c:
	.4byte gOverlayArea + 0x73e0
.L_0200aaa0:
	.4byte gOverlayArea + 0x73e4
.L_0200aaa4:
	.4byte gOverlayArea + 0x73e8
	.section .text.x0200aaa8,"ax",%progbits
	.global Func_02002aa8
	.thumb_func
Func_02002aa8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #68
	add r6, sp, #28
	movs r3, #1
	str r3, [r6]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #8]
	str r3, [r6, #12]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r6, #16]
	str r3, [r6, #20]
	mov r10, r0
	mov r8, r1
	movs r7, #0
.L_0200aace:
	bl Random16Far
	movs r3, #63
	ldr r2, .L_0200ab50
	ands r3, r0
	lsls r3, r3, #16
	add r3, r10
	add r5, sp, #16
	adds r3, r3, r2
	str r3, [r5]
	bl Random16Far
	movs r3, #15
	ands r3, r0
	lsls r3, r3, #12
	str r3, [r5, #4]
	bl Random16Far
	movs r2, #7
	ands r2, r0
	ldr r3, .L_0200ab54
	lsls r2, r2, #16
	add r2, r8
	adds r2, r2, r3
	ldr r3, [r5, #4]
	str r2, [r5, #8]
	ldr r0, [r5]
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #224
	lsls r3, r3, #12
	adds r3, #1
	str r3, [sp, #8]
	movs r1, #0
	movs r3, #0
	adds r7, #1
	str r6, [sp, #12]
	bl Func_020002f4
	cmp r7, #8
	bne .L_0200aace
	movs r0, #160
	movs r1, #224
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
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ab50:
	.4byte 0xffe00000
.L_0200ab54:
	.4byte 0xfff00000
	.section .text.x0200ab58,"ax",%progbits
	.global Func_02002b58
	.thumb_func
Func_02002b58:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #68
	movs r3, #168
	add r5, sp, #28
	lsls r3, r3, #2
	strh r3, [r5, #24]
	movs r3, #1
	str r3, [r5]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r5, #8]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #12]
	mov r8, r0
	adds r7, r1, #0
	movs r6, #0
.L_0200ab80:
	bl Random16Far
	adds r3, r0, #0
	movs r0, #31
	ands r0, r3
	ldr r3, .L_0200abcc
	lsls r0, r0, #16
	add r0, r8
	adds r0, r0, r3
	movs r3, #128
	add r2, sp, #16
	lsls r3, r3, #11
	str r0, [r2]
	str r3, [r2, #4]
	str r7, [r2, #8]
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #208
	lsls r3, r3, #13
	str r3, [sp, #8]
	movs r1, #0
	adds r2, r7, #0
	movs r3, #0
	str r5, [sp, #12]
	adds r6, #1
	bl Func_020002f4
	movs r0, #5
	bl Battle_WaitMode0
	cmp r6, #12
	bne .L_0200ab80
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200abcc:
	.4byte 0xfff00000
	.section .text.x0200abd0,"ax",%progbits
	.global Func_02002bd0
	.thumb_func
Func_02002bd0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	adds r6, r0, #0
	ldr r3, [r6, #20]
	ldr r5, [r6, #12]
	ldr r0, [r6, #48]
	subs r5, r5, r3
	movs r3, #128
	lsls r3, r3, #13
	asrs r5, r5, #1
	adds r5, r5, r3
	movs r3, #255
	ands r0, r3
	lsls r0, r0, #11
	mov r8, r3
	bl Math_Cosine
	ldr r3, .L_0200ac3c
	adds r1, r5, #0
	mov r10, r3
	mov lr, r10
	.2byte 0xf800
	ldr r3, [r6, #68]
	adds r3, r3, r0
	ldr r0, [r6, #48]
	str r3, [r6, #8]
	mov r3, r8
	ands r0, r3
	lsls r0, r0, #11
	bl Math_Sine
	adds r1, r5, #0
	mov lr, r10
	.2byte 0xf800
	movs r1, #3
	bl Engine_MathDivide
	ldr r3, [r6, #76]
	ldr r2, [r6, #72]
	adds r3, r3, r0
	str r3, [r6, #16]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #48]
	adds r3, #1
	str r3, [r6, #48]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200ac3c:
	.4byte IwramMulQ16
	.section .text.x0200ac40,"ax",%progbits
	.global Func_02002c40
	.thumb_func
Func_02002c40:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	ldr r6, [r5, #12]
	ldr r0, [r5, #48]
	movs r3, #192
	lsls r3, r3, #12
	asrs r6, r6, #2
	adds r6, r6, r3
	movs r3, #255
	ands r0, r3
	lsls r0, r0, #11
	mov r8, r3
	bl Math_Cosine
	ldr r3, .L_0200acb8
	adds r1, r6, #0
	mov r10, r3
	mov lr, r10
	.2byte 0xf800
	ldr r3, [r5, #68]
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r0, [r5, #48]
	str r3, [r5, #8]
	mov r3, r8
	ands r0, r3
	lsls r0, r0, #11
	bl Math_Sine
	adds r1, r6, #0
	mov lr, r10
	.2byte 0xf800
	ldr r3, [r5, #76]
	ldr r2, [r5, #72]
	adds r3, r3, r0
	str r3, [r5, #16]
	ldr r3, [r5, #12]
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r3, [r5, #48]
	movs r2, #2
	adds r3, #1
	str r3, [r5, #48]
	ldr r3, .L_0200acbc
	ldr r3, [r3]
	ands r3, r2
	lsrs r3, r3, #1
	lsls r1, r3, #3
	adds r1, r1, r3
	bl Object_SetPartAttribute
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200acb8:
	.4byte IwramMulQ16
.L_0200acbc:
	.4byte Data_0300122c
	.section .text.x0200acc0,"ax",%progbits
	.global Func_02002cc0
	.thumb_func
Func_02002cc0:
	push {lr}
	ldr r3, .L_0200acec
	movs r2, #3
	ldr r3, [r3]
	ands r3, r2
	adds r2, r0, #0
	adds r2, #99
	ldrb r2, [r2]
	lsrs r3, r2
	movs r2, #1
	ands r3, r2
	adds r2, r0, #0
	adds r2, #98
	ldrb r2, [r2]
	adds r1, r2, #0
	muls r1, r3
	lsls r1, r1, #24
	lsrs r1, r1, #24
	bl Object_SetPartAttribute
	pop {pc}
	.2byte 0x0000
.L_0200acec:
	.4byte Data_0300122c
	.section .text.x0200acf0,"ax",%progbits
	.global Func_02002cf0
	.thumb_func
Func_02002cf0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #128
	lsls r0, r0, #4
	sub sp, #12
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #254
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	movs r2, #128
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #3
	adds r3, #8
	strh r2, [r3]
	ldr r0, .L_0200ad84
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_02005d24
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #192
	adds r3, #212
	adds r0, r5, #0
	lsls r1, r1, #19
	ldr r2, .L_0200ad88
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r5, #0
	bl Sys_Free
	ldr r3, .L_0200ad80
	mov r0, sp
	adds r0, #10
	strh r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r1, .L_0200ad8c
	ldr r2, .L_0200ad90
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	ldr r0, .L_0200ad94
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_02005d24
	movs r7, #0
	adds r4, r5, #0
	b .L_0200ad98
	.2byte 0x0000
.L_0200ad80:
	.4byte 0x00000000
.L_0200ad84:
	.4byte 0x000001c0
.L_0200ad88:
	.4byte 0x84000200
.L_0200ad8c:
	.4byte 0x06002000
.L_0200ad90:
	.4byte 0x81000400
.L_0200ad94:
	.4byte 0x000001c1
.L_0200ad98:
	ldrh r2, [r4]
	movs r3, #252
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	adds r7, #1
	strh r3, [r4]
	adds r4, #2
	cmp r7, #64
	bne .L_0200ad98
	adds r4, r5, #0
	movs r7, #0
.L_0200adb0:
	ldr r2, .L_0200af64
	lsls r1, r7, #6
	adds r1, r1, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, #0
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r7, #1
	adds r4, #8
	cmp r7, #16
	bne .L_0200adb0
	adds r0, r5, #0
	bl Sys_Free
	movs r0, #246
	bl Func_02005f94
	movs r0, #11
	bl Object_GetById
	movs r3, #9
	adds r0, #98
	strb r3, [r0]
	movs r0, #11
	bl Object_GetById
	movs r3, #1
	adds r0, #99
	strb r3, [r0]
	movs r0, #11
	bl Object_GetById
	ldr r3, .L_0200af68
	movs r7, #0
	str r3, [r0, #108]
.L_0200ae00:
	ldr r3, .L_0200af6c
	ldr r3, [r3]
	mov r8, r3
	mov r0, r8
	movs r3, #3
	ands r0, r3
	mov r10, r3
	mov r8, r0
	cmp r0, #0
	bne .L_0200aeac
	movs r0, #11
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #11
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #11
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r0, #168
	ldr r2, [r5, #12]
	ldr r1, [r6, #8]
	lsls r0, r0, #2
	bl Func_02005d74
	adds r5, r0, #0
	movs r0, #11
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r4, [r5, #80]
	ldrb r3, [r3, #9]
	movs r1, #12
	ands r1, r3
	movs r0, #13
	ldrb r3, [r4, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	orrs r3, r1
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	strb r3, [r4, #9]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	mov r1, r8
	adds r3, #85
	strb r1, [r3]
	ldr r3, [r5, #8]
	str r3, [r5, #68]
	movs r3, #192
	lsls r3, r3, #9
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
	ldr r1, .L_0200af70
	bl Func_02005d6c
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetPartAttribute
	ldr r3, .L_0200af74
	str r3, [r5, #108]
.L_0200aeac:
	movs r0, #1
	adds r7, #1
	bl Battle_WaitMode0
	cmp r7, #45
	bne .L_0200ae00
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02005f0c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02005f04
	movs r0, #60
	bl Func_02005f14
	ldr r3, .L_0200af78
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02005ee4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02005ed4
	movs r0, #156
	movs r1, #1
	movs r2, #170
	lsls r2, r2, #18
	negs r1, r1
	movs r3, #1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02005ee4
	movs r0, #11
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #108]
	movs r0, #11
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	ldr r3, .L_0200af5c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200af60
	subs r2, #2
	strh r3, [r2]
	ldr r2, .L_0200af7c
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #152
	strh r3, [r2]
	movs r3, #42
	strh r3, [r2, #2]
	movs r0, #30
	bl WaitFrames
	movs r0, #138
	bl Func_02005f94
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #0
	bl Func_02005f0c
	movs r0, #254
	lsls r0, r0, #7
	b .L_0200af80
.L_0200af5c:
	.4byte 0x00001010
.L_0200af60:
	.4byte 0x00003f41
.L_0200af64:
	.4byte 0x06002000
.L_0200af68:
	.4byte Func_02002cc0
.L_0200af6c:
	.4byte Data_0300122c
.L_0200af70:
	.4byte Data_02005fd8
.L_0200af74:
	.4byte Func_02002c40
.L_0200af78:
	.4byte gPartyState
.L_0200af7c:
	.4byte Data_03001120
.L_0200af80:
	movs r1, #0
	adds r0, #255
	bl Func_02005f04
	movs r0, #1
	bl Func_02005f14
	movs r0, #1
	bl WaitFrames
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02005f0c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02005f04
	movs r0, #8
	bl Func_02005f14
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200aff4
	movs r0, #64
	orrs r3, r2
	strh r3, [r1]
	mov r3, r10
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r1, #78
	movs r3, #105
	movs r2, #18
	bl Func_02005d9c
	movs r0, #160
	movs r1, #224
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
	b .L_0200aff8
.L_0200aff4:
	.4byte 0x00000100
.L_0200aff8:
	movs r7, #0
.L_0200affa:
	ldr r2, .L_0200b01c
	lsrs r3, r7, #1
	subs r2, r2, r3
	ldr r3, .L_0200b020
	movs r6, #128
	lsls r6, r6, #19
	orrs r2, r3
	adds r6, #82
	strh r2, [r6]
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #32
	bne .L_0200affa
	b .L_0200b024
	.2byte 0x0000
.L_0200b01c:
	.4byte 0x00000010
.L_0200b020:
	.4byte 0x00001000
.L_0200b024:
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #0
	bl Func_02005f0c
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02005f04
	movs r0, #60
	bl Func_02005f14
	movs r0, #30
	bl WaitFrames
	movs r0, #148
	bl Func_02005f94
	movs r0, #156
	movs r1, #180
	lsls r0, r0, #17
	lsls r1, r1, #18
	bl Func_02002aa8
	movs r3, #3
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #105
	movs r2, #18
	movs r0, #64
	movs r1, #78
	bl Func_02005d9c
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02005ed4
	movs r1, #1
	movs r0, #10
	bl Func_02005eec
	bl Func_02005ee4
	movs r0, #146
	bl Func_02005f94
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02005e94
	movs r0, #10
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #10
	bl Object_GetById
	ldr r3, [r5, #8]
	ldr r1, [r0, #16]
	adds r0, r3, #0
	bl Func_02002b58
	movs r1, #0
	movs r0, #10
	bl Func_02005e94
	movs r0, #10
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #3
	bl WaitFrames
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #40
	bl WaitFrames
	movs r0, #10
	bl Object_GetById
	movs r1, #8
	movs r2, #0
	bl Func_02001084
	bl Func_02005d34
	bl Func_02005d2c
	ldr r3, .L_0200b160
	movs r0, #147
	movs r1, #128
	lsls r0, r0, #1
	lsls r1, r1, #2
	adds r0, #255
	adds r1, #38
	adds r2, r3, r0
	adds r3, r3, r1
	ldrb r0, [r2]
	ldrb r1, [r3]
	bl Func_02005e04
	ldr r3, .L_0200b154
	movs r2, #128
	strh r3, [r6]
	ldr r3, .L_0200b158
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200b15c
	movs r0, #128
	orrs r3, r2
	ldr r2, .L_0200b164
	strh r3, [r1]
	lsls r0, r0, #4
	movs r3, #0
	strh r3, [r2]
	strh r3, [r2, #2]
	adds r0, #154
	bl GameFlag_SetBit
	bl Func_02005e1c
	add sp, #12
	b .L_0200b168
.L_0200b154:
	.4byte 0x00001008
.L_0200b158:
	.4byte 0x00003f10
.L_0200b15c:
	.4byte 0x00000100
.L_0200b160:
	.4byte gPartyState
.L_0200b164:
	.4byte Data_03001120
.L_0200b168:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.section .text.x0200b170,"ax",%progbits
	.global Func_02003170
	.thumb_func
Func_02003170:
	push {r5, r6, r7, lr}
	ldr r3, .L_0200b250
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r0, [r7]
	sub sp, #8
	bl Object_GetById
	ldr r2, [r0, #16]
	ldr r1, [r0, #8]
	asrs r2, r2, #20
	asrs r1, r1, #20
	subs r2, #1
	movs r0, #1
	bl Func_02000690
	asrs r0, r0, #8
	cmp r0, #30
	bne .L_0200b24a
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	movs r0, #188
	bl Func_02005f94
	movs r5, #1
	movs r6, #2
	movs r1, #76
	movs r2, #19
	movs r3, #92
	movs r0, #65
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005d9c
	movs r0, #8
	bl Battle_WaitMode0
	movs r1, #76
	movs r2, #19
	movs r3, #92
	movs r0, #66
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005d9c
	movs r0, #8
	bl Battle_WaitMode0
	movs r1, #76
	movs r2, #19
	movs r3, #92
	movs r0, #67
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005d9c
	movs r0, #8
	bl Battle_WaitMode0
	movs r1, #76
	movs r2, #19
	movs r3, #92
	movs r0, #62
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005d9c
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, [r7]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r0, [r7]
	bl Object_GetById
	adds r5, r0, #0
	ldr r0, [r7]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #52]
	str r3, [r5, #48]
	movs r0, #123
	bl Func_02005f94
	movs r1, #156
	movs r2, #198
	lsls r1, r1, #1
	lsls r2, r2, #2
	ldr r0, [r7]
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #15
	bl Func_02005efc
	bl Func_02005e1c
.L_0200b24a:
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b250:
	.4byte gPartyState
	.section .text.x0200b254,"ax",%progbits
	.global Func_02003254
	.thumb_func
Func_02003254:
	push {lr}
	ldr r3, .L_0200b2f0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200b2f4
	cmp r2, r3
	bne .L_0200b270
	ldr r0, .L_0200b2f8
	bl Func_02005f74
	b .L_0200b2ee
.L_0200b270:
	ldr r3, .L_0200b2fc
	cmp r2, r3
	bne .L_0200b27e
	ldr r0, .L_0200b300
	bl Func_02005f74
	b .L_0200b2ee
.L_0200b27e:
	ldr r3, .L_0200b304
	cmp r2, r3
	bne .L_0200b28c
	ldr r0, .L_0200b308
	bl Func_02005f74
	b .L_0200b2ee
.L_0200b28c:
	ldr r3, .L_0200b30c
	cmp r2, r3
	bne .L_0200b29a
	ldr r0, .L_0200b310
	bl Func_02005f74
	b .L_0200b2ee
.L_0200b29a:
	ldr r3, .L_0200b314
	cmp r2, r3
	bne .L_0200b2c8
	movs r0, #152
	movs r1, #168
	lsls r0, r0, #18
	lsls r1, r1, #17
	movs r2, #2
	movs r3, #0
	bl Func_02005df4
	movs r0, #156
	movs r1, #168
	lsls r0, r0, #18
	lsls r1, r1, #17
	movs r2, #2
	movs r3, #0
	bl Func_02005df4
	ldr r0, .L_0200b318
	bl Func_02005f74
	b .L_0200b2ee
.L_0200b2c8:
	ldr r3, .L_0200b31c
	cmp r2, r3
	bne .L_0200b2d6
	ldr r0, .L_0200b320
	bl Func_02005f74
	b .L_0200b2ee
.L_0200b2d6:
	ldr r3, .L_0200b324
	cmp r2, r3
	bne .L_0200b2ee
	movs r0, #10
	bl Object_GetById
	movs r3, #0
	adds r0, #100
	strh r3, [r0]
	ldr r0, .L_0200b328
	bl Func_02005f74
.L_0200b2ee:
	pop {pc}
.L_0200b2f0:
	.4byte gPartyState
.L_0200b2f4:
	.4byte 0x00000055
.L_0200b2f8:
	.4byte Data_02006050
.L_0200b2fc:
	.4byte 0x00000056
.L_0200b300:
	.4byte Data_02006054
.L_0200b304:
	.4byte 0x00000057
.L_0200b308:
	.4byte Data_0200605a
.L_0200b30c:
	.4byte 0x00000058
.L_0200b310:
	.4byte Data_02006064
.L_0200b314:
	.4byte 0x0000005b
.L_0200b318:
	.4byte Data_02006068
.L_0200b31c:
	.4byte 0x0000005c
.L_0200b320:
	.4byte Data_02006072
.L_0200b324:
	.4byte 0x0000005d
.L_0200b328:
	.4byte Data_0200607e
	.section .text.x0200b32c,"ax",%progbits
	.global Func_0200332c
	.thumb_func
Func_0200332c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #188
	adds r3, r3, r1
	ldr r7, [r3]
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	bl Func_02005f7c
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #2
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	cmp r0, r3
	beq .L_0200b38c
	movs r2, #34
	adds r2, r2, r7
	movs r3, #2
	adds r6, r7, #0
	strb r3, [r2]
	adds r6, #85
	movs r3, #3
	strb r3, [r6]
	adds r0, r7, #0
	mov r8, r2
	bl Func_02000674
	movs r0, #188
	bl Func_02005f94
	adds r0, r7, #0
	bl Func_02000674
	movs r5, #0
	mov r3, r8
	strb r5, [r6]
	strb r5, [r3]
.L_0200b38c:
	ldr r3, [r7, #8]
	movs r1, #240
	asrs r5, r3, #19
	ldr r3, [r7, #16]
	lsls r1, r1, #1
	asrs r6, r3, #19
	ldr r3, .L_0200b634
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200b638
	cmp r2, r3
	bne .L_0200b3be
	cmp r5, #103
	beq .L_0200b3ac
	b .L_0200b628
.L_0200b3ac:
	cmp r6, #41
	beq .L_0200b3b2
	b .L_0200b628
.L_0200b3b2:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #142
	bl GameFlag_SetBit
	b .L_0200b628
.L_0200b3be:
	ldr r3, .L_0200b63c
	cmp r2, r3
	bne .L_0200b3f0
	cmp r5, #13
	bne .L_0200b3d8
	cmp r6, #49
	bne .L_0200b3d8
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #152
	bl GameFlag_SetBit
	b .L_0200b628
.L_0200b3d8:
	cmp r5, #41
	beq .L_0200b3de
	b .L_0200b628
.L_0200b3de:
	cmp r6, #49
	beq .L_0200b3e4
	b .L_0200b628
.L_0200b3e4:
	movs r0, #243
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	b .L_0200b628
.L_0200b3f0:
	ldr r3, .L_0200b640
	cmp r2, r3
	bne .L_0200b436
	cmp r5, #33
	bne .L_0200b40a
	cmp r6, #79
	bne .L_0200b40a
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #155
	bl GameFlag_SetBit
	b .L_0200b628
.L_0200b40a:
	cmp r5, #77
	bne .L_0200b41e
	cmp r6, #71
	bne .L_0200b41e
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #156
	bl GameFlag_SetBit
	b .L_0200b628
.L_0200b41e:
	cmp r5, #59
	beq .L_0200b424
	b .L_0200b628
.L_0200b424:
	cmp r6, #65
	beq .L_0200b42a
	b .L_0200b628
.L_0200b42a:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #157
	bl GameFlag_SetBit
	b .L_0200b628
.L_0200b436:
	ldr r3, .L_0200b644
	cmp r2, r3
	bne .L_0200b454
	cmp r5, #81
	beq .L_0200b442
	b .L_0200b628
.L_0200b442:
	cmp r6, #37
	beq .L_0200b448
	b .L_0200b628
.L_0200b448:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #158
	bl GameFlag_SetBit
	b .L_0200b628
.L_0200b454:
	ldr r3, .L_0200b648
	cmp r2, r3
	bne .L_0200b51c
	movs r0, #152
	movs r1, #168
	movs r3, #4
	lsls r0, r0, #18
	lsls r1, r1, #17
	movs r2, #2
	negs r3, r3
	bl Func_02005df4
	movs r0, #156
	movs r1, #168
	movs r3, #4
	lsls r0, r0, #18
	lsls r1, r1, #17
	movs r2, #2
	negs r3, r3
	bl Func_02005df4
	cmp r5, #43
	bne .L_0200b48e
	cmp r6, #87
	bne .L_0200b48e
	movs r0, #141
	lsls r0, r0, #4
	bl GameFlag_SetBit
.L_0200b48e:
	movs r0, #9
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #19
	cmp r3, #73
	bne .L_0200b4aa
	movs r0, #9
	bl Object_GetById
	ldr r3, [r0, #16]
	asrs r3, r3, #19
	cmp r3, #43
	beq .L_0200b4c6
.L_0200b4aa:
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #19
	cmp r3, #73
	bne .L_0200b4d0
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #16]
	asrs r3, r3, #19
	cmp r3, #43
	bne .L_0200b4d0
.L_0200b4c6:
	movs r0, #13
	bl Object_GetById
	movs r3, #1
	b .L_0200b4d8
.L_0200b4d0:
	movs r0, #13
	bl Object_GetById
	movs r3, #0
.L_0200b4d8:
	adds r0, #99
	strb r3, [r0]
	movs r0, #11
	bl Object_GetById
	ldr r2, [r7, #8]
	ldr r3, [r0, #8]
	cmp r2, r3
	beq .L_0200b4ec
	b .L_0200b628
.L_0200b4ec:
	movs r0, #11
	bl Object_GetById
	ldr r2, [r7, #16]
	ldr r3, [r0, #16]
	cmp r2, r3
	beq .L_0200b4fc
	b .L_0200b628
.L_0200b4fc:
	cmp r5, #83
	bne .L_0200b50e
	cmp r6, #43
	bne .L_0200b50e
	movs r0, #14
	bl Object_GetById
	movs r3, #1
	b .L_0200b516
.L_0200b50e:
	movs r0, #14
	bl Object_GetById
	movs r3, #0
.L_0200b516:
	adds r0, #99
	strb r3, [r0]
	b .L_0200b628
.L_0200b51c:
	ldr r3, .L_0200b64c
	cmp r2, r3
	bne .L_0200b5f2
	cmp r5, #35
	bne .L_0200b534
	cmp r6, #59
	bne .L_0200b534
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #209
	bl GameFlag_SetBit
.L_0200b534:
	cmp r5, #45
	bne .L_0200b546
	cmp r6, #105
	bne .L_0200b546
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #210
	bl GameFlag_SetBit
.L_0200b546:
	cmp r5, #23
	bne .L_0200b560
	cmp r6, #77
	bne .L_0200b560
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #153
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
.L_0200b560:
	cmp r6, #59
	bne .L_0200b57a
	cmp r5, #67
	beq .L_0200b570
	cmp r5, #71
	beq .L_0200b570
	cmp r5, #73
	bne .L_0200b57a
.L_0200b570:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_0200b57a:
	cmp r6, #19
	bne .L_0200b590
	cmp r5, #75
	beq .L_0200b586
	cmp r5, #79
	bne .L_0200b590
.L_0200b586:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
.L_0200b590:
	cmp r5, #69
	bne .L_0200b5a2
	cmp r6, #59
	bne .L_0200b5a2
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200b5a2:
	cmp r5, #77
	bne .L_0200b5b4
	cmp r6, #19
	bne .L_0200b5b4
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
.L_0200b5b4:
	movs r0, #8
	bl Object_GetById
	ldr r5, [r0, #8]
	movs r0, #8
	bl Object_GetById
	ldr r2, [r0, #16]
	asrs r5, r5, #20
	adds r1, r5, #0
	asrs r2, r2, #20
	movs r3, #0
	movs r0, #2
	bl Func_0200062c
	movs r0, #9
	bl Object_GetById
	ldr r5, [r0, #8]
	movs r0, #9
	bl Object_GetById
	ldr r2, [r0, #16]
	asrs r5, r5, #20
	asrs r2, r2, #20
	movs r0, #2
	adds r1, r5, #0
	movs r3, #0
	bl Func_0200062c
	b .L_0200b628
.L_0200b5f2:
	ldr r3, .L_0200b650
	cmp r2, r3
	bne .L_0200b628
	cmp r5, #45
	bne .L_0200b60a
	cmp r6, #33
	bne .L_0200b60a
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #211
	bl GameFlag_SetBit
.L_0200b60a:
	movs r0, #10
	bl Object_GetById
	ldr r5, [r0, #8]
	movs r0, #10
	bl Object_GetById
	ldr r2, [r0, #16]
	asrs r5, r5, #20
	asrs r2, r2, #20
	movs r0, #2
	adds r1, r5, #0
	movs r3, #0
	bl Func_0200062c
.L_0200b628:
	bl Func_02005e1c
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b634:
	.4byte gPartyState
.L_0200b638:
	.4byte 0x00000055
.L_0200b63c:
	.4byte 0x00000056
.L_0200b640:
	.4byte 0x00000057
.L_0200b644:
	.4byte 0x00000058
.L_0200b648:
	.4byte 0x0000005b
.L_0200b64c:
	.4byte 0x0000005c
.L_0200b650:
	.4byte 0x0000005d
	.section .text.x0200b654,"ax",%progbits
	.global Func_02003654
	.thumb_func
Func_02003654:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #128
	lsls r0, r0, #4
	sub sp, #12
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #254
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	movs r2, #128
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #3
	adds r3, #8
	strh r2, [r3]
	ldr r0, .L_0200b6e8
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_02005d24
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #192
	adds r3, #212
	adds r0, r5, #0
	lsls r1, r1, #19
	ldr r2, .L_0200b6ec
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r5, #0
	bl Sys_Free
	ldr r3, .L_0200b6e4
	mov r0, sp
	adds r0, #10
	strh r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r1, .L_0200b6f0
	ldr r2, .L_0200b6f4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	ldr r0, .L_0200b6f8
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_02005d24
	movs r7, #0
	adds r4, r5, #0
	b .L_0200b6fc
	.2byte 0x0000
.L_0200b6e4:
	.4byte 0x00000000
.L_0200b6e8:
	.4byte 0x000001c0
.L_0200b6ec:
	.4byte 0x84000200
.L_0200b6f0:
	.4byte 0x06002000
.L_0200b6f4:
	.4byte 0x81000400
.L_0200b6f8:
	.4byte 0x000001c1
.L_0200b6fc:
	ldrh r2, [r4]
	movs r3, #252
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	adds r7, #1
	strh r3, [r4]
	adds r4, #2
	cmp r7, #64
	bne .L_0200b6fc
	adds r4, r5, #0
	movs r7, #0
.L_0200b714:
	ldr r2, .L_0200b8c8
	lsls r1, r7, #6
	adds r1, r1, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, #0
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r7, #1
	adds r4, #8
	cmp r7, #16
	bne .L_0200b714
	adds r0, r5, #0
	bl Sys_Free
	movs r0, #246
	bl Func_02005f94
	movs r0, #10
	bl Object_GetById
	movs r3, #9
	adds r0, #98
	strb r3, [r0]
	movs r0, #10
	bl Object_GetById
	movs r3, #1
	adds r0, #99
	strb r3, [r0]
	movs r0, #10
	bl Object_GetById
	ldr r3, .L_0200b8cc
	movs r7, #0
	str r3, [r0, #108]
.L_0200b764:
	ldr r3, .L_0200b8d0
	ldr r3, [r3]
	mov r8, r3
	mov r0, r8
	movs r3, #3
	ands r0, r3
	mov r10, r3
	mov r8, r0
	cmp r0, #0
	bne .L_0200b810
	movs r0, #10
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r0, #168
	ldr r2, [r5, #12]
	ldr r1, [r6, #8]
	lsls r0, r0, #2
	bl Func_02005d74
	adds r5, r0, #0
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r4, [r5, #80]
	ldrb r3, [r3, #9]
	movs r1, #12
	ands r1, r3
	movs r0, #13
	ldrb r3, [r4, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	orrs r3, r1
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	strb r3, [r4, #9]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	mov r1, r8
	adds r3, #85
	strb r1, [r3]
	ldr r3, [r5, #8]
	str r3, [r5, #68]
	movs r3, #192
	lsls r3, r3, #9
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
	ldr r1, .L_0200b8d4
	bl Func_02005d6c
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetPartAttribute
	ldr r3, .L_0200b8d8
	str r3, [r5, #108]
.L_0200b810:
	movs r0, #1
	adds r7, #1
	bl Battle_WaitMode0
	cmp r7, #45
	bne .L_0200b764
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02005f0c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02005f04
	movs r0, #60
	bl Func_02005f14
	ldr r3, .L_0200b8dc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02005ee4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02005ed4
	movs r0, #216
	movs r1, #1
	movs r2, #224
	lsls r2, r2, #16
	negs r1, r1
	movs r3, #1
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_02005ee4
	movs r0, #10
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #108]
	movs r0, #10
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	ldr r3, .L_0200b8c0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200b8c4
	subs r2, #2
	strh r3, [r2]
	ldr r2, .L_0200b8e0
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #152
	strh r3, [r2]
	movs r3, #42
	strh r3, [r2, #2]
	movs r0, #30
	bl WaitFrames
	movs r0, #138
	bl Func_02005f94
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #0
	bl Func_02005f0c
	movs r0, #254
	lsls r0, r0, #7
	b .L_0200b8e4
.L_0200b8c0:
	.4byte 0x00001010
.L_0200b8c4:
	.4byte 0x00003f41
.L_0200b8c8:
	.4byte 0x06002000
.L_0200b8cc:
	.4byte Func_02002cc0
.L_0200b8d0:
	.4byte Data_0300122c
.L_0200b8d4:
	.4byte Data_02005fd8
.L_0200b8d8:
	.4byte Func_02002c40
.L_0200b8dc:
	.4byte gPartyState
.L_0200b8e0:
	.4byte Data_03001120
.L_0200b8e4:
	movs r1, #0
	adds r0, #255
	bl Func_02005f04
	movs r0, #1
	bl Func_02005f14
	movs r0, #1
	bl WaitFrames
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02005f0c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02005f04
	movs r0, #8
	bl Func_02005f14
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200b958
	movs r0, #68
	orrs r3, r2
	strh r3, [r1]
	mov r3, r10
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r1, #84
	movs r3, #77
	movs r2, #12
	bl Func_02005d9c
	movs r0, #160
	movs r1, #224
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
	b .L_0200b95c
.L_0200b958:
	.4byte 0x00000100
.L_0200b95c:
	movs r7, #0
.L_0200b95e:
	ldr r2, .L_0200b980
	lsrs r3, r7, #1
	subs r2, r2, r3
	ldr r3, .L_0200b984
	movs r6, #128
	lsls r6, r6, #19
	orrs r2, r3
	adds r6, #82
	strh r2, [r6]
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #32
	bne .L_0200b95e
	b .L_0200b988
	.2byte 0x0000
.L_0200b980:
	.4byte 0x00000010
.L_0200b984:
	.4byte 0x00001000
.L_0200b988:
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #0
	bl Func_02005f0c
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02005f04
	movs r0, #60
	bl Func_02005f14
	movs r0, #30
	bl WaitFrames
	movs r0, #148
	bl Func_02005f94
	movs r0, #216
	lsls r0, r0, #16
	ldr r1, .L_0200bab0
	bl Func_02002aa8
	movs r3, #3
	movs r2, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #77
	movs r2, #12
	movs r0, #68
	movs r1, #84
	bl Func_02005d9c
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02005ed4
	movs r1, #1
	movs r0, #8
	bl Func_02005eec
	bl Func_02005ee4
	movs r0, #146
	bl Func_02005f94
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02005e94
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #8
	bl Object_GetById
	ldr r1, [r0, #16]
	movs r0, #128
	lsls r0, r0, #14
	adds r1, r1, r0
	ldr r0, [r5, #8]
	bl Func_02002b58
	movs r1, #0
	movs r0, #8
	bl Func_02005e94
	movs r0, #8
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #3
	bl WaitFrames
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #40
	bl WaitFrames
	movs r0, #8
	bl Object_GetById
	movs r1, #8
	movs r2, #0
	bl Func_02001084
	bl Func_02005d34
	bl Func_02005d2c
	ldr r3, .L_0200bab4
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3]
	bl Func_02005e04
	ldr r3, .L_0200baa4
	movs r2, #128
	strh r3, [r6]
	ldr r3, .L_0200baa8
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200baac
	movs r0, #128
	orrs r3, r2
	ldr r2, .L_0200bab8
	strh r3, [r1]
	lsls r0, r0, #4
	movs r3, #0
	strh r3, [r2]
	strh r3, [r2, #2]
	adds r0, #153
	bl GameFlag_SetBit
	bl Func_02005e1c
	add sp, #12
	b .L_0200babc
	.2byte 0x0000
.L_0200baa4:
	.4byte 0x00001008
.L_0200baa8:
	.4byte 0x00003f10
.L_0200baac:
	.4byte 0x00000100
.L_0200bab0:
	.4byte 0x012d0000
.L_0200bab4:
	.4byte gPartyState
.L_0200bab8:
	.4byte Data_03001120
.L_0200babc:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.section .text.x0200bac4,"ax",%progbits
	.global Func_02003ac4
	.thumb_func
Func_02003ac4:
	push {lr}
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	movs r0, #216
	movs r2, #136
	movs r3, #143
	ldr r1, .L_0200bb00
	lsls r2, r2, #18
	lsls r3, r3, #1
	lsls r0, r0, #16
	bl Func_02000218
	ldr r3, .L_0200bb04
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #6
	movs r2, #0
	bl Func_02005e84
	movs r0, #3
	bl Func_02005efc
	bl Func_02005e1c
	pop {pc}
.L_0200bb00:
	.4byte 0xffe00000
.L_0200bb04:
	.4byte gPartyState
	.section .text.x0200bb08,"ax",%progbits
	.global Func_02003b08
	.thumb_func
Func_02003b08:
	push {r5, lr}
	ldr r3, .L_0200bb78
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	movs r2, #18
	ldrsh r3, [r0, r2]
	movs r2, #199
	lsls r2, r2, #1
	adds r2, #255
	cmp r3, r2
	bgt .L_0200bb76
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	movs r0, #186
	bl Func_02005f94
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	movs r1, #188
	movs r2, #167
	lsls r2, r2, #2
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	bl Func_02005e1c
.L_0200bb76:
	pop {r5, pc}
.L_0200bb78:
	.4byte gPartyState
	.section .text.x0200bb7c,"ax",%progbits
	.global Func_02003b7c
	.thumb_func
Func_02003b7c:
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
	bl Func_02005d74
	ldr r3, .L_0200bbd0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200bbd4
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
	bl Func_02005e14
	movs r0, #0
	b .L_0200bbd8
.L_0200bbd0:
	.4byte 0x00000010
.L_0200bbd4:
	.4byte 0x00003f44
.L_0200bbd8:
	bl Func_02005f2c
	ldr r3, .L_0200bc14
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #74
	strh r3, [r2]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200bc18
	movs r0, #106
	orrs r3, r2
	strh r3, [r1]
	bl Func_02005f94
	movs r5, #0
.L_0200bbfa:
	ldr r1, .L_0200bc1c
	movs r3, #128
	lsls r2, r5, #9
	lsls r3, r3, #19
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	b .L_0200bc20
	.2byte 0x0000
.L_0200bc14:
	.4byte 0x00003f1f
.L_0200bc18:
	.4byte 0x00008000
.L_0200bc1c:
	.4byte 0x00000010
.L_0200bc20:
	cmp r5, #8
	bne .L_0200bbfa
	movs r5, #0
.L_0200bc26:
	ldr r2, .L_0200bc44
	ldr r1, .L_0200bc48
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
	bne .L_0200bc26
	b .L_0200bc4c
.L_0200bc44:
	.4byte 0x00000010
.L_0200bc48:
	.4byte 0x00001000
.L_0200bc4c:
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
	bl Func_02005d9c
	movs r3, #255
	adds r1, r6, #0
	adds r2, r5, #0
	lsls r3, r3, #8
	movs r0, #0
	bl Func_0200062c
	adds r0, r7, #0
	bl Func_02005d7c
	bl Func_02005e1c
	ldr r3, .L_0200bca0
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
	b .L_0200bca4
.L_0200bca0:
	.4byte 0x00000000
.L_0200bca4:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200bcac,"ax",%progbits
	.global Func_02003cac
	.thumb_func
Func_02003cac:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	ldr r3, .L_0200bd3c
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
	bl Func_02000690
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
	beq .L_0200bd20
	cmp r0, #7
	beq .L_0200bd20
	bl Func_02002790
	movs r0, #132
	lsls r1, r6, #3
	lsls r0, r0, #1
	adds r1, #10
	bl Func_02003b7c
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bd10
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_0200bd10:
	movs r3, #0
	ldrsh r0, [r5, r3]
	movs r2, #156
	lsls r2, r2, #1
	adds r0, r0, r2
	bl GameFlag_SetBit
	b .L_0200bd38
.L_0200bd20:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bd38
	movs r0, #132
	lsls r1, r6, #3
	lsls r0, r0, #1
	adds r1, #10
	bl Func_02003b7c
.L_0200bd38:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200bd3c:
	.4byte gPartyState
	.section .text.x0200bd40,"ax",%progbits
	.global Func_02003d40
	.thumb_func
Func_02003d40:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	bl Func_02005f84
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r4, r3, #20
	ldr r3, [r0, #12]
	asrs r7, r3, #20
	ldr r3, [r0, #16]
	ldr r0, .L_0200bdc0
	asrs r1, r3, #20
	lsls r3, r4, #16
	adds r2, r3, r1
	ldr r3, [r0]
	cmp r2, r3
	beq .L_0200bdb4
	str r2, [r0]
	ldr r2, .L_0200bdc4
	movs r3, #1
	str r3, [r2]
	ldr r3, .L_0200bdc8
	ldr r6, .L_0200bdcc
	mov r8, r3
	ldr r5, .L_0200bdd0
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
	bl Func_02005db4
	mov r3, r10
	mov r1, r8
	ldr r2, [r3]
	ldr r0, [r5]
	ldr r3, [r1]
	ldr r1, [r6]
	str r0, [sp, #4]
	str r1, [sp, #0]
	movs r0, #70
	movs r1, #74
	bl Func_02005db4
.L_0200bdb4:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200bdc0:
	.4byte Data_02006c0c
.L_0200bdc4:
	.4byte gOverlayArea + 0x73dc
.L_0200bdc8:
	.4byte gOverlayArea + 0x73e0
.L_0200bdcc:
	.4byte gOverlayArea + 0x73e4
.L_0200bdd0:
	.4byte gOverlayArea + 0x73e8
	.section .text.x0200bdd4,"ax",%progbits
	.global Func_02003dd4
	.thumb_func
Func_02003dd4:
	push {r5, r6, lr}
	sub sp, #8
	movs r3, #8
	str r3, [sp, #0]
	movs r5, #19
	movs r0, #26
	movs r1, #17
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02005dac
	movs r3, #24
	str r3, [sp, #0]
	movs r0, #26
	movs r1, #17
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02005dac
	movs r5, #1
	movs r6, #2
	movs r0, #90
	movs r1, #17
	movs r2, #72
	movs r3, #19
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005d9c
	movs r0, #90
	movs r1, #17
	movs r2, #88
	movs r3, #19
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005d9c
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200be28,"ax",%progbits
	.global Func_02003e28
	.thumb_func
Func_02003e28:
	push {r5, r6, lr}
	sub sp, #8
	movs r3, #8
	str r3, [sp, #0]
	movs r5, #19
	movs r0, #25
	movs r1, #17
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02005dac
	movs r3, #24
	str r3, [sp, #0]
	movs r0, #25
	movs r1, #17
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02005dac
	movs r5, #1
	movs r6, #2
	movs r0, #89
	movs r1, #17
	movs r2, #72
	movs r3, #19
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005d9c
	movs r0, #89
	movs r1, #17
	movs r2, #88
	movs r3, #19
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005d9c
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200be7c,"ax",%progbits
	.global Func_02003e7c
	.thumb_func
Func_02003e7c:
	push {r5, lr}
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	movs r0, #168
	lsls r0, r0, #2
	bl Func_02005d74
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
	movs r1, #2
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetSpritePriority
	adds r0, r5, #0
	movs r1, #1
	bl Func_02005d5c
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	ldr r1, .L_0200bed0
	adds r0, r5, #0
	bl Func_02005d6c
	pop {r5, pc}
.L_0200bed0:
	.4byte Data_02006084
	.section .text.x0200bed4,"ax",%progbits
	.global Func_02003ed4
	.thumb_func
Func_02003ed4:
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
	beq .L_0200bf84
	ldr r3, [r6, #76]
	movs r2, #128
	lsls r2, r2, #10
	movs r0, #1
	cmp r3, r2
	beq .L_0200bf84
	adds r0, r6, #0
	bl Func_02003e7c
	ldr r2, .L_0200bf90
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
	bl Func_02003e7c
	ldr r3, [r6, #76]
	movs r0, #1
	add r3, r9
	str r3, [r6, #76]
.L_0200bf84:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200bf90:
	.4byte 0xfffc0000
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
	.4byte Data_0300122c
	.section .text.x0200c038,"ax",%progbits
	.global Func_02004038
	.thumb_func
Func_02004038:
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
	.section .text.x0200c084,"ax",%progbits
	.global Func_02004084
	.thumb_func
Func_02004084:
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
	.4byte Data_0300122c
	.section .text.x0200c138,"ax",%progbits
	.global Func_02004138
	.thumb_func
Func_02004138:
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
	bl Func_02005f94
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl Func_02005ed4
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
	bl Func_02005f94
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl Func_02005dcc
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02005f0c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_02005f04
	movs r0, #60
	bl Func_02005f14
	adds r2, r7, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	str r3, [r7, #52]
	str r3, [r7, #48]
	ldr r1, .L_0200c20c
	mov r0, r9
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #194
	bl Func_02005f94
	movs r0, #45
	bl Battle_WaitMode0
	movs r1, #128
	mov r0, r9
	lsls r1, r1, #1
	bl Func_02005e94
	ldr r3, .L_0200c204
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200c208
	subs r2, #2
	strh r3, [r2]
	movs r0, #0
	mov r8, r0
	b .L_0200c210
	.2byte 0x0000
.L_0200c204:
	.4byte 0x00001008
.L_0200c208:
	.4byte 0x00003f10
.L_0200c20c:
	.4byte Data_020060e0
.L_0200c210:
	movs r0, #168
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	lsls r0, r0, #2
	bl Func_02005d74
	adds r5, r0, #0
	movs r0, #246
	bl Func_02005f94
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
	movs r1, #2
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetSpritePriority
	adds r0, r5, #0
	movs r1, #7
	bl Func_02005d5c
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	ldr r1, .L_0200c2d0
	adds r0, r5, #0
	bl Func_02005d6c
	movs r6, #128
	ldr r3, .L_0200c2c4
	ldr r5, .L_0200c2c8
	lsls r6, r6, #19
	adds r6, #82
	strh r3, [r6]
	movs r0, #2
	bl WaitFrames
	movs r0, #2
	strh r5, [r6]
	bl WaitFrames
	ldr r3, .L_0200c2cc
	movs r0, #2
	strh r3, [r6]
	bl WaitFrames
	strh r5, [r6]
	movs r0, #2
	bl WaitFrames
	movs r2, #1
	add r8, r2
	mov r3, r8
	b .L_0200c2d4
.L_0200c2c4:
	.4byte 0x00001004
.L_0200c2c8:
	.4byte 0x0000100a
.L_0200c2cc:
	.4byte 0x00001010
.L_0200c2d0:
	.4byte Data_02006094
.L_0200c2d4:
	cmp r3, #16
	bne .L_0200c210
	ldr r3, .L_0200c314
	movs r0, #30
	strh r3, [r6]
	bl WaitFrames
	movs r0, #0
	mov r8, r0
.L_0200c2e6:
	mov r0, r10
	ldr r3, [r0, #16]
	mov r2, r10
	movs r0, #168
	ldr r1, [r2, #8]
	lsls r0, r0, #2
	ldr r2, [r2, #12]
	bl Func_02005d74
	adds r5, r0, #0
	movs r0, #195
	bl Func_02005f94
	bl Random16Far
	ldr r3, .L_0200c318
	movs r2, #128
	ands r0, r3
	lsls r2, r2, #8
	adds r6, r5, #0
	adds r0, r0, r2
	adds r6, #100
	b .L_0200c31c
.L_0200c314:
	.4byte 0x00001008
.L_0200c318:
	.4byte 0x00007fff
.L_0200c31c:
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
	ldr r7, .L_0200c370
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
	movs r1, #2
	strb r3, [r2]
	adds r0, r5, #0
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetSpritePriority
	adds r0, r5, #0
	movs r1, #7
	b .L_0200c374
.L_0200c370:
	.4byte 0x00000000
.L_0200c374:
	bl Func_02005d5c
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
	ldr r1, .L_0200c46c
	bl Func_02005d6c
	mov r0, r8
	cmp r0, #3
	bne .L_0200c3c8
	movs r1, #128
	mov r0, r11
	lsls r1, r1, #1
	bl Func_02005e94
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
	ldr r1, .L_0200c470
	bl ObjectMotion_EnableActionAndSetCallback
.L_0200c3c8:
	movs r0, #8
	bl WaitFrames
	movs r3, #1
	add r8, r3
	mov r0, r8
	cmp r0, #16
	beq .L_0200c3da
	b .L_0200c2e6
.L_0200c3da:
	movs r0, #220
	bl Func_02005f94
	movs r0, #16
	bl WaitFrames
	movs r2, #2
	mov r8, r2
.L_0200c3ea:
	mov r3, r10
	ldr r1, [r3, #8]
	ldr r3, [r3, #12]
	mov r0, r8
	lsls r2, r0, #16
	adds r2, r2, r3
	ldr r3, .L_0200c474
	mov r0, r10
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #168
	lsls r0, r0, #2
	bl Func_02005d74
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
	ldr r1, .L_0200c468
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
	movs r1, #2
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #7
	bl Func_02005d5c
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	b .L_0200c478
.L_0200c468:
	.4byte 0x00000000
.L_0200c46c:
	.4byte Data_020060b0
.L_0200c470:
	.4byte Data_02006104
.L_0200c474:
	.4byte 0xfff80000
.L_0200c478:
	adds r0, r5, #0
	ldr r1, .L_0200c524
	bl Func_02005d6c
	movs r0, #2
	add r8, r0
	mov r2, r8
	cmp r2, #32
	bne .L_0200c3ea
	movs r0, #220
	bl Func_02005f94
	movs r0, #50
	bl WaitFrames
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02005f04
	movs r0, #8
	bl Func_02005f14
	movs r0, #16
	bl WaitFrames
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Func_02005dcc
	mov r0, r9
	movs r1, #0
	bl Func_02005e94
	mov r0, r11
	movs r1, #0
	bl Func_02005e94
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #62
	bl Func_02005f94
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02005f04
	movs r0, #80
	bl Func_02005f14
	mov r0, r9
	ldr r1, .L_0200c528
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200c52c
	mov r0, r11
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r3, .L_0200c530
	mov r0, r10
	str r3, [r0, #108]
	movs r0, #120
	bl WaitFrames
	mov r2, r10
	movs r0, #195
	str r6, [r2, #108]
	lsls r0, r0, #1
	bl Func_02005f94
	bl Func_02005f3c
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
.L_0200c524:
	.4byte Data_020060c0
.L_0200c528:
	.4byte Data_02006128
.L_0200c52c:
	.4byte Data_02006158
.L_0200c530:
	.4byte Func_020040b0
	.section .text.x0200c534,"ax",%progbits
	.global Func_02004534
	.thumb_func
Func_02004534:
	push {r5, r6, lr}
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	ldr r6, .L_0200c734
	movs r1, #1
	adds r0, r6, #0
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #212
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c55e
	bl Func_02005e1c
	b .L_0200c730
.L_0200c55e:
	adds r0, r6, #0
	bl Func_02005ea4
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	ldr r5, .L_0200c738
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
	movs r0, #6
	adds r1, #51
	bl ObjectMotion_SetSpeedParameters
	ldr r1, [r5]
	movs r0, #6
	bl Func_02005e74
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
	movs r0, #6
	bl ObjectMotion_ResetAndSetPositionInMode2
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	bl Func_02005eac
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
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #6
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	movs r0, #6
	lsls r1, r1, #1
	movs r2, #137
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r6, #2
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #160
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02005dcc
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02005dcc
	bl Func_02005dd4
	movs r0, #8
	movs r1, #6
	bl Func_02004138
	movs r1, #144
	movs r0, #6
	bl UiText_DrawQuantityPairWithCue
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #212
	bl GameFlag_SetBit
	movs r2, #0
	movs r1, #6
	ldr r0, [r5]
	bl Object_LinkPair
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200c73c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200c710
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_0200c710:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #6
	bl Func_02005e6c
	movs r0, #8
	bl Object_GetById
	movs r1, #9
	bl Object_SetActionCallback
	bl Func_02005e1c
.L_0200c730:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200c734:
	.4byte 0x00001a8f
.L_0200c738:
	.4byte gPartyState
.L_0200c73c:
	.4byte 0x00013333
	.section .text.x0200c740,"ax",%progbits
	.global Func_02004740
	.thumb_func
Func_02004740:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #212
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c768
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	ldr r0, .L_0200c7a8
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02005e1c
	b .L_0200c7a6
.L_0200c768:
	ldr r5, .L_0200c7ac
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
	bl Func_02004534
.L_0200c7a6:
	pop {r5, pc}
.L_0200c7a8:
	.4byte 0x00001a8f
.L_0200c7ac:
	.4byte gPartyState
	.section .text.x0200c7b0,"ax",%progbits
	.global Func_020047b0
	.thumb_func
Func_020047b0:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #212
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c7d8
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	ldr r0, .L_0200c810
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02005e1c
	b .L_0200c80e
.L_0200c7d8:
	ldr r5, .L_0200c814
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
	bl Func_02004534
.L_0200c80e:
	pop {r5, pc}
.L_0200c810:
	.4byte 0x00001a8f
.L_0200c814:
	.4byte gPartyState
	.section .text.x0200c818,"ax",%progbits
	.global Func_02004818
	.thumb_func
Func_02004818:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #212
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c840
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	ldr r0, .L_0200c878
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02005e1c
	b .L_0200c874
.L_0200c840:
	ldr r5, .L_0200c87c
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
	bl Func_02004534
.L_0200c874:
	pop {r5, pc}
	.2byte 0x0000
.L_0200c878:
	.4byte 0x00001a8f
.L_0200c87c:
	.4byte gPartyState
	.global Data_02004880
Data_02004880:
	.4byte 0x00004770
	.section .text.x0200c884,"ax",%progbits
	.global Func_02004884
	.thumb_func
Func_02004884:
	push {lr}
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #19
	cmp r3, #65
	bne .L_0200c8aa
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_SetBit
	b .L_0200c8b4
.L_0200c8aa:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200c8b4:
	bl Func_02005e1c
	pop {pc}
	.2byte 0x0000
	.section .text.x0200c8bc,"ax",%progbits
	.global Func_020048bc
	.thumb_func
Func_020048bc:
	push {lr}
	bl Func_020007b4
	pop {pc}
	.section .text.x0200c8c4,"ax",%progbits
	.global Func_020048c4
	.thumb_func
Func_020048c4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0200ca98
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #0
	ldr r0, [r3]
	mov r8, r1
	sub sp, #4
	bl Object_GetById
	ldr r6, .L_0200ca9c
	movs r1, #255
	ldrh r3, [r6]
	lsls r1, r1, #8
	adds r1, #255
	movs r7, #0
	mov r9, r0
	cmp r3, r1
	beq .L_0200c938
.L_0200c8f4:
	ldrh r5, [r6]
	adds r0, r5, #0
	bl Object_GetById
	mov r1, r9
	ldr r2, [r0, #8]
	ldr r3, [r1, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	mov r8, r0
	cmp r2, r3
	bne .L_0200c92a
	ldr r2, [r0, #16]
	ldr r3, [r1, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_0200c92a
	movs r0, #1
	ands r0, r5
	lsls r0, r0, #1
	subs r0, r5, r0
	adds r0, #1
	bl Object_GetById
	adds r7, r0, #0
	b .L_0200c938
.L_0200c92a:
	adds r6, #6
	movs r2, #255
	ldrh r3, [r6]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200c8f4
.L_0200c938:
	mov r3, r8
	adds r1, r7, #0
	adds r3, #99
	adds r1, #99
	ldrb r2, [r3]
	ldrb r3, [r1]
	adds r1, r3, #0
	orrs r1, r2
	mov r10, r1
	cmp r1, #0
	beq .L_0200c950
	b .L_0200ca8c
.L_0200c950:
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	adds r3, r7, #0
	mov r1, r10
	mov r2, r8
	adds r3, #85
	ldr r6, [r2, #80]
	ldr r5, [r7, #80]
	strb r1, [r3]
	ldr r3, [r7, #12]
	ldr r2, .L_0200caa0
	movs r0, #1
	adds r3, r3, r2
	str r3, [r7, #12]
	bl WaitFrames
	ldrb r3, [r5, #16]
	ldr r1, .L_0200caa4
	lsls r3, r3, #2
	adds r4, r3, r1
	ldrh r3, [r4, #2]
	ldr r2, .L_0200caa8
	mov r0, sp
	adds r3, r3, r2
	str r3, [r7, #104]
	ldrb r3, [r6, #16]
	lsls r3, r3, #2
	adds r4, r3, r1
	ldrh r3, [r4, #2]
	mov r1, r8
	adds r3, r3, r2
	mov r2, r10
	str r2, [r0]
	ldrh r2, [r4]
	str r3, [r1, #104]
	movs r4, #133
	movs r3, #128
	lsls r4, r4, #24
	lsrs r2, r2, #2
	lsls r3, r3, #19
	adds r3, #212
	ldr r1, [r7, #104]
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	mov r1, r8
	lsls r2, r2, #24
	ldr r0, [r1, #104]
	adds r2, #192
	ldr r1, [r7, #104]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r2, r8
	ldr r3, [r2, #104]
	movs r2, #192
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r1, r8
	str r3, [r1, #104]
	ldr r3, [r7, #104]
	mov r6, r9
	adds r3, r3, r2
	adds r6, #85
	mov r2, r10
	str r3, [r7, #104]
	strb r2, [r6]
	movs r5, #0
.L_0200c9de:
	cmp r5, #4
	bhi .L_0200ca12
	ldr r3, [r7, #12]
	ldr r1, .L_0200caac
	mov r2, r8
	adds r3, r3, r1
	str r3, [r7, #12]
	ldr r0, [r2, #104]
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r1, [r7, #104]
	adds r2, #48
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r1, r8
	ldr r3, [r1, #104]
	movs r2, #128
	lsls r2, r2, #1
	adds r3, r3, r2
	str r3, [r1, #104]
	ldr r3, [r7, #104]
	adds r3, r3, r2
	str r3, [r7, #104]
.L_0200ca12:
	mov r2, r9
	ldr r3, [r2, #12]
	ldr r1, .L_0200cab0
	movs r0, #1
	adds r3, r3, r1
	str r3, [r2, #12]
	str r3, [r2, #20]
	mov r2, r8
	ldr r3, [r2, #28]
	ldr r1, .L_0200cab4
	adds r5, #1
	adds r3, r3, r1
	str r3, [r2, #28]
	bl WaitFrames
	cmp r5, #8
	bne .L_0200c9de
	movs r3, #3
	strb r3, [r6]
	movs r3, #128
	mov r2, r8
	lsls r3, r3, #9
	str r3, [r2, #28]
	ldr r3, [r7, #20]
	str r3, [r7, #12]
	mov r3, r8
	adds r3, #100
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl GameFlag_SetBit
	adds r3, r7, #0
	adds r3, #100
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl GameFlag_ClearBit
	mov r1, r8
	ldr r3, [r1, #80]
	mov r0, r8
	ldrb r1, [r3, #24]
	adds r1, #1
	bl Func_02005d5c
	ldr r3, [r7, #80]
	adds r0, r7, #0
	ldrb r1, [r3, #24]
	subs r1, #1
	bl Func_02005d5c
	mov r0, r8
	bl Func_020006cc
	adds r0, r7, #0
	bl Func_020006cc
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02005e1c
.L_0200ca8c:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200ca98:
	.4byte gPartyState
.L_0200ca9c:
	.4byte Data_0200621c
.L_0200caa0:
	.4byte 0xffe40000
.L_0200caa4:
	.4byte ResourceTableEntries
.L_0200caa8:
	.4byte 0x06010000
.L_0200caac:
	.4byte 0x00053333
.L_0200cab0:
	.4byte 0xfffc8000
.L_0200cab4:
	.4byte 0xffffe100
	.section .text.x0200cab8,"ax",%progbits
	.global Func_02004ab8
	.thumb_func
Func_02004ab8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #170
	lsls r1, r1, #1
	adds r1, r1, r3
	movs r2, #0
	ldrsh r3, [r1, r2]
	movs r7, #0
	movs r6, #0
	mov r8, r1
	cmp r3, #0
	beq .L_0200cba6
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	movs r0, #186
	bl Func_02005f94
	ldr r5, .L_0200cbac
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #10
	ldr r0, [r5]
	bl ObjectMotion_SetSpeedParameters
	mov r2, r8
	movs r1, #0
	ldrsh r3, [r2, r1]
	subs r3, #50
	cmp r3, #5
	bhi .L_0200cb80
	ldr r2, .L_0200cbb0
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0200cb24:
	.4byte .L_0200cb3c
	.4byte .L_0200cb58
	.4byte .L_0200cb62
	.4byte .L_0200cb6c
	.4byte .L_0200cb76
	.4byte .L_0200cb7a
.L_0200cb3c:
	ldr r5, .L_0200cbac
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r6, #158
	ldr r0, [r5]
	bl Object_GetById
	movs r7, #148
	lsls r6, r6, #2
	b .L_0200cb80
.L_0200cb58:
	movs r7, #132
	movs r6, #163
	lsls r7, r7, #1
	lsls r6, r6, #2
	b .L_0200cb80
.L_0200cb62:
	movs r7, #252
	movs r6, #254
	lsls r7, r7, #1
	lsls r6, r6, #1
	b .L_0200cb80
.L_0200cb6c:
	movs r7, #138
	movs r6, #254
	lsls r7, r7, #2
	lsls r6, r6, #1
	b .L_0200cb80
.L_0200cb76:
	movs r7, #154
	b .L_0200cb7c
.L_0200cb7a:
	movs r7, #162
.L_0200cb7c:
	lsls r7, r7, #2
	movs r6, #188
.L_0200cb80:
	ldr r5, .L_0200cbac
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r5, r1
	adds r2, r6, #0
	ldr r0, [r5]
	adds r1, r7, #0
	bl ObjectMotion_SetPositionAndCommit
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	bl Func_02005e1c
.L_0200cba6:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200cbac:
	.4byte gPartyState
.L_0200cbb0:
	.4byte .L_0200cb24
	.section .text.x0200cbb4,"ax",%progbits
	.global Func_02004bb4
	.thumb_func
Func_02004bb4:
	push {r5, lr}
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	ldr r5, .L_0200cbfc
	movs r1, #1
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #126
	bl Func_02005f94
	movs r0, #186
	lsls r0, r0, #2
	movs r1, #0
	adds r0, #255
	bl Func_02005f34
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #161
	lsls r0, r0, #1
	adds r5, #1
	bl GameFlag_ClearBit
	adds r0, r5, #0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02005e1c
	pop {r5, pc}
	.2byte 0x0000
.L_0200cbfc:
	.4byte 0x00001a92
	.section .text.x0200cc00,"ax",%progbits
	.global Func_02004c00
	.thumb_func
Func_02004c00:
	push {lr}
	ldr r3, .L_0200cc74
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200cc78
	cmp r2, r3
	bne .L_0200cc18
	ldr r0, .L_0200cc7c
	b .L_0200cc70
.L_0200cc18:
	ldr r3, .L_0200cc80
	cmp r2, r3
	beq .L_0200cc6e
	ldr r3, .L_0200cc84
	cmp r2, r3
	bne .L_0200cc28
	ldr r0, .L_0200cc88
	b .L_0200cc70
.L_0200cc28:
	ldr r3, .L_0200cc8c
	cmp r2, r3
	bne .L_0200cc32
	ldr r0, .L_0200cc90
	b .L_0200cc70
.L_0200cc32:
	ldr r3, .L_0200cc94
	cmp r2, r3
	bne .L_0200cc3c
	ldr r0, .L_0200cc98
	b .L_0200cc70
.L_0200cc3c:
	ldr r3, .L_0200cc9c
	cmp r2, r3
	bne .L_0200cc46
	ldr r0, .L_0200cca0
	b .L_0200cc70
.L_0200cc46:
	ldr r3, .L_0200cca4
	cmp r2, r3
	bne .L_0200cc50
	ldr r0, .L_0200cca8
	b .L_0200cc70
.L_0200cc50:
	ldr r3, .L_0200ccac
	cmp r2, r3
	bne .L_0200cc5a
	ldr r0, .L_0200ccb0
	b .L_0200cc70
.L_0200cc5a:
	ldr r3, .L_0200ccb4
	cmp r2, r3
	bne .L_0200cc64
	ldr r0, .L_0200ccb8
	b .L_0200cc70
.L_0200cc64:
	ldr r3, .L_0200ccbc
	cmp r2, r3
	bne .L_0200cc6e
	ldr r0, .L_0200ccc0
	b .L_0200cc70
.L_0200cc6e:
	ldr r0, .L_0200ccc4
.L_0200cc70:
	pop {pc}
	.2byte 0x0000
.L_0200cc74:
	.4byte gPartyState
.L_0200cc78:
	.4byte 0x00000055
.L_0200cc7c:
	.4byte Data_02006c10
.L_0200cc80:
	.4byte 0x00000056
.L_0200cc84:
	.4byte 0x00000057
.L_0200cc88:
	.4byte Data_02006e2c
.L_0200cc8c:
	.4byte 0x00000058
.L_0200cc90:
	.4byte Data_02006eec
.L_0200cc94:
	.4byte 0x00000059
.L_0200cc98:
	.4byte Data_02006fa0
.L_0200cc9c:
	.4byte 0x0000005a
.L_0200cca0:
	.4byte Data_0200706c
.L_0200cca4:
	.4byte 0x0000005b
.L_0200cca8:
	.4byte Data_020070fc
.L_0200ccac:
	.4byte 0x0000005c
.L_0200ccb0:
	.4byte Data_02007228
.L_0200ccb4:
	.4byte 0x0000005d
.L_0200ccb8:
	.4byte Data_02007318
.L_0200ccbc:
	.4byte 0x0000005e
.L_0200ccc0:
	.4byte Data_020073a8
.L_0200ccc4:
	.4byte Data_02006d6c
	.section .text.x0200ccc8,"ax",%progbits
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
	ldr r3, .L_0200ce9c
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #4
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	movs r2, #35
	adds r2, r2, r7
	ldrb r3, [r2]
	mov r8, r2
	str r3, [sp, #0]
	movs r0, #216
	movs r2, #136
	movs r3, #143
	lsls r2, r2, #18
	lsls r3, r3, #1
	ldr r1, .L_0200cea0
	lsls r0, r0, #16
	bl Func_02000218
	movs r1, #85
	adds r1, r1, r7
	movs r5, #0
	strb r5, [r1]
	mov r11, r0
	mov r10, r1
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r7, #48]
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	ldr r3, .L_0200cea4
	movs r0, #140
	adds r2, r2, r3
	ldr r1, [r7, #8]
	ldr r3, [r7, #16]
	lsls r0, r0, #1
	bl Func_02005d74
	movs r1, #2
	adds r6, r0, #0
	bl Func_02005d5c
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
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
	bl Event_SetStatus1c6
	b .L_0200cdac
.L_0200cd8c:
	ldr r3, .L_0200cea8
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
	bl WaitFrames
.L_0200cdac:
	adds r0, r7, #0
	bl Func_02005dec
	cmp r0, #0
	beq .L_0200cd8c
	movs r0, #144
	lsls r0, r0, #1
	bl Func_02005f94
	movs r0, #10
	bl Battle_WaitMode0
	ldr r5, .L_0200ce9c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r3, .L_0200ceac
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
	bl WaitFrames
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
	movs r2, #35
	strb r3, [r1]
	adds r2, r2, r7
	movs r3, #1
	strb r3, [r2]
	ldr r3, [r7, #16]
	ldr r1, [r7, #8]
	add r3, r9
	mov r8, r2
	adds r0, r7, #0
	ldr r2, [r7, #12]
	bl Func_02005d8c
	adds r0, r7, #0
	bl Func_02000674
	adds r0, r7, #0
	bl Func_02005d94
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	movs r1, #5
	adds r0, r6, #0
	bl Func_02005d5c
	movs r0, #12
	bl WaitFrames
	adds r0, r6, #0
	bl Func_02005d7c
	mov r1, r10
	movs r3, #3
	strb r3, [r1]
	ldr r3, [r7, #12]
	movs r1, #1
	str r3, [r7, #20]
	adds r0, r7, #0
	bl ObjectDispatch_SetSingleChildField26
	mov r2, sp
	ldrb r2, [r2]
	mov r3, r8
	strb r2, [r3]
	mov r0, r11
	bl Func_02005d7c
	movs r0, #1
	bl WaitFrames
	bl Event_WaitValue1c8Frames
	bl Func_02005e1c
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200ce9c:
	.4byte gPartyState
.L_0200cea0:
	.4byte 0xffe00000
.L_0200cea4:
	.4byte 0xfff80000
.L_0200cea8:
	.4byte Data_0300122c
.L_0200ceac:
	.4byte 0xfff00000
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
	bl Func_02005d74
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
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetPartAttribute
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
	.section .text.x0200cf3c,"ax",%progbits
	.global Func_02004f3c
	.thumb_func
Func_02004f3c:
	push {r5, r6, lr}
	sub sp, #8
	bl Func_02005f84
	bl Object_GetById
	adds r1, r0, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #32
	orrs r3, r2
	strb r3, [r1]
	movs r2, #128
	ldr r3, [r0, #12]
	lsls r2, r2, #14
	adds r3, r3, r2
	movs r2, #156
	lsls r2, r2, #17
	movs r6, #2
	cmp r3, r2
	bge .L_0200cf72
	movs r2, #216
	lsls r2, r2, #16
	movs r6, #1
	cmp r3, r2
	bge .L_0200cf72
	movs r6, #0
.L_0200cf72:
	ldr r3, .L_0200d000
	ldr r3, [r3]
	cmp r3, r6
	beq .L_0200cffc
	cmp r6, #1
	beq .L_0200cfaa
	cmp r6, #1
	bgt .L_0200cf88
	cmp r6, #0
	beq .L_0200cfd2
	b .L_0200cff8
.L_0200cf88:
	cmp r6, #2
	bne .L_0200cff8
	movs r3, #7
	movs r5, #39
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #64
	movs r2, #6
	movs r3, #4
	str r5, [sp, #4]
	bl Func_02005dac
	movs r3, #26
	str r3, [sp, #0]
	movs r0, #70
	movs r1, #64
	b .L_0200cfc6
.L_0200cfaa:
	movs r3, #7
	movs r5, #39
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #68
	movs r2, #6
	movs r3, #4
	str r5, [sp, #4]
	bl Func_02005dac
	movs r3, #26
	str r3, [sp, #0]
	movs r0, #70
	movs r1, #68
.L_0200cfc6:
	movs r2, #6
	movs r3, #4
	str r5, [sp, #4]
	bl Func_02005dac
	b .L_0200cff8
.L_0200cfd2:
	movs r3, #7
	movs r5, #39
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #72
	movs r2, #6
	movs r3, #4
	str r5, [sp, #4]
	bl Func_02005dac
	movs r3, #26
	str r3, [sp, #0]
	movs r0, #70
	movs r1, #68
	movs r2, #6
	movs r3, #4
	str r5, [sp, #4]
	bl Func_02005dac
.L_0200cff8:
	ldr r3, .L_0200d000
	str r6, [r3]
.L_0200cffc:
	add sp, #8
	pop {r5, r6, pc}
.L_0200d000:
	.4byte Data_020073d8
	.section .text.x0200d004,"ax",%progbits
	.global Func_02005004
	.thumb_func
Func_02005004:
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
	bge .L_0200d034
	adds r3, #15
.L_0200d034:
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
	.section .text.x0200d05c,"ax",%progbits
	.global Func_0200505c
	.thumb_func
Func_0200505c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200d1e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02005e14
	movs r0, #0
	bl Func_02005f2c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02005d84
	movs r0, #1
	bl WaitFrames
	movs r3, #130
	lsls r3, r3, #16
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
	movs r4, #214
	lsls r4, r4, #1
	movs r2, #128
	adds r3, r3, r4
	lsls r2, r2, #1
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #204
	bl Func_02005f94
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200d1e8
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200d0f6:
	mov r4, r10
	lsls r5, r4, #12
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
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_0200d1ec
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200d1f0
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r5, r5, r3
	adds r5, r5, r4
	ldr r4, [r6, #4]
	str r5, [r6, #8]
	ldr r2, [r7, #16]
	ldr r3, [r6]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r4, [sp, #0]
	ldr r4, .L_0200d1f4
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_020002f4
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200d0f6
	movs r0, #188
	bl Func_02005f94
	ldr r5, .L_0200d1e4
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02005ec4
	ldr r0, [r5]
	movs r1, #22
	bl Object_SetModeById
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
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02005dcc
	bl Func_02005dd4
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02005ec4
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
	bl Func_02005e1c
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200d1e4:
	.4byte gPartyState
.L_0200d1e8:
	.4byte Func_02005004
.L_0200d1ec:
	.4byte 0xffffa000
.L_0200d1f0:
	.4byte 0xffffd000
.L_0200d1f4:
	.4byte 0x01090001
	.section .text.x0200d1f8,"ax",%progbits
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
	ldr r5, .L_0200d2dc
	lsls r2, r2, #2
	str r2, [r3]
	adds r2, #16
	adds r6, r5, r2
	ldr r0, [r6]
	sub sp, #8
	bl Object_GetById
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
	ldr r3, .L_0200d2e0
	cmp r2, r3
	beq .L_0200d248
	b .L_0200d424
.L_0200d248:
	bl Func_02002770
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #16
	bne .L_0200d282
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d282
	ldr r0, [r6]
	bl Object_GetById
	movs r3, #1
	adds r0, #34
	strb r3, [r0]
	movs r0, #8
	bl WaitFrames
	bl Func_02005d84
	movs r0, #1
	bl WaitFrames
.L_0200d282:
	ldr r3, .L_0200d2dc
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #11
	ble .L_0200d296
	cmp r3, #16
	bne .L_0200d2b4
.L_0200d296:
	movs r1, #144
	ldr r0, .L_0200d2e4
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0200d2d4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200d2d8
	subs r2, #2
	strh r3, [r2]
	bl Func_02004eb0
.L_0200d2b4:
	ldr r3, .L_0200d2dc
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #13
	ble .L_0200d2fc
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #142
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d2f6
	b .L_0200d2e8
.L_0200d2d4:
	.4byte 0x00001008
.L_0200d2d8:
	.4byte 0x00003f10
.L_0200d2dc:
	.4byte gPartyState
.L_0200d2e0:
	.4byte 0x00000055
.L_0200d2e4:
	.4byte Func_02004f3c
.L_0200d2e8:
	movs r1, #206
	movs r2, #164
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02005e6c
.L_0200d2f6:
	ldr r0, .L_0200d41c
	bl Func_02005f6c
.L_0200d2fc:
	movs r0, #10
	bl Object_GetById
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #10
	bl Object_GetById
	str r5, [r0, #12]
	movs r0, #10
	bl Object_GetById
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r1, #3
	movs r0, #10
	bl ObjectMotion_SetActionVariant
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #154
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d340
	movs r0, #10
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	b .L_0200d36e
.L_0200d340:
	movs r0, #10
	bl Object_GetById
	movs r2, #0
	movs r1, #8
	bl Func_02001084
	movs r0, #11
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	movs r3, #3
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #78
	movs r2, #18
	movs r3, #105
	bl Func_02005d9c
.L_0200d36e:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d3a2
	movs r0, #8
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #9
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #12
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	b .L_0200d3cc
.L_0200d3a2:
	movs r0, #8
	bl Object_GetById
	movs r1, #180
	movs r2, #0
	bl Func_02001084
	movs r0, #9
	bl Object_GetById
	movs r1, #60
	movs r2, #0
	bl Func_02001084
	movs r0, #12
	bl Object_GetById
	movs r1, #60
	movs r2, #0
	bl Func_02001420
.L_0200d3cc:
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_0200d420
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #11
	beq .L_0200d404
	bl .L_0200dca4
.L_0200d404:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d414
	bl .L_0200dca4
.L_0200d414:
	bl Func_0200505c
	bl .L_0200dca4
.L_0200d41c:
	.4byte Data_02006050
.L_0200d420:
	.4byte gPartyState
.L_0200d424:
	ldr r3, .L_0200d464
	cmp r2, r3
	bne .L_0200d510
	ldr r3, .L_0200d45c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200d460
	subs r2, #2
	strh r3, [r2]
	bl Func_02002770
	movs r0, #243
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d468
	movs r1, #164
	movs r2, #196
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02005e6c
	b .L_0200d468
.L_0200d45c:
	.4byte 0x00001008
.L_0200d460:
	.4byte 0x00003f10
.L_0200d464:
	.4byte 0x00000056
.L_0200d468:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #152
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d484
	movs r1, #208
	movs r2, #196
	movs r0, #11
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Func_02005e6c
.L_0200d484:
	ldr r0, .L_0200d508
	bl Func_02005f6c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d4a6
	movs r0, #8
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	b .L_0200d4d4
.L_0200d4a6:
	movs r0, #8
	bl Object_GetById
	movs r2, #0
	movs r1, #60
	bl Func_02001084
	movs r0, #10
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	movs r3, #3
	movs r2, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #68
	movs r1, #84
	movs r2, #12
	movs r3, #77
	bl Func_02005d9c
.L_0200d4d4:
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_0200d50c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #3
	beq .L_0200d4f2
	b .L_0200dca4
.L_0200d4f2:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d500
	b .L_0200dca4
.L_0200d500:
	bl Func_02004cc8
	b .L_0200dca4
	.2byte 0x0000
.L_0200d508:
	.4byte Data_02006054
.L_0200d50c:
	.4byte gPartyState
.L_0200d510:
	ldr r3, .L_0200d564
	cmp r2, r3
	beq .L_0200d518
	b .L_0200d644
.L_0200d518:
	bl Func_02002770
	movs r0, #245
	bl Func_02005f4c
	ldr r3, .L_0200d55c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200d560
	subs r2, #2
	movs r1, #144
	strh r3, [r2]
	ldr r0, .L_0200d568
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #155
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d56c
	movs r1, #132
	movs r2, #158
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005e6c
	b .L_0200d56c
	.2byte 0x0000
.L_0200d55c:
	.4byte 0x00000c08
.L_0200d560:
	.4byte 0x00003f10
.L_0200d564:
	.4byte 0x00000057
.L_0200d568:
	.4byte Func_02001690
.L_0200d56c:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #156
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d588
	movs r1, #154
	movs r2, #142
	movs r0, #11
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02005e6c
.L_0200d588:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #157
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d5a4
	movs r1, #236
	movs r2, #130
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005e6c
.L_0200d5a4:
	ldr r0, .L_0200d8cc
	bl Func_02005f6c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d5de
	movs r0, #9
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #10
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #13
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	b .L_0200d608
.L_0200d5de:
	movs r0, #9
	bl Object_GetById
	movs r1, #60
	movs r2, #0
	bl Func_02001084
	movs r0, #10
	bl Object_GetById
	movs r1, #180
	movs r2, #0
	bl Func_02001084
	movs r0, #13
	bl Object_GetById
	movs r1, #200
	movs r2, #0
	bl Func_02001084
.L_0200d608:
	movs r0, #9
	bl Object_GetById
	movs r5, #0
	adds r0, #90
	strb r5, [r0]
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	strb r5, [r0]
	movs r0, #13
	bl Object_GetById
	adds r0, #90
	strb r5, [r0]
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #13
	b .L_0200dbc6
.L_0200d644:
	ldr r3, .L_0200d8d0
	cmp r2, r3
	beq .L_0200d64c
	b .L_0200d7bc
.L_0200d64c:
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
	bl Object_GetById
	movs r3, #1
	adds r0, #98
	strb r3, [r0]
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d6ee
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
	b .L_0200d73e
.L_0200d6ee:
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
.L_0200d73e:
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d798
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
.L_0200d798:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #158
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d7b4
	movs r1, #162
	movs r2, #148
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02005e6c
.L_0200d7b4:
	ldr r0, .L_0200d8d4
	bl Func_02005f6c
	b .L_0200dca4
.L_0200d7bc:
	ldr r3, .L_0200d8d8
	cmp r2, r3
	bne .L_0200d83e
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Func_02002770
	movs r6, #0
.L_0200d7d0:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #2
	adds r0, r6, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d808
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
.L_0200d808:
	adds r6, #1
	cmp r6, #6
	bne .L_0200d7d0
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
	b .L_0200dca4
.L_0200d83e:
	ldr r3, .L_0200d8dc
	cmp r2, r3
	beq .L_0200d846
	b .L_0200d99e
.L_0200d846:
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
	bne .L_0200d8e0
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
	b .L_0200d930
.L_0200d8cc:
	.4byte Data_0200605a
.L_0200d8d0:
	.4byte 0x00000058
.L_0200d8d4:
	.4byte Data_02006064
.L_0200d8d8:
	.4byte 0x00000059
.L_0200d8dc:
	.4byte 0x0000005a
.L_0200d8e0:
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
.L_0200d930:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d94c
	movs r1, #130
	movs r2, #148
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02005e6c
.L_0200d94c:
	movs r0, #244
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d95c
	b .L_0200dca4
.L_0200d95c:
	movs r1, #156
	movs r2, #148
	lsls r2, r2, #17
	movs r0, #8
	lsls r1, r1, #17
	bl Func_02005e6c
	movs r1, #2
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	movs r0, #8
	bl Object_GetById
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	movs r0, #8
	bl Object_GetById
	ldr r3, .L_0200daa8
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
	b .L_0200dca4
.L_0200d99e:
	ldr r3, .L_0200daac
	cmp r2, r3
	beq .L_0200d9a6
	b .L_0200dab8
.L_0200d9a6:
	bl Func_02002770
	movs r0, #12
	bl Object_GetById
	movs r6, #1
	adds r0, #98
	strb r6, [r0]
	movs r0, #13
	bl Object_GetById
	adds r0, #98
	strb r6, [r0]
	movs r0, #14
	bl Object_GetById
	adds r0, #98
	strb r6, [r0]
	movs r0, #15
	bl Object_GetById
	adds r0, #98
	strb r6, [r0]
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d9f4
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
.L_0200d9f4:
	ldr r0, .L_0200dab0
	bl Func_02000908
	movs r0, #141
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200da14
	movs r1, #172
	movs r2, #174
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005e6c
.L_0200da14:
	ldr r0, .L_0200dab4
	bl Func_02005f6c
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200da5e
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
.L_0200da5e:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200da6e
	b .L_0200dca4
.L_0200da6e:
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
	b .L_0200dca4
.L_0200daa8:
	.4byte 0xffe00000
.L_0200daac:
	.4byte 0x0000005b
.L_0200dab0:
	.4byte Data_0200621c
.L_0200dab4:
	.4byte Data_02006068
.L_0200dab8:
	ldr r3, .L_0200db08
	cmp r2, r3
	beq .L_0200dac0
	b .L_0200dbd2
.L_0200dac0:
	movs r0, #245
	bl Func_02005f4c
	ldr r3, .L_0200db00
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200db04
	subs r2, #2
	movs r1, #144
	strh r3, [r2]
	ldr r0, .L_0200db0c
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #209
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200db10
	movs r1, #140
	movs r2, #236
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02005e6c
	b .L_0200db10
	.2byte 0x0000
.L_0200db00:
	.4byte 0x00000c08
.L_0200db04:
	.4byte 0x00003f10
.L_0200db08:
	.4byte 0x0000005c
.L_0200db0c:
	.4byte Func_02001690
.L_0200db10:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #210
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200db2c
	movs r1, #180
	movs r2, #210
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005e6c
.L_0200db2c:
	movs r0, #153
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200db50
	movs r1, #184
	movs r2, #154
	movs r0, #12
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02005e6c
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_0200db50:
	movs r1, #2
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	ldr r0, .L_0200dcac
	bl Func_02005f6c
	movs r0, #8
	bl Object_GetById
	ldr r5, [r0, #8]
	movs r0, #8
	bl Object_GetById
	ldr r2, [r0, #16]
	asrs r5, r5, #20
	adds r1, r5, #0
	asrs r2, r2, #20
	movs r3, #0
	movs r0, #2
	bl Func_0200062c
	movs r0, #9
	bl Object_GetById
	ldr r5, [r0, #8]
	movs r0, #9
	bl Object_GetById
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
	bne .L_0200dbb6
	movs r0, #11
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	b .L_0200dbc4
.L_0200dbb6:
	movs r0, #11
	bl Object_GetById
	movs r1, #8
	movs r2, #0
	bl Func_02001084
.L_0200dbc4:
	movs r0, #11
.L_0200dbc6:
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	b .L_0200dca4
.L_0200dbd2:
	ldr r3, .L_0200dcb0
	cmp r2, r3
	bne .L_0200dca4
	bl Func_02002770
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #211
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200dbf8
	movs r1, #180
	movs r2, #132
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02005e6c
.L_0200dbf8:
	ldr r0, .L_0200dcb4
	bl Func_02005f6c
	movs r0, #10
	bl Object_GetById
	ldr r5, [r0, #8]
	movs r0, #10
	bl Object_GetById
	ldr r2, [r0, #16]
	asrs r5, r5, #20
	movs r3, #0
	asrs r2, r2, #20
	adds r1, r5, #0
	movs r0, #2
	bl Func_0200062c
	movs r0, #10
	bl Object_GetById
	movs r3, #0
	adds r0, #90
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200dc44
	movs r0, #10
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	b .L_0200dc52
.L_0200dc44:
	movs r0, #10
	bl Object_GetById
	movs r1, #60
	movs r2, #0
	bl Func_02001084
.L_0200dc52:
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200dca4
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
.L_0200dca4:
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200dcac:
	.4byte Data_02006072
.L_0200dcb0:
	.4byte 0x0000005d
.L_0200dcb4:
	.4byte Data_0200607e
	.section .text.x0200dcb8,"ax",%progbits
	.global Func_02005cb8
	.thumb_func
Func_02005cb8:
	push {lr}
	ldr r3, .L_0200dcd4
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200dcd8
	cmp r2, r3
	bne .L_0200dcd0
	bl Func_02004f3c
.L_0200dcd0:
	movs r0, #0
	pop {pc}
.L_0200dcd4:
	.4byte gPartyState
.L_0200dcd8:
	.4byte 0x00000055
	.section .rodata.x0200df9c,"a",%progbits
.L_0200df9c:
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
	.global Data_02005fd8
Data_02005fd8:
.L_0200dfd8:
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
.L_0200e014:
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
	.global Data_02006050
Data_02006050:
	.4byte 0xffff000c
	.global Data_02006054
Data_02006054:
	.4byte 0x000b0009
	.2byte 0xffff
	.global Data_0200605a
Data_0200605a:
	.2byte 0x0008
	.4byte 0x000c000b
	.4byte 0xffff000a
	.global Data_02006064
Data_02006064:
	.4byte 0xffff0009
	.global Data_02006068
Data_02006068:
	.4byte 0x00090008
	.4byte 0x000b000a
	.2byte 0xffff
	.global Data_02006072
Data_02006072:
	.2byte 0x000c
	.4byte 0x00090008
	.4byte 0x000e000d
	.2byte 0xffff
	.global Data_0200607e
Data_0200607e:
	.2byte 0x0009
	.4byte 0xffff000a
	.global Data_02006084
Data_02006084:
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02006094
Data_02006094:
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_020060b0
Data_020060b0:
	.4byte 0x0000002e
	.4byte Func_02003ed4
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_020060c0
Data_020060c0:
	.4byte 0x0000002e
	.4byte Func_02003f94
	.4byte 0x0000002e
	.4byte Func_02004038
	.4byte 0x0000002e
	.4byte Func_02004084
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_020060e0
Data_020060e0:
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000004
	.4byte 0x00000011
	.global Data_02006104
Data_02006104:
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000004
	.4byte 0x00000011
	.global Data_02006128
Data_02006128:
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
	.global Data_02006158
Data_02006158:
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
	.global Data_02006188
Data_02006188:
	.4byte .L_0200df9c
	.4byte .L_0200dfd8
	.4byte .L_0200e014
	.global Data_02006194
Data_02006194:
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_020061ac
Data_020061ac:
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_020061cc
Data_020061cc:
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
.L_0200e1f8:
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000011
	.global Data_0200621c
Data_0200621c:
	.4byte 0x0001000c
	.4byte 0x000d0200
	.4byte Data_02010001
	.4byte 0x0001000e
	.4byte 0x000f0202
	.4byte Data_02030000 + 0x1
	.4byte 0x0000ffff
	.global Data_02006238
Data_02006238:
	.4byte 0xffff0000
	.4byte 0x00000140
	.4byte 0x40000140
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006268
Data_02006268:
	.4byte 0xffff0001
	.4byte 0x000000e8
	.4byte 0x40000078
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff0002
	.4byte 0x00000098
	.4byte 0x40000088
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff0003
	.4byte 0x00000098
	.4byte 0x400000e8
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff0004
	.4byte 0x01400098
	.4byte 0x40000288
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff0005
	.4byte 0x00800098
	.4byte 0x40000288
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff0006
	.4byte 0x000000e8
	.4byte 0x40000278
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff0007
	.4byte 0x00000138
	.4byte 0x40000268
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff0008
	.4byte 0x000001d8
	.4byte 0x40000088
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff0009
	.4byte 0x000001d8
	.4byte 0x400000e8
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff000a
	.4byte 0x00000198
	.4byte 0xc00003b8
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff000b
	.4byte 0x00000138
	.4byte 0x40000348
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0xffff000c
	.4byte 0x00000328
	.4byte 0xc0000088
	.4byte 0x02600000
	.4byte 0x03600010
	.4byte 0x000000e0
	.4byte 0xffff000d
	.4byte 0x000002a8
	.4byte 0x400000a8
	.4byte 0x02600000
	.4byte 0x03600010
	.4byte 0x000000e0
	.4byte 0xffff000e
	.4byte 0x00000368
	.4byte 0x40000158
	.4byte 0x02500000
	.4byte 0x03d00100
	.4byte 0x000003f0
	.4byte 0xffff000f
	.4byte 0x00000388
	.4byte 0x40000388
	.4byte 0x02500000
	.4byte 0x03d00100
	.4byte 0x000003f0
	.4byte 0xffff0010
	.4byte 0x00000138
	.4byte 0x40000328
	.4byte 0x00400000
	.4byte 0x02300010
	.4byte 0x000003f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006400
Data_02006400:
	.4byte 0x00000055
	.4byte 0x10102056
	.4byte 0xffffffff
	.4byte 0x10201057
	.4byte 0xffffffff
	.4byte 0x10304058
	.4byte 0xffffffff
	.4byte 0x1040205a
	.4byte 0xffffffff
	.4byte 0x1050c055
	.4byte 0xffffffff
	.4byte 0x1060105e
	.4byte 0xffffffff
	.4byte 0x1070205e
	.4byte 0xffffffff
	.4byte 0x1080105b
	.4byte 0xffffffff
	.4byte 0x1090105c
	.4byte 0xffffffff
	.4byte 0x10a04053
	.4byte 0xffffffff
	.4byte 0x10b0e055
	.4byte 0xffffffff
	.4byte 0x10c05055
	.4byte 0xffffffff
	.4byte 0x10d0d055
	.4byte 0xffffffff
	.4byte 0x10e0205d
	.4byte 0xffffffff
	.4byte 0x10f01059
	.4byte 0xffffffff
	.4byte 0x11003056
	.4byte 0xffffffff
	.4byte 0x00000056
	.4byte 0x1010e054
	.4byte 0xffffffff
	.4byte 0x10201055
	.4byte 0xffffffff
	.4byte 0x1030b055
	.4byte 0xffffffff
	.4byte 0x00000057
	.4byte 0x10102055
	.4byte 0xffffffff
	.4byte 0x10201058
	.4byte 0xffffffff
	.4byte 0x00000058
	.4byte 0x10102057
	.4byte 0xffffffff
	.4byte 0x1020105a
	.4byte 0xffffffff
	.4byte 0x1030105d
	.4byte 0xffffffff
	.4byte 0x10403055
	.4byte 0xffffffff
	.4byte 0x00000059
	.4byte 0x10110055
	.4byte 0xffffffff
	.4byte 0x0000005a
	.4byte 0x10102058
	.4byte 0xffffffff
	.4byte 0x10204055
	.4byte 0xffffffff
	.4byte 0x0000005b
	.4byte 0x10108055
	.4byte 0xffffffff
	.4byte 0x1020205c
	.4byte 0xffffffff
	.4byte 0x0000005c
	.4byte 0x10109055
	.4byte 0xffffffff
	.4byte 0x1020205b
	.4byte 0xffffffff
	.4byte 0x0000005d
	.4byte 0x10103058
	.4byte 0xffffffff
	.4byte 0x1020f055
	.4byte 0xffffffff
	.4byte 0x0000005e
	.4byte 0x10106055
	.4byte 0xffffffff
	.4byte 0x10207055
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_0200654c
Data_0200654c:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006564
Data_02006564:
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapBlocks
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02028000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapCollision
	.4byte 0xffff017e
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00024000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0118
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0118
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0118
	.4byte 0x00000007
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200663c
Data_0200663c:
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff017e
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020066b4
Data_020066b4:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapBlocks
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02028000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200675c
Data_0200675c:
	.4byte 0xffff0145
	.4byte .L_0200e1f8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0145
	.4byte .L_0200e1f8
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
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
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020068ac
Data_020068ac:
	.4byte 0xffff0143
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0006
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
	.global Data_020068f4
Data_020068f4:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0145
	.4byte .L_0200e1f8
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0145
	.4byte .L_0200e1f8
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
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
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020069e4
Data_020069e4:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0110
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
	.4byte Data_02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0145
	.4byte .L_0200e1f8
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0145
	.4byte .L_0200e1f8
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006aec
Data_02006aec:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0145
	.4byte .L_0200e1f8
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02028000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006bac
Data_02006bac:
	.4byte 0xffff0145
	.4byte .L_0200e1f8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapBlocks
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006c0c
Data_02006c0c:
	.4byte 0xffffffff
	.global Data_02006c10
Data_02006c10:
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
	.4byte 0x00000031
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
	.4byte 0x00000021
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000031
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000021
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x0000c602
	.4byte 0xffff001e
	.4byte Func_02003170
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte Func_02002898
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte Func_02002790
	.4byte 0x00000002
	.4byte 0xffff0051
	.4byte Func_02002898
	.4byte 0x00000002
	.4byte 0xffff0052
	.4byte Func_02002980
	.4byte 0x00000002
	.4byte 0xffff0053
	.4byte Func_02002a14
	.4byte 0x80004e15
	.4byte Monster_WhiteKnightSprites + 0xc9f
	.4byte Func_02000610
	.4byte 0x10004e15
	.4byte Monster_WhiteKnightSprites + 0xc9f
	.4byte Func_0200061c
	.4byte 0x00004e15
	.4byte Monster_WhiteKnightSprites + 0xc9f
	.4byte Func_02002cf0
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02003254
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte 0xffff000c
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte Func_0200332c
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte Func_02000988
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006d6c
Data_02006d6c:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00004602
	.4byte 0xffff0014
	.4byte Func_02003ac4
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte Func_02002790
	.4byte 0x00000002
	.4byte 0xffff0051
	.4byte Func_02002898
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02003254
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte Monster_BlueDevilSprites + 0x4b19
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte Monster_BlueDevilSprites + 0x4b19
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte Monster_ApeSprites + 0x158b
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte Monster_ApeSprites + 0x158b
	.4byte Func_0200332c
	.4byte 0x80004e15
	.4byte Monster_SkullMageSprites + 0x70e
	.4byte Func_02000610
	.4byte 0x10004e15
	.4byte Monster_SkullMageSprites + 0x70e
	.4byte Func_0200061c
	.4byte 0x00004e15
	.4byte Monster_SkullMageSprites + 0x70e
	.4byte Func_02003654
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte Func_02000988
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006e2c
Data_02006e2c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte Func_02002790
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02003254
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte Monster_PinkOgreSprites + 0x954
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte Monster_PinkOgreSprites + 0x954
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte Resource_Data21F + 0x3023
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte Resource_Data21F + 0x3023
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte Monster_DragonLeftSprites + 0x226c
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte Monster_DragonLeftSprites + 0x226c
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte 0xffff000a
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte Func_0200332c
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte Func_02000988
	.4byte 0x00000006
	.4byte 0xffff00c9
	.4byte Func_02003b08
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006eec
Data_02006eec:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02003254
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte Monster_VultureSprites + 0x95d
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte Monster_VultureSprites + 0x95d
	.4byte Func_0200332c
	.4byte 0x80004e15
	.4byte 0xffff0008
	.4byte Func_02000610
	.4byte 0x10004e15
	.4byte 0xffff0008
	.4byte Func_0200061c
	.4byte 0x00004e15
	.4byte 0xffff0008
	.4byte Func_02001828
	.4byte 0x80004e15
	.4byte 0xffff000a
	.4byte Func_02000610
	.4byte 0x10004e15
	.4byte 0xffff000a
	.4byte Func_0200061c
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte Func_02001828
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006fa0
Data_02006fa0:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte Func_02002790
	.4byte 0x00000002
	.4byte 0xffff0051
	.4byte Func_02002898
	.4byte 0x00000002
	.4byte 0xffff005a
	.4byte Func_02003d40
	.4byte 0x00008602
	.4byte 0xffff000c
	.4byte Data_02004880 + 0x1
	.4byte 0x0000c403
	.4byte 0xffff001e
	.4byte Func_02004534
	.4byte 0x00004403
	.4byte 0xffff001e
	.4byte Func_02004740
	.4byte 0x00008403
	.4byte 0xffff001e
	.4byte Func_020047b0
	.4byte 0x00000403
	.4byte 0xffff001e
	.4byte Func_02004818
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02003e28
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02003dd4
	.4byte 0x00000006
	.4byte 0xffff00ca
	.4byte Func_02003cac
	.4byte 0x00000006
	.4byte 0xffff00cb
	.4byte Func_02003cac
	.4byte 0x00000006
	.4byte 0xffff00cc
	.4byte Func_02003cac
	.4byte 0x00000006
	.4byte 0xffff00cd
	.4byte Func_02003cac
	.4byte 0x00000006
	.4byte 0xffff00ce
	.4byte Func_02003cac
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200706c
Data_0200706c:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte Func_02002790
	.4byte 0x00000002
	.4byte 0xffff0051
	.4byte Func_02002898
	.4byte 0x00008c15
	.4byte Monster_ScorpionSprites + 0x142c
	.4byte Func_02004884
	.4byte 0x80004e15
	.4byte 0xffff0009
	.4byte Func_02000610
	.4byte 0x10004e15
	.4byte 0xffff0009
	.4byte Func_0200061c
	.4byte 0x00004e15
	.4byte 0xffff0009
	.4byte Func_02001828
	.4byte 0x80004e15
	.4byte 0xffff000a
	.4byte Func_02000610
	.4byte 0x10004e15
	.4byte 0xffff000a
	.4byte Func_0200061c
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte Func_02001828
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020070fc
Data_020070fc:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte Func_02002790
	.4byte 0x00000002
	.4byte 0xffff00d4
	.4byte Func_020048c4
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02003254
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte Tileset_Set73TilesD + 0x1228
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte Tileset_Set73TilesD + 0x1228
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte 0xffff0009
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte 0xffff000a
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte 0xffff000b
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte Func_0200332c
	.4byte 0x50008615
	.4byte Data_02000000 + 0xc
	.4byte Func_020048bc
	.4byte 0x50008615
	.4byte Data_02010002 + 0xb
	.4byte Func_020048bc
	.4byte 0x50008615
	.4byte Data_02020004 + 0xa
	.4byte Func_020048bc
	.4byte 0x50008615
	.4byte Data_02030000 + 0xf
	.4byte Func_020048bc
	.4byte 0x80004e15
	.4byte 0xffff0010
	.4byte Func_02000610
	.4byte 0x10004e15
	.4byte 0xffff0010
	.4byte Func_0200061c
	.4byte 0x00004e15
	.4byte 0xffff0010
	.4byte Func_02001828
	.4byte 0x80004e15
	.4byte 0xffff0011
	.4byte Func_02000610
	.4byte 0x10004e15
	.4byte 0xffff0011
	.4byte Func_0200061c
	.4byte 0x00004e15
	.4byte 0xffff0011
	.4byte Func_02001828
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02007228
Data_02007228:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000006
	.4byte 0xffff00c9
	.4byte Func_02004ab8
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte Func_02000988
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02003254
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte 0x0a8f000c
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte 0x0a8f000c
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte Field_Map165 + 0x8e8
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte Field_Map165 + 0x8e8
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte Field_Map172 + 0x769
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte Field_Map172 + 0x769
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte 0xffff000d
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte 0xffff000e
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte Func_0200332c
	.4byte 0x80004e15
	.4byte 0xffff000a
	.4byte Func_02000610
	.4byte 0x10004e15
	.4byte 0xffff000a
	.4byte Func_0200061c
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte Func_02001828
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02007318
Data_02007318:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte Func_02002790
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte Func_02000988
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02003254
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_0200332c
	.4byte 0x10008c15
	.4byte Field_Map176 + 0x15f9
	.4byte Func_02003254
	.4byte 0x00008c15
	.4byte Field_Map176 + 0x15f9
	.4byte Func_0200332c
	.4byte 0x80004e15
	.4byte 0xffff0008
	.4byte Func_02000610
	.4byte 0x10004e15
	.4byte 0xffff0008
	.4byte Func_0200061c
	.4byte 0x00004e15
	.4byte 0xffff0008
	.4byte Func_02001828
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020073a8
Data_020073a8:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0xffff001e
	.4byte Func_02004bb4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020073d8
Data_020073d8:
	.4byte 0xffffffff
