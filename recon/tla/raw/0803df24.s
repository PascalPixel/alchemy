.syntax unified
	.thumb
	.global Func_0803df24
	.thumb_func
Func_0803df24:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #72]
	movs r0, #192
	mov r10, r3
	lsls r0, r0, #2
	movs r3, #0
	sub sp, #24
	add r0, r10
	str r0, [sp, #8]
	mov r11, r3
	movs r3, #210
	lsls r3, r3, #2
	add r3, r10
	movs r1, #182
	movs r2, #195
	ldr r7, [r3]
	lsls r1, r1, #2
	lsls r2, r2, #2
	add r1, r10
	add r2, r10
	mov r8, r1
	mov r9, r2
	cmp r7, #0
	bne .L_0803df66
	b .L_0803e0ba
.L_0803df66:
	adds r6, r7, #0
	adds r6, #40
	ldrb r3, [r6, #5]
	movs r4, #4
	negs r4, r4
	adds r2, r4, #0
	ands r3, r2
	strb r3, [r6, #5]
	movs r5, #63
	ldrb r3, [r6, #7]
	negs r5, r5
	adds r2, r5, #0
	ands r3, r2
	strb r3, [r6, #7]
	ldrh r1, [r7, #16]
	ldr r3, .L_0803dfb0
	ldr r2, .L_0803dfb4
	ands r1, r3
	ldrh r3, [r6, #6]
	movs r0, #240
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #6]
	ldrh r3, [r7, #18]
	strb r3, [r6, #4]
	adds r1, r3, #0
	str r0, [sp, #4]
	movs r3, #232
	lsls r3, r3, #2
	add r3, r10
	ldrh r2, [r3]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0803dfb8
	adds r3, r2, r1
	strb r3, [r6, #4]
	b .L_0803e058
.L_0803dfb0:
	.4byte 0x000001ff
.L_0803dfb4:
	.4byte 0xfffffe00
.L_0803dfb8:
	movs r1, #16
	ldrsh r2, [r7, r1]
	movs r3, #24
	ldrsh r1, [r7, r3]
	ldrh r5, [r7, #16]
	ldrh r4, [r7, #24]
	cmp r2, r1
	beq .L_0803e00c
	ldrh r0, [r7, #20]
	mov r12, r0
	movs r0, #20
	ldrsh r3, [r7, r0]
	cmp r3, #0
	ble .L_0803dfe0
	adds r3, r2, r3
	cmp r3, r1
	bgt .L_0803dfe6
	mov r1, r12
	adds r3, r5, r1
	b .L_0803dfee
.L_0803dfe0:
	adds r3, r2, r3
	cmp r3, r1
	bge .L_0803dfea
.L_0803dfe6:
	strh r4, [r7, #16]
	b .L_0803dff0
.L_0803dfea:
	mov r2, r12
	adds r3, r5, r2
.L_0803dfee:
	strh r3, [r7, #16]
.L_0803dff0:
	ldrh r1, [r7, #16]
	ldr r3, .L_0803e004
	ldr r2, .L_0803e008
	ands r1, r3
	ldrh r3, [r6, #6]
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #6]
	b .L_0803e058
	.2byte 0x0000
.L_0803e004:
	.4byte 0x000001ff
.L_0803e008:
	.4byte 0xfffffe00
.L_0803e00c:
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #158
	add r3, r10
	ldrh r3, [r3]
	cmp r11, r3
	bne .L_0803e058
	movs r3, #241
	str r3, [sp, #4]
	mov r4, r8
	ldrh r3, [r4, #10]
	cmp r3, #0
	beq .L_0803e058
	add r5, sp, #12
	adds r1, r5, #0
	ldrh r0, [r7, #8]
	bl Func_081180b0
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_0803e058
	ldr r2, [r5]
	mov r3, r8
	strh r2, [r3, #24]
	mov r4, r8
	ldr r1, [r5, #4]
	movs r5, #34
	ldrsh r3, [r4, r5]
	strh r1, [r4, #26]
	cmp r3, #0
	bne .L_0803e058
	mov r0, r8
	strh r2, [r0, #16]
	movs r3, #1
	mov r2, r8
	strh r1, [r2, #18]
	strh r3, [r4, #34]
.L_0803e058:
	movs r5, #34
	ldrsh r3, [r7, r5]
	cmp r3, #0
	beq .L_0803e0ae
	movs r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0803e0a6
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #226
	add r3, r10
	ldrh r3, [r3]
	cmp r3, #1
	bne .L_0803e088
	ldrb r3, [r6, #5]
	movs r0, #13
	negs r0, r0
	ands r3, r0
	movs r2, #4
	orrs r3, r2
	b .L_0803e090
.L_0803e088:
	ldrb r3, [r6, #5]
	movs r1, #13
	negs r1, r1
	ands r3, r1
.L_0803e090:
	strb r3, [r6, #5]
	ldrh r3, [r7, #10]
	cmp r3, #1
	bne .L_0803e0a6
	ldrb r3, [r6, #5]
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #5]
.L_0803e0a6:
	adds r0, r6, #0
	ldr r1, [sp, #4]
	bl Runtime_PushSlotEntry
.L_0803e0ae:
	ldr r7, [r7, #4]
	movs r3, #1
	add r11, r3
	cmp r7, #0
	beq .L_0803e0ba
	b .L_0803df66
.L_0803e0ba:
	mov r4, r9
	ldrh r3, [r4, #10]
	cmp r3, #0
	bne .L_0803e0c4
	b .L_0803e1ea
.L_0803e0c4:
	mov r0, r10
	bl NodeChain_GetNodeAtCount
	mov r5, r9
	adds r5, #40
	ldrb r3, [r5, #5]
	movs r6, #13
	negs r6, r6
	adds r2, r6, #0
	ldrb r1, [r5, #7]
	ands r2, r3
	movs r3, #4
	negs r3, r3
	ands r2, r3
	subs r3, #59
	ands r3, r1
	movs r1, #17
	negs r1, r1
	ands r2, r1
	movs r1, #32
	adds r7, r0, #0
	orrs r2, r1
	movs r0, #63
	ands r2, r0
	strb r2, [r5, #5]
	ands r3, r0
	ldrb r2, [r5, #9]
	movs r1, #128
	orrs r3, r1
	strb r3, [r5, #7]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r5, #9]
	mov r0, r9
	ldrh r3, [r0, #14]
	movs r2, #192
	lsls r2, r2, #2
	adds r2, #255
	ands r2, r3
	ldrh r1, [r5, #8]
	ldr r3, .L_0803e314
	ldr r0, .L_0803e318
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
	ldrh r2, [r7, #16]
	movs r1, #128
	lsls r1, r1, #1
	adds r1, #255
	subs r2, #4
	ands r2, r1
	ldr r3, .L_0803e31c
	ldrh r1, [r5, #6]
	mov r11, r3
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #6]
	ldr r3, .L_0803e320
	movs r2, #15
	ldr r3, [r3]
	ldrb r1, [r7, #18]
	lsrs r3, r3, #1
	ands r3, r2
	ldrb r3, [r0, r3]
	mov r4, r9
	lsls r3, r3, #24
	asrs r3, r3, #25
	adds r1, r1, r3
	subs r1, #4
	strb r1, [r5, #4]
	movs r0, #34
	ldrsh r2, [r4, r0]
	movs r0, #38
	ldrsh r3, [r4, r0]
	ldrh r1, [r4, #34]
	cmp r2, r3
	beq .L_0803e1ca
	movs r0, #208
	lsls r0, r0, #2
	add r0, r10
	strh r1, [r0]
	movs r3, #192
	ldrh r2, [r4, #34]
	lsls r3, r3, #2
	adds r3, #66
	add r3, r10
	strh r2, [r3]
	movs r2, #209
	lsls r2, r2, #2
	add r2, r10
	movs r3, #0
	strh r3, [r2]
	bl AffineMatrix_BuildForEffect
	ldrb r2, [r5, #7]
	movs r3, #31
	ands r0, r3
	movs r3, #63
	negs r3, r3
	lsls r0, r0, #1
	ands r3, r2
	orrs r3, r0
	strb r3, [r5, #7]
	ldrb r3, [r5, #5]
	ldrh r1, [r5, #6]
	movs r2, #3
	orrs r3, r2
	movs r2, #255
	strb r3, [r5, #5]
	lsls r2, r2, #8
	lsls r3, r1, #23
	movs r4, #128
	adds r2, #240
	lsrs r3, r3, #23
	lsls r4, r4, #1
	adds r3, r3, r2
	adds r4, #255
	mov r2, r11
	ands r3, r4
	ands r2, r1
	orrs r2, r3
	ldrb r3, [r5, #4]
	strh r2, [r5, #6]
	adds r3, #240
	strb r3, [r5, #4]
	mov r0, r9
	ldrh r3, [r0, #34]
	ldrh r2, [r0, #36]
	mov r1, r9
	adds r3, r3, r2
	strh r3, [r1, #34]
.L_0803e1ca:
	movs r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0803e1e2
	ldrb r3, [r5, #5]
	adds r2, r6, #0
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	strb r2, [r5, #5]
.L_0803e1e2:
	adds r0, r5, #0
	movs r1, #248
	bl Runtime_PushSlotEntry
.L_0803e1ea:
	mov r0, r10
	movs r1, #0
	bl Func_0803e488
	mov r0, r10
	movs r1, #1
	bl Func_0803e488
	movs r3, #211
	lsls r3, r3, #2
	add r3, r10
	ldr r7, [r3]
	cmp r7, #0
	bne .L_0803e208
	b .L_0803e34e
.L_0803e208:
	movs r2, #208
	lsls r2, r2, #2
	movs r3, #13
	add r2, r10
	negs r3, r3
	mov r9, r2
	mov r11, r3
.L_0803e216:
	movs r4, #16
	ldrsh r2, [r7, r4]
	movs r5, #24
	ldrsh r3, [r7, r5]
	adds r6, r7, #0
	adds r6, #40
	ldrh r1, [r7, #16]
	cmp r2, r3
	beq .L_0803e22e
	ldrh r3, [r7, #20]
	adds r3, r1, r3
	strh r3, [r7, #16]
.L_0803e22e:
	movs r0, #18
	ldrsh r2, [r7, r0]
	movs r4, #26
	ldrsh r3, [r7, r4]
	ldrh r1, [r7, #18]
	cmp r2, r3
	beq .L_0803e242
	ldrh r3, [r7, #22]
	adds r3, r1, r3
	strh r3, [r7, #18]
.L_0803e242:
	movs r4, #128
	ldrh r3, [r7, #16]
	lsls r4, r4, #1
	ldr r5, .L_0803e31c
	ldrh r1, [r6, #6]
	adds r4, #255
	adds r2, r4, #0
	ands r2, r3
	adds r3, r5, #0
	ands r3, r1
	orrs r3, r2
	strh r3, [r6, #6]
	ldrh r3, [r7, #18]
	movs r0, #0
	strb r3, [r6, #4]
	ldrh r1, [r7, #34]
	movs r3, #34
	ldrsh r2, [r7, r3]
	mov lr, r2
	movs r3, #38
	ldrsh r2, [r7, r3]
	mov r12, r2
	cmp lr, r12
	beq .L_0803e2d6
	ldrh r3, [r7, #36]
	str r4, [sp, #0]
	adds r3, r1, r3
	mov r1, r9
	strh r3, [r7, #34]
	strh r3, [r1]
	movs r3, #192
	ldrh r2, [r7, #34]
	lsls r3, r3, #2
	adds r3, #66
	add r3, r10
	strh r2, [r3]
	movs r3, #209
	lsls r3, r3, #2
	add r3, r10
	strh r0, [r3]
	mov r0, r9
	bl AffineMatrix_BuildForEffect
	movs r3, #31
	ands r0, r3
	movs r1, #63
	ldrb r3, [r6, #7]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	lsls r0, r0, #1
	orrs r3, r0
	strb r3, [r6, #7]
	ldrb r3, [r6, #5]
	ldrh r1, [r6, #6]
	movs r2, #3
	orrs r3, r2
	strb r3, [r6, #5]
	movs r3, #255
	lsls r2, r1, #23
	lsls r3, r3, #8
	ldr r4, [sp, #0]
	adds r3, #248
	lsrs r2, r2, #23
	adds r2, r2, r3
	adds r3, r5, #0
	ands r2, r4
	ands r3, r1
	orrs r3, r2
	strh r3, [r6, #6]
	ldrb r3, [r6, #4]
	adds r3, #248
	strb r3, [r6, #4]
	b .L_0803e2ee
.L_0803e2d6:
	ldrb r3, [r6, #5]
	movs r4, #4
	negs r4, r4
	adds r2, r4, #0
	ands r3, r2
	strb r3, [r6, #5]
	movs r5, #63
	ldrb r3, [r6, #7]
	negs r5, r5
	adds r2, r5, #0
	ands r3, r2
	strb r3, [r6, #7]
.L_0803e2ee:
	movs r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0803e33e
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #226
	add r3, r10
	ldrh r3, [r3]
	cmp r3, #1
	bne .L_0803e324
	ldrb r3, [r6, #5]
	mov r0, r11
	ands r3, r0
	movs r2, #4
	orrs r3, r2
	b .L_0803e32a
.L_0803e314:
	.4byte 0xfffffc00
.L_0803e318:
	.4byte Data_0805ea0c
.L_0803e31c:
	.4byte 0xfffffe00
.L_0803e320:
	.4byte gFrameTick
.L_0803e324:
	ldrb r3, [r6, #5]
	mov r1, r11
	ands r3, r1
.L_0803e32a:
	strb r3, [r6, #5]
	ldrh r3, [r7, #10]
	cmp r3, #1
	bne .L_0803e33e
	ldrb r3, [r6, #5]
	mov r2, r11
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #5]
.L_0803e33e:
	adds r0, r6, #0
	movs r1, #240
	bl Runtime_PushSlotEntry
	ldr r7, [r7, #4]
	cmp r7, #0
	beq .L_0803e34e
	b .L_0803e216
.L_0803e34e:
	mov r4, r8
	ldrh r3, [r4, #10]
	cmp r3, #0
	bne .L_0803e358
	b .L_0803e46c
.L_0803e358:
	ldr r3, .L_0803e3a8
	ldr r5, .L_0803e3ac
	ldr r2, [r3]
	movs r3, #15
	lsrs r2, r2, #2
	ands r2, r3
	mov r11, r5
	lsls r2, r2, #8
	movs r1, #128
	add r2, r11
	ldrh r0, [r4, #12]
	lsls r1, r1, #1
	bl VramBlock_LoadCached
	ldr r3, .L_0803e3a4
	ldr r1, [sp, #8]
	ands r0, r3
	ldrh r2, [r1, #8]
	ldr r3, .L_0803e3b0
	ands r3, r2
	orrs r3, r0
	adds r2, r1, #0
	strh r3, [r2, #8]
	mov r3, r8
	ldrh r0, [r3, #24]
	movs r4, #24
	ldrsh r2, [r3, r4]
	ldrh r1, [r3, #16]
	movs r5, #16
	ldrsh r3, [r3, r5]
	cmp r2, r3
	beq .L_0803e3c0
	subs r3, r2, r3
	asrs r3, r3, #1
	cmp r3, #0
	beq .L_0803e3bc
	b .L_0803e3b4
	.2byte 0x0000
.L_0803e3a4:
	.4byte 0x000003ff
.L_0803e3a8:
	.4byte gFrameTick
.L_0803e3ac:
	.4byte Data_0805c9c4
.L_0803e3b0:
	.4byte 0xfffffc00
.L_0803e3b4:
	adds r3, r1, r3
	mov r0, r8
	strh r3, [r0, #16]
	b .L_0803e3c0
.L_0803e3bc:
	mov r1, r8
	strh r0, [r1, #16]
.L_0803e3c0:
	mov r2, r8
	mov r5, r8
	ldrh r1, [r2, #26]
	movs r3, #26
	ldrsh r2, [r2, r3]
	movs r4, #18
	ldrsh r3, [r5, r4]
	adds r0, r3, #0
	cmp r2, r3
	beq .L_0803e3ec
	subs r3, r2, r3
	asrs r3, r3, #1
	cmp r3, #0
	beq .L_0803e3e6
	adds r3, r0, r3
	mov r0, r8
	strh r3, [r0, #18]
	adds r0, r3, #0
	b .L_0803e3ec
.L_0803e3e6:
	mov r2, r8
	strh r1, [r2, #18]
	adds r0, r1, #0
.L_0803e3ec:
	ldr r3, .L_0803e438
	ldr r1, .L_0803e43c
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #2
	ands r3, r2
	ldrb r3, [r1, r3]
	ldr r4, [sp, #8]
	adds r3, r3, r0
	subs r3, #32
	strb r3, [r4, #4]
	mov r5, r8
	ldrh r2, [r5, #16]
	ldr r3, .L_0803e434
	subs r2, #4
	ands r2, r3
	ldrh r1, [r4, #6]
	ldr r3, .L_0803e440
	ldr r0, [sp, #8]
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #6]
	movs r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0803e464
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #226
	add r3, r10
	ldrh r3, [r3]
	cmp r3, #1
	bne .L_0803e456
	b .L_0803e444
.L_0803e434:
	.4byte 0x000001ff
.L_0803e438:
	.4byte gFrameTick
.L_0803e43c:
	.4byte Data_0805ea0c
.L_0803e440:
	.4byte 0xfffffe00
.L_0803e444:
	ldr r1, [sp, #8]
	movs r2, #13
	ldrb r3, [r1, #5]
	negs r2, r2
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	strb r2, [r1, #5]
	b .L_0803e464
.L_0803e456:
	ldr r3, [sp, #8]
	ldr r4, [sp, #8]
	ldrb r2, [r3, #5]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	strb r3, [r4, #5]
.L_0803e464:
	ldr r0, [sp, #8]
	movs r1, #248
	bl Runtime_PushSlotEntry
.L_0803e46c:
	movs r2, #192
	lsls r2, r2, #2
	adds r2, #162
	add r2, r10
	ldrh r3, [r2]
	add sp, #24
	adds r3, #1
	strh r3, [r2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
