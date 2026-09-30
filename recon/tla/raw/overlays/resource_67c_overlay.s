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
	bl Func_02003cd4
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
	bl Func_02003cd4
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
	bl Func_02003cd4
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
	bl Func_02003cc4
	ldr r2, .L_02008330
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_02003ccc
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
	bl Func_02003cc4
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02003ccc
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
	.4byte Data_020044f8
.L_02008334:
	.4byte Func_02000124
.L_02008338:
	.4byte 0xffff0000
	.section .text.x0200833c,"ax",%progbits
	.global Func_0200033c
	.thumb_func
Func_0200033c:
	push {lr}
	movs r0, #10
	movs r1, #32
	bl Func_02003e64
	pop {pc}
	.section .text.x0200835c,"ax",%progbits
	.global Func_0200035c
	.thumb_func
Func_0200035c:
	push {lr}
	ldr r3, .L_020083d4
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020083d8
	cmp r2, r3
	bne .L_02008374
	ldr r0, .L_020083dc
	b .L_020083d0
.L_02008374:
	ldr r3, .L_020083e0
	cmp r2, r3
	bne .L_0200837e
	ldr r0, .L_020083e4
	b .L_020083d0
.L_0200837e:
	ldr r3, .L_020083e8
	cmp r2, r3
	bne .L_02008388
	ldr r0, .L_020083ec
	b .L_020083d0
.L_02008388:
	ldr r3, .L_020083f0
	cmp r2, r3
	bne .L_02008392
	ldr r0, .L_020083f4
	b .L_020083d0
.L_02008392:
	ldr r3, .L_020083f8
	cmp r2, r3
	bne .L_0200839c
	ldr r0, .L_020083fc
	b .L_020083d0
.L_0200839c:
	ldr r3, .L_02008400
	cmp r2, r3
	bne .L_020083a6
	ldr r0, .L_02008404
	b .L_020083d0
.L_020083a6:
	ldr r3, .L_02008408
	cmp r2, r3
	bne .L_020083b0
	ldr r0, .L_0200840c
	b .L_020083d0
.L_020083b0:
	ldr r3, .L_02008410
	cmp r2, r3
	bne .L_020083ba
	ldr r0, .L_02008414
	b .L_020083d0
.L_020083ba:
	ldr r3, .L_02008418
	cmp r2, r3
	bne .L_020083c4
	ldr r0, .L_0200841c
	b .L_020083d0
.L_020083c4:
	ldr r3, .L_02008420
	cmp r2, r3
	bne .L_020083ce
	ldr r0, .L_02008424
	b .L_020083d0
.L_020083ce:
	ldr r0, .L_02008428
.L_020083d0:
	pop {pc}
	.2byte 0x0000
.L_020083d4:
	.4byte gPartyState
.L_020083d8:
	.4byte 0x000000a0
.L_020083dc:
	.4byte Data_020046a0
.L_020083e0:
	.4byte 0x000000a1
.L_020083e4:
	.4byte Data_02004748
.L_020083e8:
	.4byte 0x000000a2
.L_020083ec:
	.4byte Data_020047a8
.L_020083f0:
	.4byte 0x000000a3
.L_020083f4:
	.4byte Data_02004808
.L_020083f8:
	.4byte 0x000000a4
.L_020083fc:
	.4byte Data_020048c8
.L_02008400:
	.4byte 0x000000a5
.L_02008404:
	.4byte Data_02004910
.L_02008408:
	.4byte 0x000000a6
.L_0200840c:
	.4byte Data_020049e8
.L_02008410:
	.4byte 0x000000a7
.L_02008414:
	.4byte Data_02004a00
.L_02008418:
	.4byte 0x000000a8
.L_0200841c:
	.4byte Data_02004a48
.L_02008420:
	.4byte 0x000000a9
.L_02008424:
	.4byte Data_02004b98
.L_02008428:
	.4byte Data_02004688
	.section .text.x0200842c,"ax",%progbits
	.global Func_0200042c
	.thumb_func
Func_0200042c:
	push {r5, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	sub sp, #8
	bl Object_GetById
	adds r2, r0, #0
	cmp r5, #10
	bne .L_02008486
	ldr r3, [r2, #16]
	asrs r5, r3, #20
	cmp r5, #12
	bne .L_020084ac
	adds r1, r2, #0
	adds r1, #85
	movs r3, #0
	strb r3, [r1]
	ldr r1, .L_020084b0
	ldr r3, [r2, #12]
	movs r0, #144
	adds r3, r3, r1
	lsls r0, r0, #4
	str r3, [r2, #12]
	adds r0, #90
	bl GameFlag_SetBit
	movs r3, #9
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003d1c
	movs r3, #13
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003d1c
	b .L_020084ac
.L_02008486:
	cmp r5, #11
	bne .L_020084ac
	ldr r3, [r2, #8]
	asrs r3, r3, #20
	cmp r3, #51
	bne .L_020084ac
	adds r1, r2, #0
	adds r1, #85
	movs r3, #0
	strb r3, [r1]
	ldr r1, .L_020084b0
	ldr r3, [r2, #12]
	movs r0, #144
	adds r3, r3, r1
	lsls r0, r0, #4
	str r3, [r2, #12]
	adds r0, #91
	bl GameFlag_SetBit
.L_020084ac:
	add sp, #8
	pop {r5, pc}
.L_020084b0:
	.4byte 0xffff0000
	.section .text.x020084b4,"ax",%progbits
	.global Func_020004b4
	.thumb_func
Func_020004b4:
	push {lr}
	bl Func_02003e6c
	pop {pc}
	.section .text.x020084bc,"ax",%progbits
	.global Func_020004bc
	.thumb_func
Func_020004bc:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	adds r5, r1, #0
	movs r2, #160
	lsls r2, r2, #1
	adds r0, r5, #0
	sub sp, #8
	adds r6, r3, r2
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	adds r0, r5, #0
	bl Func_02001e78
	movs r1, #3
	adds r0, r5, #0
	bl Object_SetModeById
	ldr r2, .L_02008570
	ldr r3, [r6, #12]
	movs r5, #0
	adds r3, r3, r2
	str r3, [r6, #12]
	bl Func_02003d54
	bl Func_02003cdc
	movs r0, #12
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	adds r7, r3, #0
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02003ef4
	subs r7, #42
.L_0200851a:
	lsls r0, r5, #16
	bl Func_02003460
	movs r1, #0
	adds r0, r7, #0
	bl Func_0200349c
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #4
	adds r5, #1
	bl WaitFrames
	cmp r5, #31
	ble .L_0200851a
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200855e
	movs r3, #43
	movs r2, #44
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #41
	movs r1, #42
	movs r2, #1
	movs r3, #1
	bl Func_02003d1c
.L_0200855e:
	bl Func_02003d84
	movs r0, #140
	lsls r0, r0, #2
	bl GameFlag_SetBit
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008570:
	.4byte 0xffe00000
	.section .text.x02008574,"ax",%progbits
	.global Func_02000574
	.thumb_func
Func_02000574:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	adds r5, r1, #0
	movs r2, #160
	lsls r2, r2, #1
	adds r0, r5, #0
	adds r6, r3, r2
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	adds r0, r5, #0
	bl Func_02001e78
	adds r0, r5, #0
	movs r1, #3
	bl Object_SetModeById
	ldr r2, .L_020085f0
	ldr r3, [r6, #12]
	movs r5, #0
	adds r3, r3, r2
	str r3, [r6, #12]
	bl Func_02003d54
	bl Func_02003cdc
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02003ef4
.L_020085c2:
	lsls r0, r5, #16
	bl Func_02003460
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #4
	adds r5, #1
	bl WaitFrames
	cmp r5, #31
	ble .L_020085c2
	bl Func_02003d84
	movs r0, #153
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020085f0:
	.4byte 0xffe00000
	.global Data_020005f4
Data_020005f4:
	.4byte 0x00004770
	.section .text.x020085f8,"ax",%progbits
	.global Func_020005f8
	.thumb_func
Func_020005f8:
	push {lr}
	sub sp, #8
	movs r3, #43
	movs r2, #44
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #42
	movs r2, #1
	movs r3, #1
	movs r0, #41
	bl Func_02003d1c
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.section .text.x0200861c,"ax",%progbits
	.global Func_0200061c
	.thumb_func
Func_0200061c:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #85
	movs r3, #3
	strb r3, [r7]
.L_02008628:
	movs r0, #1
	bl WaitFrames
	ldr r5, [r6, #40]
	cmp r5, #0
	bne .L_02008628
	movs r0, #188
	bl Func_02003ef4
	movs r0, #10
	bl WaitFrames
	strb r5, [r7]
	pop {r5, r6, r7, pc}
	.section .text.x02008644,"ax",%progbits
	.global Func_02000644
	.thumb_func
Func_02000644:
	push {r5, r6, r7, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	sub sp, #8
	bl Object_GetById
	adds r6, r0, #0
	cmp r5, #9
	bne .L_0200868c
	ldr r3, [r6, #8]
	asrs r7, r3, #20
	cmp r7, #39
	bne .L_0200868c
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	adds r0, r6, #0
	bl Func_0200061c
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #92
	bl GameFlag_SetBit
	movs r0, #41
	movs r1, #9
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003d1c
	bl Func_02003d84
.L_0200868c:
	add sp, #8
	pop {r5, r6, r7, pc}
	.section .text.x02008690,"ax",%progbits
	.global Func_02000690
	.thumb_func
Func_02000690:
	push {lr}
	movs r1, #4
	movs r0, #8
	sub sp, #8
	bl Object_SetModeById
	movs r0, #8
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	movs r3, #110
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #123
	movs r1, #15
	movs r2, #5
	movs r3, #3
	bl Func_02003d14
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #181
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #88
	strh r3, [r2]
	add sp, #8
	pop {pc}
	.section .text.x020086d0,"ax",%progbits
	.global Func_020006d0
	.thumb_func
Func_020006d0:
	push {r5, lr}
	ldr r5, .L_02008714
	sub sp, #8
	ldr r3, [r5]
	subs r1, r3, #1
	str r1, [r5]
	adds r2, r1, #0
	cmp r1, #0
	bge .L_020086e4
	adds r2, r3, #6
.L_020086e4:
	movs r3, #1
	ands r3, r1
	lsls r0, r3, #2
	asrs r2, r2, #3
	adds r0, r0, r3
	movs r1, #47
	movs r3, #110
	subs r1, r1, r2
	str r3, [sp, #0]
	adds r0, #118
	movs r3, #2
	movs r2, #5
	str r1, [sp, #4]
	bl Func_02003d14
	ldr r3, [r5]
	cmp r3, #0
	bne .L_0200870e
	ldr r0, .L_02008718
	bl Scheduler_RemoveCallbackFar
.L_0200870e:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_02008714:
	.4byte Data_02004d84
.L_02008718:
	.4byte Func_020006d0
	.section .text.x0200871c,"ax",%progbits
	.global Func_0200071c
	.thumb_func
Func_0200071c:
	push {lr}
	sub sp, #8
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	ldr r3, .L_020087a0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #128
	ldr r0, [r3]
	movs r2, #0
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #151
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003ef4
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_020087a4
	bl Scheduler_AddOrUpdateCallback
	movs r0, #160
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_02003e44
	movs r0, #194
	movs r1, #1
	movs r2, #162
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02003e54
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_02003d84
	movs r0, #144
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r3, #46
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #59
	movs r1, #16
	movs r2, #5
	movs r3, #32
	bl Func_02003d1c
	add sp, #8
	pop {pc}
	.2byte 0x0000
.L_020087a0:
	.4byte gPartyState
.L_020087a4:
	.4byte Func_020006d0
	.section .text.x020087a8,"ax",%progbits
	.global Func_020007a8
	.thumb_func
Func_020007a8:
	push {lr}
	movs r1, #228
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02003eec
	movs r0, #20
	bl Object_GetById
	ldr r1, [r0, #80]
	movs r2, #1
	ldrb r3, [r1, #17]
	orrs r3, r2
	adds r2, r0, #0
	strb r3, [r1, #17]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #192
	lsls r3, r3, #13
	str r3, [r0, #12]
	pop {pc}
	.section .text.x020087d4,"ax",%progbits
	.global Func_020007d4
	.thumb_func
Func_020007d4:
	push {r5, r6, lr}
	sub sp, #8
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	ldr r3, .L_0200894c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #192
	ldr r0, [r3]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	bl Func_020007a8
	movs r0, #78
	bl Func_02003ef4
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #162
	bl Func_02003ef4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Func_02003d44
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Func_02003d44
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #10
	lsls r0, r0, #9
	bl Func_02003d44
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008950
	bl Scheduler_AddOrUpdateCallback
	movs r0, #160
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_02003e44
	movs r0, #156
	movs r1, #1
	movs r2, #252
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	bl Motion_CamBounds
	movs r0, #164
	bl Func_02003ef4
	movs r3, #80
	movs r2, #27
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #75
	movs r1, #96
	movs r2, #7
	movs r3, #13
	bl Func_02003d24
	movs r6, #31
	movs r5, #16
	movs r0, #91
	movs r1, #96
	movs r2, #7
	movs r3, #10
	str r6, [sp, #4]
	str r5, [sp, #0]
	bl Func_02003d24
	movs r3, #91
	str r3, [sp, #4]
	movs r0, #99
	movs r1, #96
	movs r2, #7
	movs r3, #14
	str r5, [sp, #0]
	bl Func_02003d24
	movs r3, #17
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #5
	movs r3, #9
	str r6, [sp, #4]
	bl Func_02003d1c
	movs r6, #0
.L_020088ca:
	movs r5, #0
.L_020088cc:
	movs r3, #1
	ldr r2, .L_02008954
	bics r3, r5
	adds r3, r6, r3
	lsls r3, r3, #1
	ldrb r0, [r2, r3]
	adds r3, #1
	ldrb r1, [r2, r3]
	movs r3, #80
	movs r2, #27
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #13
	movs r2, #7
	bl Func_02003d24
	adds r5, #1
	movs r0, #1
	bl WaitFrames
	cmp r5, #6
	ble .L_020088cc
	adds r6, #1
	cmp r6, #14
	ble .L_020088ca
	bl Func_02003e54
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02003d44
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02003ef4
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #80
	bl Func_02003ef4
	bl AudioCommand_WaitForCompletion
	bl Func_02003d84
	bl Func_02003ebc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #93
	bl GameFlag_SetBit
	movs r0, #68
	adds r0, #255
	bl GameFlag_SetBit
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200894c:
	.4byte gPartyState
.L_02008950:
	.4byte Func_020006d0
.L_02008954:
	.4byte Data_02003fb8
	.section .text.x02008958,"ax",%progbits
	.global Func_02000958
	.thumb_func
Func_02000958:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #8
	mov r8, r0
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	movs r0, #188
	bl Func_02003ef4
	movs r5, #1
	movs r6, #2
	movs r1, #96
	movs r2, #19
	movs r3, #29
	movs r0, #107
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003cf4
	movs r0, #8
	bl Battle_WaitMode0
	movs r1, #96
	movs r2, #19
	movs r3, #29
	movs r0, #108
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003cf4
	movs r0, #8
	bl Battle_WaitMode0
	movs r1, #98
	movs r2, #19
	movs r3, #29
	movs r0, #107
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003cf4
	movs r0, #8
	bl Battle_WaitMode0
	movs r1, #98
	movs r2, #19
	movs r3, #29
	movs r0, #108
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003cf4
	movs r0, #15
	bl Battle_WaitMode0
	ldr r5, .L_02008a20
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
	bl Func_02003ef4
	movs r1, #156
	movs r2, #248
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r5]
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #5
	bl Battle_WaitMode0
	mov r0, r8
	bl Func_02003e5c
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02008a20:
	.4byte gPartyState
	.section .text.x02008a24,"ax",%progbits
	.global Func_02000a24
	.thumb_func
Func_02000a24:
	push {r5, lr}
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #93
	bl GameFlag_Test
	ldr r5, .L_02008a94
	cmp r0, #0
	bne .L_02008a72
	adds r0, r5, #0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r1, #1
	adds r0, r5, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #228
	lsls r0, r0, #1
	bl PartyInventory_FindOwner
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_02008a8c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #173
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
	b .L_02008a8c
.L_02008a72:
	adds r0, r5, #0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #228
	lsls r0, r0, #1
	movs r1, #2
	bl Func_02003d64
	adds r0, r5, #2
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
.L_02008a8c:
	bl Func_02003d84
	pop {r5, pc}
	.2byte 0x0000
.L_02008a94:
	.4byte 0x000022a2
	.section .text.x02008a98,"ax",%progbits
	.global Func_02000a98
	.thumb_func
Func_02000a98:
	push {r5, lr}
	bl Func_02003628
	movs r1, #4
	movs r2, #16
	movs r3, #2
	movs r0, #16
	bl Func_02003688
	movs r0, #22
	movs r1, #4
	movs r2, #16
	movs r3, #0
	bl Func_02003688
	movs r5, #5
.L_02008ab8:
	movs r1, #55
	movs r0, #33
	bl Func_020036b8
	subs r5, #1
	movs r0, #15
	bl WaitFrames
	cmp r5, #0
	bge .L_02008ab8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #102
	bl GameFlag_SetBit
	pop {r5, pc}
	.section .text.x02008ad8,"ax",%progbits
	.global Func_02000ad8
	.thumb_func
Func_02000ad8:
	push {r5, r6, lr}
	sub sp, #8
	movs r3, #81
	str r3, [sp, #0]
	movs r6, #14
	movs r0, #106
	movs r1, #14
	movs r2, #14
	movs r3, #13
	str r6, [sp, #4]
	bl Func_02003d24
	movs r3, #78
	str r3, [sp, #4]
	movs r5, #17
	movs r0, #44
	movs r1, #78
	movs r2, #14
	movs r3, #12
	str r5, [sp, #0]
	bl Func_02003d24
	movs r1, #14
	movs r2, #14
	movs r3, #13
	movs r0, #81
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003d1c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #106
	bl GameFlag_SetBit
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008b24,"ax",%progbits
	.global Func_02000b24
	.thumb_func
Func_02000b24:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r6, r1, #0
	ldr r5, [r3, #32]
	bl Object_GetById
	cmp r0, #0
	beq .L_02008b58
	movs r2, #156
	lsls r2, r2, #1
	adds r3, r5, r2
	ldr r1, [r3]
	ldr r3, [r0, #16]
	ldr r2, [r0, #8]
	asrs r3, r3, #20
	lsls r3, r3, #7
	asrs r2, r2, #20
	adds r2, r2, r3
	lsls r2, r2, #2
	adds r1, r1, r2
	ldrb r2, [r1, #3]
	movs r3, #128
	orrs r3, r2
	strb r6, [r1, #2]
	strb r3, [r1, #3]
.L_02008b58:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008b5c,"ax",%progbits
	.global Func_02000b5c
	.thumb_func
Func_02000b5c:
	push {lr}
	movs r0, #12
	sub sp, #8
	bl Object_GetById
	ldr r2, [r0, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #43
	movs r2, #1
	movs r3, #1
	bl Func_02003d1c
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008b84,"ax",%progbits
	.global Func_02000b84
	.thumb_func
Func_02000b84:
	push {lr}
	movs r0, #12
	sub sp, #8
	bl Object_GetById
	ldr r2, [r0, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #42
	movs r2, #1
	movs r3, #1
	bl Func_02003d1c
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008bac,"ax",%progbits
	.global Func_02000bac
	.thumb_func
Func_02000bac:
	push {lr}
	movs r0, #12
	sub sp, #8
	bl Object_GetById
	ldr r2, [r0, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #41
	movs r2, #1
	movs r3, #1
	bl Func_02003d1c
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008bd4,"ax",%progbits
	.global Func_02000bd4
	.thumb_func
Func_02000bd4:
	push {r5, r6, lr}
	movs r0, #165
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008c24
	movs r1, #38
	movs r0, #12
	bl Func_02000b24
	bl Func_02003628
	movs r0, #12
	movs r1, #2
	movs r2, #45
	movs r3, #1
	bl Func_02003688
	movs r5, #3
.L_02008bfe:
	movs r0, #33
	movs r1, #55
	bl Func_020036b8
	subs r5, #1
	adds r6, r0, #0
	cmp r5, #0
	bge .L_02008bfe
	movs r0, #146
	lsls r0, r0, #2
	bl GameFlag_SetBit
	cmp r6, #1
	bne .L_02008c24
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #100
	bl GameFlag_SetBit
.L_02008c24:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008c28,"ax",%progbits
	.global Func_02000c28
	.thumb_func
Func_02000c28:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #160
	lsls r2, r2, #1
	movs r0, #9
	adds r6, r3, r2
	adds r5, r1, #0
	bl Object_GetById
	movs r1, #5
	bl Object_SetPartAttribute
	adds r0, r5, #0
	bl Func_02001e78
	adds r0, r5, #0
	movs r1, #3
	bl Object_SetModeById
	ldr r2, .L_02008ca4
	ldr r3, [r6, #12]
	movs r5, #31
	adds r3, r3, r2
	str r3, [r6, #12]
	bl Func_02003d54
	bl Func_02003cdc
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	movs r0, #151
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003ef4
.L_02008c78:
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #4
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02008c78
	bl Func_02003d84
	bl Func_02000b5c
	movs r0, #165
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008ca4:
	.4byte 0xffe00000
	.section .text.x02008ca8,"ax",%progbits
	.global Func_02000ca8
	.thumb_func
Func_02000ca8:
	push {lr}
	ldr r3, .L_02008cc4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #6
	movs r2, #0
	bl Func_02003df4
	bl Func_02003d84
	pop {pc}
	.2byte 0x0000
.L_02008cc4:
	.4byte gPartyState
	.section .text.x02008cc8,"ax",%progbits
	.global Func_02000cc8
	.thumb_func
Func_02000cc8:
	push {r5, r6, lr}
	ldr r3, .L_02008dcc
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008dd0
	sub sp, #12
	adds r6, r0, #0
	movs r5, #1
	cmp r2, r3
	bne .L_02008d00
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r6, #77
	bne .L_02008cf6
	cmp r3, #17
	bne .L_02008cf6
	movs r5, #0
.L_02008cf6:
	cmp r6, #78
	bne .L_02008d00
	cmp r3, #19
	bne .L_02008d00
	movs r5, #0
.L_02008d00:
	ldr r3, .L_02008dcc
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008dd4
	cmp r2, r3
	bne .L_02008d3a
	cmp r6, #77
	bne .L_02008d26
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #9
	bne .L_02008d26
	movs r5, #0
.L_02008d26:
	cmp r6, #78
	bne .L_02008d3a
	movs r0, #11
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #13
	bne .L_02008d3a
	movs r5, #0
.L_02008d3a:
	cmp r5, #0
	beq .L_02008dc6
	ldr r3, .L_02008dcc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	b .L_02008da8
.L_02008d5a:
	cmp r0, #0
	bge .L_02008d6c
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	adds r0, r6, #0
	bl Func_02000ca8
	b .L_02008dc6
.L_02008d6c:
	ldr r3, [r5, #16]
	movs r1, #128
	lsls r1, r1, #10
	adds r3, r3, r1
	str r3, [r5, #16]
	ldr r2, [r5, #8]
	ldr r3, .L_02008dd8
	ldr r1, .L_02008ddc
	ands r3, r2
	adds r3, r3, r1
	movs r1, #128
	lsls r1, r1, #9
	cmp r3, r1
	ble .L_02008d8c
	movs r3, #128
	lsls r3, r3, #9
.L_02008d8c:
	ldr r1, .L_02008de0
	cmp r3, r1
	bge .L_02008d94
	ldr r3, .L_02008de0
.L_02008d94:
	subs r3, r2, r3
	str r3, [r5, #8]
	ldrh r3, [r5, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r5, #6]
	movs r0, #1
	bl WaitFrames
.L_02008da8:
	ldr r3, [r5, #8]
	mov r1, sp
	str r3, [r1]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #12
	str r3, [r1, #4]
	adds r0, r5, #0
	ldr r3, [r5, #16]
	adds r3, r3, r2
	str r3, [r1, #8]
	bl Func_02003d2c
	cmp r0, #0
	ble .L_02008d5a
.L_02008dc6:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008dcc:
	.4byte gPartyState
.L_02008dd0:
	.4byte 0x000000a4
.L_02008dd4:
	.4byte 0x000000a5
.L_02008dd8:
	.4byte 0x000fffff
.L_02008ddc:
	.4byte 0xfff80000
.L_02008de0:
	.4byte 0xffff0000
	.section .text.x02008de4,"ax",%progbits
	.global Func_02000de4
	.thumb_func
Func_02000de4:
	push {r5, lr}
	bl Func_02003628
	movs r0, #44
	movs r1, #0
	movs r2, #34
	movs r3, #2
	bl Func_02003688
	movs r5, #4
.L_02008df8:
	movs r1, #55
	movs r0, #33
	bl Func_020036b8
	subs r5, #1
	movs r0, #15
	bl WaitFrames
	cmp r5, #0
	bge .L_02008df8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #103
	bl GameFlag_SetBit
	pop {r5, pc}
	.section .text.x02008e18,"ax",%progbits
	.global Func_02000e18
	.thumb_func
Func_02000e18:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_02008ec8
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	mov r8, r0
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	ldr r0, [r5]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	adds r3, r6, #0
	adds r3, #85
	movs r7, #0
	strb r7, [r3]
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #32
	orrs r3, r2
	strb r3, [r1]
	adds r0, r6, #0
	movs r1, #18
	bl Func_02003cc4
	movs r0, #215
	bl Func_02003ef4
	movs r0, #13
	bl WaitFrames
	adds r0, r6, #0
	movs r1, #9
	bl Animation_ApplyChildValues
	adds r0, r6, #0
	movs r1, #28
	bl Func_02003cc4
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #54
	bl Func_02003ef4
	movs r5, #0
.L_02008e86:
	cmp r5, #30
	bne .L_02008e8e
	bl Event_ClearStatus1c6
.L_02008e8e:
	movs r3, #128
	lsls r3, r3, #7
	adds r7, r7, r3
	ldr r3, [r6, #12]
	adds r3, r3, r7
	str r3, [r6, #12]
	movs r3, #7
	ands r3, r5
	cmp r3, #0
	bne .L_02008eac
	movs r0, #15
	bl Object_GetById
	bl Func_02002bd4
.L_02008eac:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	ble .L_02008e86
	bl Func_02003d84
	mov r0, r8
	bl Func_02003e5c
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008ec8:
	.4byte gPartyState
	.section .text.x02008ecc,"ax",%progbits
	.global Func_02000ecc
	.thumb_func
Func_02000ecc:
	push {r5, r6, lr}
	adds r0, r1, #0
	sub sp, #8
	bl Object_GetById
	adds r2, r0, #0
	ldr r3, [r2, #8]
	asrs r3, r3, #20
	cmp r3, #49
	beq .L_02008ee2
	b .L_02009002
.L_02008ee2:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #160
	lsls r1, r1, #1
	adds r6, r3, r1
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	ldr r3, .L_02009008
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #128
	ldr r0, [r3]
	movs r2, #0
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #151
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02003ef4
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	movs r3, #113
	movs r2, #39
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #114
	movs r1, #39
	movs r2, #1
	movs r3, #2
	bl Func_02003d24
	movs r5, #31
.L_02008f34:
	ldr r3, [r6, #12]
	ldr r1, .L_0200900c
	movs r0, #4
	adds r3, r3, r1
	str r3, [r6, #12]
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02008f34
	movs r5, #13
.L_02008f4a:
	adds r0, r5, #0
	bl Object_GetById
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #100
	movs r1, #0
	ldrsh r3, [r3, r1]
	adds r0, r5, #0
	lsls r3, r3, #16
	str r3, [r2, #8]
	adds r3, r2, #0
	adds r3, #102
	movs r1, #0
	ldrsh r3, [r3, r1]
	adds r1, r2, #0
	lsls r3, r3, #16
	str r3, [r2, #16]
	adds r1, #85
	movs r3, #0
	strb r3, [r1]
	ldr r3, .L_02009010
	movs r1, #3
	str r3, [r2, #20]
	str r3, [r2, #12]
	adds r5, #1
	bl ObjectMotion_SetActionVariant
	cmp r5, #15
	ble .L_02008f4a
	bl Func_02003d04
	movs r5, #12
.L_02008f8c:
	ldr r1, .L_02009014
	ldr r0, .L_02009018
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_02008fc0
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	lsls r2, r2, #2
	strh r3, [r1]
	movs r3, #128
	adds r2, r2, r1
	lsls r3, r3, #5
	adds r2, #4
	orrs r3, r5
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2]
.L_02008fc0:
	strh r4, [r0]
	movs r0, #4
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02008f8c
	movs r5, #41
	movs r0, #105
	movs r1, #41
	movs r2, #12
	movs r3, #13
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003d1c
	movs r3, #105
	str r3, [sp, #0]
	movs r0, #105
	movs r1, #105
	movs r2, #12
	movs r3, #13
	str r5, [sp, #4]
	bl Func_02003d24
	bl Func_02003cfc
	bl Func_02003d84
	movs r0, #142
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_02009002:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009008:
	.4byte gPartyState
.L_0200900c:
	.4byte 0xffff0000
.L_02009010:
	.4byte 0xffe00000
.L_02009014:
	.4byte Data_020038e0
.L_02009018:
	.4byte 0x04000208
	.section .text.x0200901c,"ax",%progbits
	.global Func_0200101c
	.thumb_func
Func_0200101c:
	push {r5, r6, lr}
	adds r6, r1, #0
	adds r3, r6, #0
	subs r3, #13
	sub sp, #8
	cmp r3, #2
	bhi .L_0200905a
	adds r0, r6, #0
	bl Object_GetById
	adds r5, r0, #0
	ldr r2, [r5, #8]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #47
	movs r1, #45
	movs r2, #1
	movs r3, #1
	bl Func_02003d1c
	movs r3, #145
	lsls r3, r3, #2
	adds r0, r6, r3
	bl GameFlag_SetBit
	adds r5, #35
	movs r3, #0
	strb r3, [r5]
.L_0200905a:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02009060,"ax",%progbits
	.global Func_02001060
	.thumb_func
Func_02001060:
	push {lr}
	sub sp, #8
	movs r3, #8
	movs r2, #34
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #8
	movs r1, #63
	movs r2, #7
	movs r3, #1
	bl Func_02003d1c
	add sp, #8
	pop {pc}
	.section .text.x0200907c,"ax",%progbits
	.global Func_0200107c
	.thumb_func
Func_0200107c:
	push {r5, r6, lr}
	subs r1, #10
	sub sp, #8
	cmp r1, #1
	bhi .L_020090ca
	movs r0, #10
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	movs r1, #33
	ldr r3, [r0, #8]
	movs r2, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	movs r6, #34
	movs r3, #1
	movs r0, #8
	str r6, [sp, #4]
	bl Func_02003d1c
	movs r0, #11
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #85
	strb r5, [r3]
	movs r1, #33
	ldr r3, [r0, #8]
	movs r2, #1
	asrs r3, r3, #20
	str r3, [sp, #0]
	movs r0, #8
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02003d1c
.L_020090ca:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020090d0,"ax",%progbits
	.global Func_020010d0
	.thumb_func
Func_020010d0:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r5, #60
.L_020090d6:
	cmp r5, #0
	beq .L_020090e8
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #40]
	subs r5, #1
	cmp r3, #0
	bne .L_020090d6
.L_020090e8:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020090ec,"ax",%progbits
	.global Func_020010ec
	.thumb_func
Func_020010ec:
	push {r5, lr}
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	ldr r5, .L_02009134
	movs r1, #1
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #126
	bl Func_02003ef4
	movs r0, #186
	lsls r0, r0, #2
	movs r1, #0
	adds r0, #255
	bl Func_02003eb4
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #161
	lsls r0, r0, #1
	adds r5, #1
	bl GameFlag_ClearBit
	adds r0, r5, #0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02003d84
	pop {r5, pc}
	.2byte 0x0000
.L_02009134:
	.4byte 0x00001a92
	.section .text.x02009138,"ax",%progbits
	.global Func_02001138
	.thumb_func
Func_02001138:
	push {r5, r6, lr}
	adds r0, r1, #0
	sub sp, #8
	bl Object_GetById
	movs r3, #20
	str r3, [sp, #0]
	adds r6, r0, #0
	movs r5, #39
	movs r0, #17
	movs r1, #39
	movs r2, #2
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003d1c
	ldr r3, [r6, #8]
	movs r0, #23
	asrs r3, r3, #20
	str r3, [sp, #0]
	movs r1, #39
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003d1c
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02009170,"ax",%progbits
	.global Func_02001170
	.thumb_func
Func_02001170:
	push {r5, r6, lr}
	movs r0, #166
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020091d2
	movs r0, #12
	movs r1, #38
	bl Func_02000b24
	movs r0, #13
	movs r1, #39
	bl Func_02000b24
	movs r1, #37
	movs r0, #15
	bl Func_02000b24
	bl Func_02003628
	movs r0, #38
	movs r1, #2
	movs r2, #19
	movs r3, #1
	bl Func_02003688
	movs r5, #8
.L_020091aa:
	movs r0, #33
	movs r1, #55
	bl Func_020036b8
	subs r5, #1
	adds r6, r0, #0
	cmp r5, #0
	bge .L_020091aa
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #74
	bl GameFlag_SetBit
	cmp r6, #1
	bne .L_020091d2
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_SetBit
.L_020091d2:
	pop {r5, r6, pc}
	.section .text.x020091d4,"ax",%progbits
	.global Func_020011d4
	.thumb_func
Func_020011d4:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	adds r5, r1, #0
	movs r2, #160
	lsls r2, r2, #1
	adds r0, r5, #0
	adds r6, r3, r2
	bl Object_GetById
	movs r1, #5
	bl Object_SetPartAttribute
	adds r0, r5, #0
	bl Func_02001e78
	adds r0, r5, #0
	movs r1, #3
	bl Object_SetModeById
	ldr r2, .L_02009248
	ldr r3, [r6, #12]
	movs r5, #31
	adds r3, r3, r2
	str r3, [r6, #12]
	bl Func_02003d54
	bl Func_02003cdc
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02003ef4
.L_02009222:
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #4
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02009222
	bl Func_02003d84
	movs r0, #166
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	pop {r5, r6, pc}
.L_02009248:
	.4byte 0xffe00000
	.section .text.x0200924c,"ax",%progbits
	.global Func_0200124c
	.thumb_func
Func_0200124c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #160
	lsls r2, r2, #1
	adds r2, r2, r3
	sub sp, #8
	mov r8, r2
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	mov r2, r8
	ldr r3, [r2, #12]
	ldr r2, .L_02009324
	movs r7, #14
	adds r3, r3, r2
	mov r2, r8
	str r3, [r2, #12]
	bl Func_02003cdc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_02003d44
.L_0200928e:
	movs r1, #8
	movs r2, #7
	movs r3, #6
	movs r5, #77
	movs r6, #8
	movs r0, #70
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003d24
	movs r0, #1
	bl WaitFrames
	movs r0, #50
	movs r1, #8
	movs r2, #7
	movs r3, #6
	str r5, [sp, #0]
	str r6, [sp, #4]
	subs r7, #1
	bl Func_02003d24
	movs r0, #1
	bl WaitFrames
	cmp r7, #0
	bge .L_0200928e
	movs r0, #10
	bl WaitFrames
	movs r7, #127
.L_020092cc:
	mov r2, r8
	ldr r3, [r2, #12]
	movs r2, #128
	lsls r2, r2, #7
	adds r3, r3, r2
	mov r2, r8
	str r3, [r2, #12]
	movs r0, #1
	subs r7, #1
	bl WaitFrames
	cmp r7, #0
	bge .L_020092cc
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02003d44
	bl Func_02003d84
	movs r3, #13
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #9
	movs r2, #7
	movs r3, #6
	movs r0, #38
	bl Func_02003d1c
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #73
	bl GameFlag_SetBit
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009324:
	.4byte 0xffe00000
	.section .text.x02009328,"ax",%progbits
	.global Func_02001328
	.thumb_func
Func_02001328:
	push {lr}
	sub sp, #8
	movs r3, #77
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #70
	movs r1, #8
	movs r2, #7
	movs r3, #6
	bl Func_02003d24
	movs r3, #13
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #17
	movs r2, #7
	movs r3, #6
	movs r0, #38
	bl Func_02003d1c
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #73
	bl GameFlag_ClearBit
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02009364,"ax",%progbits
	.global Func_02001364
	.thumb_func
Func_02001364:
	push {r5, lr}
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	movs r0, #168
	lsls r0, r0, #2
	bl Func_02003cd4
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
	bl Func_02003cc4
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	ldr r1, .L_020093b8
	adds r0, r5, #0
	bl Func_02003ccc
	pop {r5, pc}
.L_020093b8:
	.4byte Data_02003fd8
	.section .text.x020093bc,"ax",%progbits
	.global Func_020013bc
	.thumb_func
Func_020013bc:
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
	beq .L_0200946c
	ldr r3, [r6, #76]
	movs r2, #128
	lsls r2, r2, #10
	movs r0, #1
	cmp r3, r2
	beq .L_0200946c
	adds r0, r6, #0
	bl Func_02001364
	ldr r2, .L_02009478
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
	bl Func_02001364
	ldr r3, [r6, #76]
	movs r0, #1
	add r3, r9
	str r3, [r6, #76]
.L_0200946c:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009478:
	.4byte 0xfffc0000
	.section .text.x0200947c,"ax",%progbits
	.global Func_0200147c
	.thumb_func
Func_0200147c:
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
	ldr r6, .L_02009518
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
	beq .L_020094f4
	ldr r3, .L_0200951c
	movs r2, #1
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_020094f4
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
.L_020094f4:
	mov r3, r8
	ldrh r2, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	movs r0, #0
	cmp r3, #0
	beq .L_02009512
	ldrb r3, [r5]
	cmp r3, #141
	bne .L_02009510
	adds r3, r2, #0
	subs r3, #128
	mov r1, r8
	strh r3, [r1]
.L_02009510:
	movs r0, #1
.L_02009512:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009518:
	.4byte IwramMulQ16
.L_0200951c:
	.4byte Data_0300122c
	.section .text.x02009520,"ax",%progbits
	.global Func_02001520
	.thumb_func
Func_02001520:
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
	.section .text.x0200956c,"ax",%progbits
	.global Func_0200156c
	.thumb_func
Func_0200156c:
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
	.section .text.x02009598,"ax",%progbits
	.global Func_02001598
	.thumb_func
Func_02001598:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200961c
	sub sp, #68
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_02009612
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
.L_02009612:
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200961c:
	.4byte Data_0300122c
	.section .text.x02009620,"ax",%progbits
	.global Func_02001620
	.thumb_func
Func_02001620:
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
	bl Func_02003ef4
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl Func_02003e44
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
	bl Func_02003ef4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl Func_02003d44
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02003e7c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_02003e74
	movs r0, #60
	bl Func_02003e84
	adds r2, r7, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	str r3, [r7, #52]
	str r3, [r7, #48]
	ldr r1, .L_020096f4
	mov r0, r9
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #194
	bl Func_02003ef4
	movs r0, #45
	bl Battle_WaitMode0
	movs r1, #128
	mov r0, r9
	lsls r1, r1, #1
	bl Func_02003e04
	ldr r3, .L_020096ec
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_020096f0
	subs r2, #2
	strh r3, [r2]
	movs r0, #0
	mov r8, r0
	b .L_020096f8
	.2byte 0x0000
.L_020096ec:
	.4byte 0x00001008
.L_020096f0:
	.4byte 0x00003f10
.L_020096f4:
	.4byte Data_02004034
.L_020096f8:
	movs r0, #168
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	lsls r0, r0, #2
	bl Func_02003cd4
	adds r5, r0, #0
	movs r0, #246
	bl Func_02003ef4
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
	bl Func_02003cc4
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	ldr r1, .L_020097b8
	adds r0, r5, #0
	bl Func_02003ccc
	movs r6, #128
	ldr r3, .L_020097ac
	ldr r5, .L_020097b0
	lsls r6, r6, #19
	adds r6, #82
	strh r3, [r6]
	movs r0, #2
	bl WaitFrames
	movs r0, #2
	strh r5, [r6]
	bl WaitFrames
	ldr r3, .L_020097b4
	movs r0, #2
	strh r3, [r6]
	bl WaitFrames
	strh r5, [r6]
	movs r0, #2
	bl WaitFrames
	movs r2, #1
	add r8, r2
	mov r3, r8
	b .L_020097bc
.L_020097ac:
	.4byte 0x00001004
.L_020097b0:
	.4byte 0x0000100a
.L_020097b4:
	.4byte 0x00001010
.L_020097b8:
	.4byte Data_02003fe8
.L_020097bc:
	cmp r3, #16
	bne .L_020096f8
	ldr r3, .L_020097fc
	movs r0, #30
	strh r3, [r6]
	bl WaitFrames
	movs r0, #0
	mov r8, r0
.L_020097ce:
	mov r0, r10
	ldr r3, [r0, #16]
	mov r2, r10
	movs r0, #168
	ldr r1, [r2, #8]
	lsls r0, r0, #2
	ldr r2, [r2, #12]
	bl Func_02003cd4
	adds r5, r0, #0
	movs r0, #195
	bl Func_02003ef4
	bl Random16Far
	ldr r3, .L_02009800
	movs r2, #128
	ands r0, r3
	lsls r2, r2, #8
	adds r6, r5, #0
	adds r0, r0, r2
	adds r6, #100
	b .L_02009804
.L_020097fc:
	.4byte 0x00001008
.L_02009800:
	.4byte 0x00007fff
.L_02009804:
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
	ldr r7, .L_02009858
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
	b .L_0200985c
.L_02009858:
	.4byte 0x00000000
.L_0200985c:
	bl Func_02003cc4
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
	ldr r1, .L_02009954
	bl Func_02003ccc
	mov r0, r8
	cmp r0, #3
	bne .L_020098b0
	movs r1, #128
	mov r0, r11
	lsls r1, r1, #1
	bl Func_02003e04
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
	ldr r1, .L_02009958
	bl ObjectMotion_EnableActionAndSetCallback
.L_020098b0:
	movs r0, #8
	bl WaitFrames
	movs r3, #1
	add r8, r3
	mov r0, r8
	cmp r0, #16
	beq .L_020098c2
	b .L_020097ce
.L_020098c2:
	movs r0, #220
	bl Func_02003ef4
	movs r0, #16
	bl WaitFrames
	movs r2, #2
	mov r8, r2
.L_020098d2:
	mov r3, r10
	ldr r1, [r3, #8]
	ldr r3, [r3, #12]
	mov r0, r8
	lsls r2, r0, #16
	adds r2, r2, r3
	ldr r3, .L_0200995c
	mov r0, r10
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #168
	lsls r0, r0, #2
	bl Func_02003cd4
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
	ldr r1, .L_02009950
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
	bl Func_02003cc4
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	b .L_02009960
.L_02009950:
	.4byte 0x00000000
.L_02009954:
	.4byte Data_02004004
.L_02009958:
	.4byte Data_02004058
.L_0200995c:
	.4byte 0xfff80000
.L_02009960:
	adds r0, r5, #0
	ldr r1, .L_02009a0c
	bl Func_02003ccc
	movs r0, #2
	add r8, r0
	mov r2, r8
	cmp r2, #32
	bne .L_020098d2
	movs r0, #220
	bl Func_02003ef4
	movs r0, #50
	bl WaitFrames
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02003e74
	movs r0, #8
	bl Func_02003e84
	movs r0, #16
	bl WaitFrames
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Func_02003d44
	mov r0, r9
	movs r1, #0
	bl Func_02003e04
	mov r0, r11
	movs r1, #0
	bl Func_02003e04
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #62
	bl Func_02003ef4
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02003e74
	movs r0, #80
	bl Func_02003e84
	mov r0, r9
	ldr r1, .L_02009a10
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009a14
	mov r0, r11
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r3, .L_02009a18
	mov r0, r10
	str r3, [r0, #108]
	movs r0, #120
	bl WaitFrames
	mov r2, r10
	movs r0, #195
	str r6, [r2, #108]
	lsls r0, r0, #1
	bl Func_02003ef4
	bl Func_02003ebc
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
.L_02009a0c:
	.4byte Data_02004014
.L_02009a10:
	.4byte Data_0200407c
.L_02009a14:
	.4byte Data_020040ac
.L_02009a18:
	.4byte Func_02001598
	.section .text.x02009a1c,"ax",%progbits
	.global Func_02001a1c
	.thumb_func
Func_02001a1c:
	push {r5, r6, lr}
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	ldr r6, .L_02009c58
	movs r1, #1
	adds r0, r6, #0
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #174
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009a46
	bl Func_02003d84
	b .L_02009c56
.L_02009a46:
	adds r0, r6, #0
	bl Func_02003e14
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	ldr r5, .L_02009c5c
	adds r3, #1
	strh r3, [r2]
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r1, [r5]
	movs r0, #7
	bl Func_02003de4
	movs r0, #1
	bl Battle_WaitMode0
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #137
	lsls r1, r1, #1
	movs r2, #184
	movs r0, #7
	bl ObjectMotion_ResetAndSetPositionInMode2
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	bl Func_02003e1c
	movs r1, #3
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #248
	movs r2, #184
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	ldr r0, [r5]
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #7
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #132
	movs r0, #7
	lsls r1, r1, #1
	movs r2, #168
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r6, #2
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #14
	movs r1, #7
	bl Func_02001620
	movs r1, #138
	movs r0, #7
	bl UiText_DrawQuantityPairWithCue
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #174
	bl GameFlag_SetBit
	movs r0, #107
	bl Func_02003ef4
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r5]
	bl Func_02003e34
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_02003e34
	bl Func_0200124c
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02003ef4
	movs r0, #30
	bl Battle_WaitMode0
	ldr r0, .L_02009c60
	bl Func_02003e14
	movs r1, #6
	adds r1, #255
	movs r2, #20
	movs r0, #7
	bl Func_02003e34
	movs r2, #0
	movs r1, #7
	ldr r0, [r5]
	bl Object_LinkPair
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	bl Func_02003e1c
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #7
	bl Func_02003e34
	movs r1, #0
	movs r0, #7
	bl Func_02003e1c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_02009c64
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009c42
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_02009c42:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02003ddc
	bl Func_02003d84
.L_02009c56:
	pop {r5, r6, pc}
.L_02009c58:
	.4byte 0x0000229d
.L_02009c5c:
	.4byte gPartyState
.L_02009c60:
	.4byte 0x000022a0
.L_02009c64:
	.4byte 0x00013333
	.section .text.x02009c68,"ax",%progbits
	.global Func_02001c68
	.thumb_func
Func_02001c68:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #174
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009c90
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	ldr r0, .L_02009cd0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02003d84
	b .L_02009cce
.L_02009c90:
	ldr r5, .L_02009cd4
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #248
	movs r2, #152
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #248
	movs r2, #184
	bl ObjectMotion_SetPositionAndReset
	movs r1, #132
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02001a1c
.L_02009cce:
	pop {r5, pc}
.L_02009cd0:
	.4byte 0x0000229d
.L_02009cd4:
	.4byte gPartyState
	.section .text.x02009cd8,"ax",%progbits
	.global Func_02001cd8
	.thumb_func
Func_02001cd8:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #174
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009d00
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	ldr r0, .L_02009d38
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02003d84
	b .L_02009d36
.L_02009d00:
	ldr r5, .L_02009d3c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #140
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_SetPositionAndReset
	movs r1, #132
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02001a1c
.L_02009d36:
	pop {r5, pc}
.L_02009d38:
	.4byte 0x0000229d
.L_02009d3c:
	.4byte gPartyState
	.section .text.x02009d40,"ax",%progbits
	.global Func_02001d40
	.thumb_func
Func_02001d40:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #174
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009d68
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	ldr r0, .L_02009da0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02003d84
	b .L_02009d9c
.L_02009d68:
	ldr r5, .L_02009da4
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #248
	movs r2, #184
	bl ObjectMotion_SetPositionAndReset
	movs r1, #132
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02001a1c
.L_02009d9c:
	pop {r5, pc}
	.2byte 0x0000
.L_02009da0:
	.4byte 0x0000229d
.L_02009da4:
	.4byte gPartyState
	.section .text.x02009da8,"ax",%progbits
	.global Func_02001da8
	.thumb_func
Func_02001da8:
	push {lr}
	ldr r3, .L_02009e20
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009e24
	cmp r2, r3
	bne .L_02009dc0
	ldr r0, .L_02009e28
	b .L_02009e1c
.L_02009dc0:
	ldr r3, .L_02009e2c
	cmp r2, r3
	bne .L_02009dca
	ldr r0, .L_02009e30
	b .L_02009e1c
.L_02009dca:
	ldr r3, .L_02009e34
	cmp r2, r3
	bne .L_02009dd4
	ldr r0, .L_02009e38
	b .L_02009e1c
.L_02009dd4:
	ldr r3, .L_02009e3c
	cmp r2, r3
	bne .L_02009dde
	ldr r0, .L_02009e40
	b .L_02009e1c
.L_02009dde:
	ldr r3, .L_02009e44
	cmp r2, r3
	bne .L_02009de8
	ldr r0, .L_02009e48
	b .L_02009e1c
.L_02009de8:
	ldr r3, .L_02009e4c
	cmp r2, r3
	bne .L_02009df2
	ldr r0, .L_02009e50
	b .L_02009e1c
.L_02009df2:
	ldr r3, .L_02009e54
	cmp r2, r3
	bne .L_02009dfc
	ldr r0, .L_02009e58
	b .L_02009e1c
.L_02009dfc:
	ldr r3, .L_02009e5c
	cmp r2, r3
	bne .L_02009e06
	ldr r0, .L_02009e60
	b .L_02009e1c
.L_02009e06:
	ldr r3, .L_02009e64
	cmp r2, r3
	bne .L_02009e10
	ldr r0, .L_02009e68
	b .L_02009e1c
.L_02009e10:
	ldr r3, .L_02009e6c
	cmp r2, r3
	bne .L_02009e1a
	ldr r0, .L_02009e70
	b .L_02009e1c
.L_02009e1a:
	ldr r0, .L_02009e74
.L_02009e1c:
	pop {pc}
	.2byte 0x0000
.L_02009e20:
	.4byte gPartyState
.L_02009e24:
	.4byte 0x000000a0
.L_02009e28:
	.4byte Data_02004c7c
.L_02009e2c:
	.4byte 0x000000a1
.L_02009e30:
	.4byte Data_02004d88
.L_02009e34:
	.4byte 0x000000a2
.L_02009e38:
	.4byte Data_02004e48
.L_02009e3c:
	.4byte 0x000000a3
.L_02009e40:
	.4byte Data_02004ed8
.L_02009e44:
	.4byte 0x000000a4
.L_02009e48:
	.4byte Data_02004fb0
.L_02009e4c:
	.4byte 0x000000a5
.L_02009e50:
	.4byte Data_0200507c
.L_02009e54:
	.4byte 0x000000a6
.L_02009e58:
	.4byte Data_020051b4
.L_02009e5c:
	.4byte 0x000000a7
.L_02009e60:
	.4byte Data_020051fc
.L_02009e64:
	.4byte 0x000000a8
.L_02009e68:
	.4byte Data_02005268
.L_02009e6c:
	.4byte 0x000000a9
.L_02009e70:
	.4byte Data_020052f8
.L_02009e74:
	.4byte Data_02004c70
	.section .text.x02009e78,"ax",%progbits
	.global Func_02001e78
	.thumb_func
Func_02001e78:
	push {r5, lr}
	bl Object_GetById
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009e9c
	movs r1, #126
	adds r1, #255
	ldr r0, [r5, #80]
	bl ResourceMetadata_Register
	movs r3, #0
	strb r3, [r0, #5]
	strb r3, [r0, #6]
	movs r1, #2
	adds r0, r5, #0
	bl Func_02003cc4
.L_02009e9c:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009ea0,"ax",%progbits
	.global Func_02001ea0
	.thumb_func
Func_02001ea0:
	push {r5, r6, r7, lr}
	ldr r3, .L_02009efc
	ldr r7, [r3]
	movs r3, #15
	ands r7, r3
	cmp r7, #0
	bne .L_02009efa
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	movs r0, #14
	adds r0, #255
	bl Func_02003cd4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009efa
	ldr r1, .L_02009f00
	ldr r6, [r5, #80]
	bl Func_02003ccc
	adds r3, r5, #0
	adds r3, #85
	adds r2, r5, #0
	strb r7, [r3]
	adds r2, #34
	movs r3, #1
	strb r3, [r2]
	cmp r6, #0
	beq .L_02009efa
	adds r0, r6, #0
	movs r1, #2
	bl Animation_ApplyChildArgument
	movs r3, #128
	ldrb r2, [r6, #9]
	lsls r3, r3, #7
	strh r3, [r6, #18]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r7, [r6, #26]
	strb r3, [r6, #9]
.L_02009efa:
	pop {r5, r6, r7, pc}
.L_02009efc:
	.4byte Data_0300122c
.L_02009f00:
	.4byte Data_020040dc
	.section .text.x02009f04,"ax",%progbits
	.global Func_02001f04
	.thumb_func
Func_02001f04:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009fc4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #85
	adds r6, r0, #0
	adds r3, r3, r6
	ldrb r2, [r3]
	mov r8, r3
	mov r10, r2
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	bl Event_SetStatus1c6
	adds r0, r6, #0
	movs r1, #9
	bl Animation_ApplyChildValues
	movs r3, #0
	mov r2, r8
	strb r3, [r2]
	movs r3, #160
	lsls r3, r3, #15
	str r3, [r6, #12]
	movs r7, #192
	lsls r7, r7, #10
	movs r5, #31
.L_02009f4e:
	ldr r3, [r6, #12]
	movs r2, #128
	subs r3, r3, r7
	str r3, [r6, #12]
	ldrh r3, [r6, #6]
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r6, #6]
	ldr r3, .L_02009fc8
	adds r0, r6, #0
	movs r1, #0
	adds r7, r7, r3
	subs r5, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02009f4e
	adds r0, r6, #0
	bl Func_02002bd4
	adds r0, r6, #0
	movs r1, #0
	bl Animation_ApplyChildValues
	ldr r5, .L_02009fc4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	mov r2, r8
	mov r3, r10
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r6, #40]
	movs r0, #215
	bl Func_02003ef4
	ldr r0, [r5]
	movs r1, #168
	movs r2, #200
	bl ObjectMotion_SetPositionAndCommit
	bl Func_02003d84
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009fc4:
	.4byte gPartyState
.L_02009fc8:
	.4byte 0xfffff334
	.section .text.x02009fcc,"ax",%progbits
	.global Func_02001fcc
	.thumb_func
Func_02001fcc:
	push {lr}
	bl Func_02003bcc
	cmp r0, #0
	beq .L_02009fde
	ldr r3, .L_0200a018
	cmp r0, r3
	beq .L_02009ff6
	b .L_0200a00e
.L_02009fde:
	movs r1, #1
	movs r0, #0
	bl Func_02003bb8
	movs r0, #1
	movs r1, #1
	bl Func_02003bb8
	movs r0, #170
	bl Func_02003ecc
	b .L_0200a00e
.L_02009ff6:
	movs r1, #0
	movs r0, #0
	bl Func_02003bb8
	movs r0, #1
	movs r1, #0
	bl Func_02003bb8
	movs r0, #1
	negs r0, r0
	bl Func_02003ecc
.L_0200a00e:
	ldr r0, .L_0200a01c
	bl Func_02003bd8
	pop {pc}
	.2byte 0x0000
.L_0200a018:
	.4byte 0x00034bbf
.L_0200a01c:
	.4byte 0x00034bc0
	.section .text.x0200a020,"ax",%progbits
	.global Func_02002020
	.thumb_func
Func_02002020:
	push {r5, r6, lr}
	ldr r2, .L_0200a304
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r1, [r3, r0]
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r2, #0
	ldrsh r6, [r3, r2]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	subs r0, #54
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	str r2, [r3]
	ldr r3, .L_0200a308
	sub sp, #8
	cmp r1, r3
	bne .L_0200a0a2
	adds r0, #132
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a06a
	movs r0, #8
	bl Func_02001e78
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	b .L_0200a076
.L_0200a06a:
	movs r0, #8
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
.L_0200a076:
	movs r0, #153
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a094
	movs r0, #9
	bl Func_02001e78
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	b .L_0200a2fe
.L_0200a094:
	movs r0, #9
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	b .L_0200a2fe
.L_0200a0a2:
	ldr r3, .L_0200a30c
	cmp r1, r3
	bne .L_0200a140
	movs r0, #144
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a0ea
	movs r3, #110
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #118
	movs r1, #15
	movs r2, #5
	movs r3, #32
	bl Func_02003d24
	movs r3, #46
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #16
	movs r0, #59
	movs r2, #5
	movs r3, #32
	bl Func_02003d1c
	movs r0, #8
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	b .L_0200a0f8
.L_0200a0ea:
	movs r0, #8
	bl Func_02001e78
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
.L_0200a0f8:
	movs r0, #9
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	movs r0, #144
	adds r3, #85
	movs r6, #0
	lsls r0, r0, #4
	strb r6, [r3]
	adds r0, #92
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a118
	b .L_0200a2fe
.L_0200a118:
	movs r1, #158
	movs r2, #152
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02003ddc
	movs r3, #39
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #41
	movs r1, #9
	movs r2, #1
	movs r3, #1
	bl Func_02003d1c
	str r6, [r5, #12]
	str r6, [r5, #20]
	b .L_0200a2fe
.L_0200a140:
	ldr r3, .L_0200a310
	cmp r1, r3
	bne .L_0200a152
	cmp r6, #7
	beq .L_0200a14c
	b .L_0200a2fe
.L_0200a14c:
	bl Func_02002ffc
	b .L_0200a2fe
.L_0200a152:
	ldr r3, .L_0200a314
	cmp r1, r3
	bne .L_0200a1e8
	movs r0, #10
	bl Func_02001e78
	movs r1, #3
	movs r0, #10
	bl Object_SetModeById
	movs r0, #11
	bl Func_02001e78
	movs r0, #11
	movs r1, #3
	bl Object_SetModeById
	subs r3, r6, #6
	cmp r3, #1
	bhi .L_0200a1b4
	movs r0, #165
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a1a8
	movs r0, #9
	bl Object_GetById
	movs r1, #5
	bl Object_SetPartAttribute
	movs r0, #9
	bl Func_02001e78
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	bl Func_02000b5c
	b .L_0200a1b4
.L_0200a1a8:
	movs r0, #9
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
.L_0200a1b4:
	movs r0, #13
	bl Object_GetById
	movs r5, #1
	adds r0, #98
	strb r5, [r0]
	movs r0, #14
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #64
	bl Object_GetById
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200a1d8
	b .L_0200a2fe
.L_0200a1d8:
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	ldr r3, .L_0200a318
	str r3, [r5, #20]
	str r3, [r5, #12]
	b .L_0200a2fe
.L_0200a1e8:
	ldr r3, .L_0200a31c
	cmp r1, r3
	bne .L_0200a208
	subs r3, r6, #5
	cmp r3, #1
	bls .L_0200a1f6
	b .L_0200a2fe
.L_0200a1f6:
	ldr r0, .L_0200a320
	ldr r1, .L_0200a324
	ldr r2, .L_0200a328
	bl Func_02003ab0
	movs r0, #170
	bl Func_02003ecc
	b .L_0200a2fe
.L_0200a208:
	ldr r3, .L_0200a32c
	cmp r1, r3
	bne .L_0200a284
	cmp r6, #5
	beq .L_0200a216
	cmp r6, #7
	bne .L_0200a22e
.L_0200a216:
	ldr r1, .L_0200a330
	ldr r2, .L_0200a328
	ldr r0, .L_0200a334
	bl Func_02003ab0
	movs r0, #170
	bl Func_02003ecc
	movs r0, #0
	movs r1, #10
	bl Func_0200107c
.L_0200a22e:
	movs r0, #142
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a248
	movs r0, #9
	bl Func_02001e78
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
.L_0200a248:
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	adds r5, r0, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r2, r5, #0
	adds r2, #89
	movs r3, #0
	strb r3, [r2]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	ldr r3, .L_0200a338
	movs r0, #10
	str r3, [r5, #108]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a2fe
	cmp r6, #13
	bne .L_0200a2fe
	bl Func_02001f04
	b .L_0200a2fe
.L_0200a284:
	ldr r3, .L_0200a33c
	cmp r1, r3
	bne .L_0200a2fe
	movs r0, #166
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a2b4
	movs r0, #9
	bl Object_GetById
	movs r1, #5
	bl Object_SetPartAttribute
	movs r0, #9
	bl Func_02001e78
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	b .L_0200a2c0
.L_0200a2b4:
	movs r0, #9
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
.L_0200a2c0:
	movs r0, #10
	bl Object_GetById
	movs r5, #1
	adds r0, #98
	strb r5, [r0]
	movs r0, #11
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #14
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #16
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #17
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #18
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
.L_0200a2fe:
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
.L_0200a304:
	.4byte gPartyState
.L_0200a308:
	.4byte 0x000000a0
.L_0200a30c:
	.4byte 0x000000a1
.L_0200a310:
	.4byte 0x000000a2
.L_0200a314:
	.4byte 0x000000a3
.L_0200a318:
	.4byte 0xffe00000
.L_0200a31c:
	.4byte 0x000000a4
.L_0200a320:
	.4byte Data_020040f4
.L_0200a324:
	.4byte Data_02004102
.L_0200a328:
	.4byte Func_02001fcc
.L_0200a32c:
	.4byte 0x000000a5
.L_0200a330:
	.4byte Data_02004114
.L_0200a334:
	.4byte Data_02004106
.L_0200a338:
	.4byte Func_02001ea0
.L_0200a33c:
	.4byte 0x000000a8
	.section .text.x0200a340,"ax",%progbits
	.global Func_02002340
	.thumb_func
Func_02002340:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r6, .L_0200a644
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r5, [r3, r2]
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r7, [r3, r2]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	mov r8, r3
	ldrb r2, [r1, #23]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #23]
	sub sp, #12
	bl Func_02003278
	ldr r3, .L_0200a648
	cmp r5, r3
	beq .L_0200a380
	b .L_0200a52a
.L_0200a380:
	subs r3, r7, #3
	cmp r3, #1
	bls .L_0200a38e
	cmp r7, #6
	beq .L_0200a38e
	cmp r7, #8
	bne .L_0200a39c
.L_0200a38e:
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	b .L_0200a4c8
.L_0200a39c:
	cmp r7, #5
	beq .L_0200a3a4
	cmp r7, #7
	bne .L_0200a462
.L_0200a3a4:
	movs r0, #12
	bl Object_GetById
	adds r6, r0, #0
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	movs r1, #0
	str r5, [r6, #20]
	str r5, [r6, #12]
	bl Func_02003cc4
	movs r0, #13
	bl Object_GetById
	adds r6, r0, #0
	adds r3, r6, #0
	adds r3, #85
	movs r0, #140
	strb r5, [r3]
	lsls r0, r0, #2
	str r5, [r6, #20]
	str r5, [r6, #12]
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200a422
	movs r3, #104
	movs r2, #40
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #104
	movs r1, #80
	movs r2, #16
	movs r3, #10
	bl Func_02003d24
	movs r3, #39
	movs r2, #105
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #110
	movs r1, #116
	movs r2, #18
	movs r3, #12
	bl Func_02003d24
	movs r3, #32
	movs r0, #64
	movs r1, #32
	movs r2, #64
	str r5, [sp, #0]
	str r3, [sp, #4]
	bl Func_02003d1c
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl Func_020032d4
	b .L_0200a440
.L_0200a422:
	movs r1, #4
	movs r0, #33
	movs r2, #0
	bl Func_020032d4
	movs r0, #12
	bl Object_GetById
	adds r6, r0, #0
	ldr r0, [r6, #8]
	movs r1, #0
	asrs r0, r0, #20
	subs r0, #42
	bl Func_0200349c
.L_0200a440:
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a4c8
	movs r3, #43
	movs r2, #44
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #41
	movs r1, #42
	movs r2, #1
	movs r3, #1
	bl Func_02003d1c
	b .L_0200a4c8
.L_0200a462:
	adds r3, r7, #0
	subs r3, #9
	cmp r3, #1
	bhi .L_0200a4c8
	movs r0, #153
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200a4be
	movs r3, #77
	str r3, [sp, #0]
	movs r5, #32
	movs r0, #77
	movs r1, #80
	movs r2, #23
	movs r3, #20
	str r5, [sp, #4]
	bl Func_02003d24
	movs r3, #12
	movs r2, #98
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #112
	movs r1, #96
	movs r2, #16
	movs r3, #20
	bl Func_02003d24
	movs r0, #64
	movs r1, #32
	movs r2, #64
	movs r3, #32
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003d1c
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl Func_020032d4
	b .L_0200a4c8
.L_0200a4be:
	movs r0, #33
	movs r1, #4
	movs r2, #0
	bl Func_020032d4
.L_0200a4c8:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #90
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a50a
	movs r1, #184
	movs r2, #200
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02003ddc
	movs r3, #9
	movs r5, #12
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003d1c
	movs r3, #13
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #12
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003d1c
.L_0200a50a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #91
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a51a
	b .L_0200abc4
.L_0200a51a:
	movs r1, #206
	movs r2, #216
	movs r0, #11
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02003ddc
	b .L_0200abc4
.L_0200a52a:
	ldr r3, .L_0200a64c
	cmp r5, r3
	bne .L_0200a558
	subs r3, r7, #1
	cmp r3, #1
	bhi .L_0200a542
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
.L_0200a542:
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	str r3, [r6, #20]
	str r3, [r6, #12]
	b .L_0200abc4
.L_0200a558:
	ldr r3, .L_0200a650
	cmp r5, r3
	bne .L_0200a654
	movs r0, #0
	bl Func_02003ea4
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #8
	bl Func_020032d4
	movs r0, #40
	movs r1, #8
	movs r2, #8
	bl Func_020032d4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #102
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a5cc
	bl Func_02003628
	movs r1, #4
	movs r2, #16
	movs r3, #2
	movs r0, #16
	bl Func_02003688
	movs r0, #22
	movs r1, #4
	movs r2, #16
	movs r3, #0
	bl Func_02003688
	movs r5, #5
.L_0200a5be:
	movs r0, #33
	movs r1, #55
	subs r5, #1
	bl Func_020036b8
	cmp r5, #0
	bge .L_0200a5be
.L_0200a5cc:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #93
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a62c
	movs r3, #80
	movs r2, #27
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #75
	movs r1, #96
	movs r2, #7
	movs r3, #13
	bl Func_02003d24
	movs r5, #16
	movs r6, #31
	movs r0, #91
	movs r1, #96
	movs r2, #7
	movs r3, #10
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003d24
	movs r3, #91
	str r3, [sp, #4]
	movs r0, #99
	movs r1, #96
	movs r2, #7
	movs r3, #14
	str r5, [sp, #0]
	bl Func_02003d24
	movs r3, #17
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #5
	movs r3, #9
	str r6, [sp, #4]
	bl Func_02003d1c
	bl Func_020007a8
	b .L_0200abc4
.L_0200a62c:
	movs r3, #17
	movs r2, #31
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #17
	movs r1, #32
	movs r2, #5
	movs r3, #1
	bl Func_02003d1c
	b .L_0200abc4
	.2byte 0x0000
.L_0200a644:
	.4byte gPartyState
.L_0200a648:
	.4byte 0x000000a0
.L_0200a64c:
	.4byte 0x000000a1
.L_0200a650:
	.4byte 0x000000a2
.L_0200a654:
	ldr r3, .L_0200a720
	cmp r5, r3
	beq .L_0200a65c
	b .L_0200a7cc
.L_0200a65c:
	movs r0, #0
	bl Func_02003ea4
	subs r3, r7, #6
	cmp r3, #1
	bls .L_0200a66a
	b .L_0200a7aa
.L_0200a66a:
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #5
	movs r2, #4
	movs r0, #12
	bl Func_02003420
	movs r1, #3
	movs r2, #4
	movs r0, #13
	bl Func_02003420
	movs r1, #4
	movs r2, #4
	movs r0, #14
	bl Func_02003420
	movs r0, #40
	movs r1, #4
	movs r2, #4
	bl Func_020032d4
	movs r0, #165
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a724
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #100
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a724
	mov r3, r8
	ldr r6, [r3, #32]
	movs r3, #71
	str r3, [sp, #0]
	movs r5, #39
	movs r0, #64
	movs r1, #64
	movs r2, #16
	movs r3, #13
	str r5, [sp, #4]
	bl Func_02003d24
	movs r3, #7
	str r3, [sp, #0]
	movs r0, #71
	movs r1, #39
	movs r2, #16
	movs r3, #15
	str r5, [sp, #4]
	bl Func_02003d1c
	bl Func_02000b84
	movs r3, #131
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #1
	strb r3, [r2]
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #10
	ldrh r2, [r0]
	movs r3, #255
	lsls r3, r3, #8
	mov r1, sp
	adds r3, #252
	adds r1, #10
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_0200a71c
	orrs r3, r2
	strh r3, [r0]
	b .L_0200a78c
	.2byte 0x0000
.L_0200a71c:
	.4byte 0x00000001
.L_0200a720:
	.4byte 0x000000a3
.L_0200a724:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #100
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a73e
	movs r0, #146
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a78c
.L_0200a73e:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #100
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a75a
	movs r1, #140
	movs r2, #178
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02003ddc
.L_0200a75a:
	movs r1, #38
	movs r0, #12
	bl Func_02000b24
	bl Func_02003628
	movs r0, #12
	movs r1, #2
	movs r2, #45
	movs r3, #1
	bl Func_02003688
	movs r5, #7
.L_0200a774:
	movs r0, #33
	movs r1, #55
	subs r5, #1
	bl Func_020036b8
	cmp r5, #0
	bge .L_0200a774
	movs r0, #165
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_0200a78c:
	movs r5, #12
.L_0200a78e:
	adds r0, r5, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	adds r5, #1
	strb r3, [r2]
	str r3, [r6, #20]
	str r3, [r6, #12]
	cmp r5, #14
	ble .L_0200a78e
	b .L_0200a7b6
.L_0200a7aa:
	movs r2, #4
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
.L_0200a7b6:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #106
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a7c6
	b .L_0200abc4
.L_0200a7c6:
	bl Func_02000ad8
	b .L_0200abc4
.L_0200a7cc:
	ldr r3, .L_0200aa50
	cmp r5, r3
	bne .L_0200a82c
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	movs r0, #40
	movs r1, #0
	movs r2, #0
	bl Func_020032d4
	movs r0, #0
	bl Func_02003ea4
	subs r3, r7, #1
	cmp r3, #3
	bls .L_0200a7fa
	cmp r7, #7
	beq .L_0200a7fa
	b .L_0200abc4
.L_0200a7fa:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #103
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a80a
	b .L_0200abc4
.L_0200a80a:
	bl Func_02003628
	movs r0, #44
	movs r1, #0
	movs r2, #34
	movs r3, #2
	bl Func_02003688
	movs r5, #0
.L_0200a81c:
	movs r0, #33
	movs r1, #55
	adds r5, #1
	bl Func_020036b8
	cmp r5, #4
	ble .L_0200a81c
	b .L_0200abc4
.L_0200a82c:
	ldr r3, .L_0200aa54
	cmp r5, r3
	bne .L_0200a900
	movs r0, #0
	bl Func_02003ea4
	cmp r7, #1
	beq .L_0200a840
	cmp r7, #13
	bne .L_0200a84e
.L_0200a840:
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	b .L_0200a866
.L_0200a84e:
	cmp r7, #9
	beq .L_0200a85a
	cmp r7, #11
	beq .L_0200a85a
	cmp r7, #12
	bne .L_0200a866
.L_0200a85a:
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
.L_0200a866:
	movs r0, #142
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a890
	movs r5, #13
.L_0200a874:
	adds r0, r5, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	adds r5, #1
	strb r3, [r2]
	str r3, [r6, #8]
	str r3, [r6, #16]
	cmp r5, #15
	ble .L_0200a874
	b .L_0200abc4
.L_0200a890:
	movs r3, #113
	movs r2, #39
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #114
	movs r1, #39
	movs r2, #1
	movs r3, #2
	bl Func_02003d24
	movs r5, #41
	movs r0, #105
	movs r1, #41
	movs r2, #12
	movs r3, #13
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003d1c
	movs r3, #105
	str r3, [sp, #0]
	movs r0, #105
	movs r1, #105
	movs r2, #12
	movs r3, #13
	str r5, [sp, #4]
	bl Func_02003d24
	movs r5, #13
.L_0200a8ca:
	adds r0, r5, #0
	bl Object_GetById
	movs r2, #145
	lsls r2, r2, #2
	adds r6, r0, #0
	adds r0, r5, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a8f8
	ldr r2, [r6, #8]
	ldr r3, [r6, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #47
	movs r1, #45
	movs r2, #1
	movs r3, #1
	bl Func_02003d1c
.L_0200a8f8:
	adds r5, #1
	cmp r5, #15
	ble .L_0200a8ca
	b .L_0200abc4
.L_0200a900:
	ldr r3, .L_0200aa58
	cmp r5, r3
	bne .L_0200a914
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	b .L_0200abc4
.L_0200a914:
	ldr r3, .L_0200aa5c
	cmp r5, r3
	bne .L_0200a93c
	movs r0, #0
	bl Func_02003ea4
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	bl Func_02003edc
	movs r0, #0
	movs r1, #8
	movs r2, #9
	bl Func_02003ee4
	b .L_0200abc4
.L_0200a93c:
	ldr r3, .L_0200aa60
	cmp r5, r3
	beq .L_0200a944
	b .L_0200ab46
.L_0200a944:
	subs r3, r7, #3
	cmp r3, #1
	bls .L_0200a94c
	b .L_0200aafe
.L_0200a94c:
	ldr r3, .L_0200aa64
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #6
	movs r2, #4
	movs r0, #10
	bl Func_02003420
	movs r1, #3
	movs r2, #4
	movs r0, #11
	bl Func_02003420
	movs r1, #5
	movs r2, #4
	movs r0, #12
	bl Func_02003420
	movs r1, #6
	movs r2, #4
	movs r0, #13
	bl Func_02003420
	movs r1, #5
	movs r2, #4
	movs r0, #14
	bl Func_02003420
	movs r1, #4
	movs r2, #4
	movs r0, #15
	bl Func_02003420
	movs r1, #4
	movs r2, #4
	movs r0, #16
	bl Func_02003420
	movs r1, #3
	movs r2, #4
	movs r0, #17
	bl Func_02003420
	movs r1, #6
	movs r2, #4
	movs r0, #18
	bl Func_02003420
	movs r0, #40
	movs r1, #4
	movs r2, #4
	bl Func_020032d4
	movs r0, #166
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200aa68
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200aa68
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #32]
	movs r3, #100
	str r3, [sp, #0]
	movs r5, #13
	movs r0, #64
	movs r1, #64
	movs r2, #19
	movs r3, #17
	str r5, [sp, #4]
	bl Func_02003d24
	movs r3, #35
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #105
	movs r2, #23
	movs r3, #24
	bl Func_02003d24
	movs r3, #36
	str r3, [sp, #0]
	movs r0, #100
	movs r1, #13
	movs r2, #19
	movs r3, #19
	str r5, [sp, #4]
	bl Func_02003d1c
	movs r3, #131
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #1
	strb r3, [r2]
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #10
	ldrh r2, [r0]
	movs r3, #255
	lsls r3, r3, #8
	mov r1, sp
	adds r3, #252
	adds r1, #10
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_0200aa4c
	orrs r3, r2
	strh r3, [r0]
	b .L_0200aafe
.L_0200aa4c:
	.4byte 0x00000001
.L_0200aa50:
	.4byte 0x000000a4
.L_0200aa54:
	.4byte 0x000000a5
.L_0200aa58:
	.4byte 0x000000a6
.L_0200aa5c:
	.4byte 0x000000a7
.L_0200aa60:
	.4byte 0x000000a8
.L_0200aa64:
	.4byte gPartyState
.L_0200aa68:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200aa84
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #74
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200aafe
.L_0200aa84:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200aabc
	movs r1, #202
	movs r2, #148
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02003ddc
	movs r1, #170
	movs r2, #180
	movs r0, #13
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02003ddc
	movs r1, #186
	movs r2, #212
	movs r0, #15
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02003ddc
.L_0200aabc:
	movs r0, #12
	movs r1, #38
	bl Func_02000b24
	movs r0, #13
	movs r1, #39
	bl Func_02000b24
	movs r1, #37
	movs r0, #15
	bl Func_02000b24
	bl Func_02003628
	movs r0, #38
	movs r1, #2
	movs r2, #19
	movs r3, #1
	bl Func_02003688
	movs r5, #7
.L_0200aae6:
	movs r0, #33
	movs r1, #55
	subs r5, #1
	bl Func_020036b8
	cmp r5, #0
	bge .L_0200aae6
	movs r0, #166
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_0200aafe:
	movs r5, #10
.L_0200ab00:
	adds r0, r5, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	adds r5, #1
	strb r3, [r2]
	str r3, [r6, #20]
	str r3, [r6, #12]
	cmp r5, #20
	ble .L_0200ab00
	movs r0, #19
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	movs r2, #39
	asrs r3, r3, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #23
	movs r1, #39
	movs r2, #1
	bl Func_02003d1c
	subs r3, r7, #1
	cmp r3, #1
	bhi .L_0200abc4
	movs r0, #0
	bl Func_02003ea4
	b .L_0200abc4
.L_0200ab46:
	ldr r3, .L_0200abd0
	cmp r5, r3
	bne .L_0200abc4
	movs r0, #0
	bl Func_02003ea4
	movs r2, #2
	negs r2, r2
	movs r0, #33
	movs r1, #0
	bl Func_020032d4
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #73
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ab8a
	movs r3, #13
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #9
	movs r2, #7
	movs r3, #6
	bl Func_02003d1c
	b .L_0200abc4
.L_0200ab8a:
	movs r3, #50
	str r3, [sp, #0]
	movs r5, #8
	movs r0, #77
	movs r1, #8
	movs r2, #7
	movs r3, #6
	str r5, [sp, #4]
	bl Func_02003d24
	movs r3, #77
	str r3, [sp, #0]
	movs r0, #70
	movs r1, #8
	movs r2, #7
	movs r3, #6
	str r5, [sp, #4]
	bl Func_02003d24
	movs r3, #13
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #17
	movs r2, #7
	movs r3, #6
	bl Func_02003d1c
.L_0200abc4:
	movs r0, #0
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200abd0:
	.4byte 0x000000a9
	.section .text.x0200abd4,"ax",%progbits
	.global Func_02002bd4
	.thumb_func
Func_02002bd4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	movs r0, #128
	lsls r0, r0, #11
	adds r2, r2, r0
	adds r3, r3, r0
	ldr r1, [r6, #8]
	movs r0, #14
	bl Func_02003cd4
	ldr r2, [r6, #80]
	adds r5, r0, #0
	mov r8, r2
	cmp r5, #0
	beq .L_0200ac30
	ldr r3, [r6, #20]
	ldr r7, [r5, #80]
	str r3, [r5, #20]
	ldr r1, .L_0200ac38
	bl Func_02003ccc
	adds r3, r5, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	cmp r7, #0
	beq .L_0200ac30
	movs r1, #1
	adds r0, r7, #0
	bl Animation_ApplyChildArgument
	strb r5, [r7, #26]
	mov r2, r8
	ldrb r3, [r2, #9]
	ldrb r1, [r7, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r7, #9]
.L_0200ac30:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ac38:
	.4byte Data_0200411c
	.section .text.x0200ac3c,"ax",%progbits
	.global Func_02002c3c
	.thumb_func
Func_02002c3c:
	push {r5, r6, r7, lr}
	ldr r3, .L_0200ac98
	ldr r7, [r3]
	movs r3, #15
	ands r7, r3
	cmp r7, #0
	bne .L_0200ac94
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	movs r0, #14
	adds r0, #255
	bl Func_02003cd4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200ac94
	ldr r1, .L_0200ac9c
	ldr r6, [r5, #80]
	bl Func_02003ccc
	adds r3, r5, #0
	adds r3, #85
	strb r7, [r3]
	ldr r3, .L_0200aca0
	adds r2, r5, #0
	str r3, [r5, #12]
	adds r2, #34
	movs r3, #1
	strb r3, [r2]
	cmp r6, #0
	beq .L_0200ac94
	adds r0, r6, #0
	movs r1, #2
	bl Animation_ApplyChildArgument
	ldrb r3, [r6, #9]
	movs r2, #13
	negs r2, r2
	ands r2, r3
	movs r3, #8
	orrs r2, r3
	strb r7, [r6, #26]
	strb r2, [r6, #9]
.L_0200ac94:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ac98:
	.4byte Data_0300122c
.L_0200ac9c:
	.4byte Data_02004128
.L_0200aca0:
	.4byte 0xfff88000
	.section .text.x0200aca4,"ax",%progbits
	.global Func_02002ca4
	.thumb_func
Func_02002ca4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r6, .L_0200adf0
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #133
	ldr r2, [r3, #108]
	lsls r0, r0, #2
	adds r3, r6, r0
	movs r1, #230
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r2, [r2]
	mov r8, r3
	mov r0, r8
	mov r10, r2
	sub sp, #12
	bl Object_GetById
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r6, r2
	adds r5, r0, #0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_0200adf4
	cmp r2, r3
	bne .L_0200ace8
	ldr r3, [r5, #20]
	cmp r3, #0
	beq .L_0200ade4
.L_0200ace8:
	ldr r3, [r5, #8]
	mov r7, sp
	str r3, [r7]
	movs r1, #128
	ldr r3, [r5, #12]
	lsls r1, r1, #10
	str r3, [r7, #4]
	adds r0, r5, #0
	ldr r3, [r5, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r7, #0
	bl Func_02003d2c
	ldr r3, .L_0200adf8
	movs r2, #4
	ldr r3, [r3]
	adds r6, r0, #0
	ands r3, r2
	cmp r3, #0
	bne .L_0200ad18
	adds r0, r5, #0
	bl Func_02002bd4
.L_0200ad18:
	cmp r6, #0
	bge .L_0200ad6a
	movs r1, #129
	mov r0, r8
	lsls r1, r1, #1
	bl Func_02003e3c
	ldr r3, [r5, #16]
	movs r0, #128
	lsls r0, r0, #12
	adds r3, r3, r0
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	adds r0, r5, #0
	bl Func_02003ce4
	adds r0, r5, #0
	movs r1, #49
	bl Func_02003cc4
	adds r0, r5, #0
	bl Func_02003cec
.L_0200ad46:
	movs r0, #1
	bl WaitFrames
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	cmp r2, r3
	bne .L_0200ad46
	adds r0, r5, #0
	bl Func_02002bd4
	adds r0, r5, #0
	movs r1, #49
	bl Func_02003cc4
	movs r0, #3
	bl WaitFrames
	b .L_0200ade4
.L_0200ad6a:
	ldr r3, [r5, #8]
	movs r1, #128
	str r3, [r7]
	lsls r1, r1, #12
	ldr r3, [r5, #12]
	adds r0, r5, #0
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r7, #0
	bl Func_02003d2c
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_0200ade4
	ldr r3, [r5, #8]
	ldr r2, .L_0200adfc
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r7]
	adds r1, r7, #0
	ldr r3, [r5, #12]
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r2
	str r3, [r7, #8]
	bl Func_02003d2c
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_0200ade4
	ldr r3, [r5, #8]
	ldr r2, .L_0200ae00
	ldr r0, .L_0200adfc
	adds r3, r3, r2
	str r3, [r7]
	adds r1, r7, #0
	ldr r3, [r5, #12]
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r0
	str r3, [r7, #8]
	adds r0, r5, #0
	bl Func_02003d2c
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_0200ade4
	mov r1, r10
	ldr r3, [r1, #16]
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r1, #16]
	ldr r3, [r5, #16]
	adds r3, r3, r2
	str r3, [r5, #16]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
.L_0200ade4:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200adf0:
	.4byte gPartyState
.L_0200adf4:
	.4byte 0x0000009e
.L_0200adf8:
	.4byte Data_0300122c
.L_0200adfc:
	.4byte 0x0005b333
.L_0200ae00:
	.4byte 0xfffa4ccd
	.section .text.x0200ae04,"ax",%progbits
	.global Func_02002e04
	.thumb_func
Func_02002e04:
	push {r5, lr}
	bl Object_GetById
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200ae30
	movs r1, #126
	adds r1, #255
	ldr r0, [r5, #80]
	bl ResourceMetadata_Register
	movs r3, #0
	strb r3, [r0, #5]
	strb r3, [r0, #6]
	movs r1, #0
	adds r0, r5, #0
	bl Func_02003cc4
	adds r0, r5, #0
	movs r1, #2
	bl Func_02003cc4
.L_0200ae30:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200ae34,"ax",%progbits
	.global Func_02002e34
	.thumb_func
Func_02002e34:
	push {r5, r6, r7, lr}
	ldr r5, .L_0200aecc
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	adds r7, r0, #0
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	ldr r0, [r5]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #32
	orrs r3, r2
	strb r3, [r1]
	movs r0, #215
	bl Func_02003ef4
	adds r0, r6, #0
	movs r1, #18
	bl Func_02003cc4
	movs r0, #153
	lsls r0, r0, #2
	bl Func_02003ef4
	movs r5, #0
.L_0200ae86:
	cmp r5, #30
	bne .L_0200ae8e
	bl Event_ClearStatus1c6
.L_0200ae8e:
	ldr r3, [r6, #12]
	ldr r2, .L_0200aed0
	adds r3, r3, r2
	str r3, [r6, #12]
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r6, #6]
	movs r3, #7
	ands r3, r5
	cmp r3, #0
	bne .L_0200aeb2
	movs r0, #15
	bl Object_GetById
	bl Func_02002bd4
.L_0200aeb2:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	ble .L_0200ae86
	bl Func_02003d84
	adds r0, r7, #0
	bl Func_02003e5c
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200aecc:
	.4byte gPartyState
.L_0200aed0:
	.4byte 0xffffc000
	.section .text.x0200aed4,"ax",%progbits
	.global Func_02002ed4
	.thumb_func
Func_02002ed4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200af5c
	sub sp, #56
	ldr r2, [r3]
	mov r8, r3
	movs r3, #1
	ands r3, r2
	adds r7, r0, #0
	cmp r3, #0
	beq .L_0200af50
	movs r3, #7
	add r6, sp, #16
	str r3, [r6, #4]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	bne .L_0200aefe
	movs r3, #5
	str r3, [r6, #4]
.L_0200aefe:
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	movs r5, #0
	str r3, [r6, #8]
	str r3, [r6, #12]
	str r5, [r6]
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r4, r0, #1
	adds r4, r4, r0
	lsls r3, r4, #4
	adds r4, r4, r3
	lsls r3, r4, #8
	adds r4, r4, r3
	mov r3, r8
	ldr r2, [r3]
	movs r3, #15
	ldr r0, [r7, #8]
	ands r2, r3
	movs r3, #8
	subs r3, r3, r2
	ldr r1, [r7, #12]
	lsls r3, r3, #16
	adds r0, r0, r3
	movs r3, #208
	lsls r3, r3, #13
	adds r1, r1, r3
	movs r3, #176
	lsls r3, r3, #12
	ldr r2, [r7, #16]
	negs r4, r4
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #0]
	str r5, [sp, #4]
	str r6, [sp, #12]
	bl Func_0200015c
.L_0200af50:
	movs r0, #0
	add sp, #56
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200af5c:
	.4byte Data_0300122c
	.section .text.x0200af60,"ax",%progbits
	.global Func_02002f60
	.thumb_func
Func_02002f60:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r5, .L_0200aff4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	mov r10, r0
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	movs r0, #228
	bl Func_02003ef4
	ldr r3, .L_0200aff8
	movs r2, #0
	str r3, [r6, #108]
	mov r8, r2
	adds r3, r6, #0
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	str r3, [r6, #48]
	movs r1, #2
	ldr r0, [r5]
	bl Object_SetModeById
	movs r2, #8
	negs r2, r2
	movs r1, #0
	ldr r0, [r5]
	bl ObjectMotion_OffsetPositionAndResetMotion
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #9
	bl Object_SetPartAttribute
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	mov r3, r8
	str r3, [r6, #108]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	mov r0, r10
	bl Func_02003e5c
	bl Func_02003d84
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200aff4:
	.4byte gPartyState
.L_0200aff8:
	.4byte Func_02002ed4
	.section .text.x0200affc,"ax",%progbits
	.global Func_02002ffc
	.thumb_func
Func_02002ffc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200b0e8
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	adds r7, r0, #0
	cmp r7, #0
	bne .L_0200b0e2
	bl Func_02003d7c
	movs r0, #0
	bl Func_02003eac
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r0, r0
	negs r1, r1
	movs r3, #0
	bl Motion_CamBounds
	movs r3, #85
	adds r3, r3, r6
	strb r7, [r3]
	mov r8, r3
	movs r2, #10
	ldrsh r1, [r6, r2]
	movs r3, #18
	ldrsh r2, [r6, r3]
	ldr r3, .L_0200b0ec
	lsls r2, r2, #16
	adds r2, r2, r3
	lsls r1, r1, #16
	ldr r0, [r5]
	bl Func_02003ddc
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #9
	bl Object_SetPartAttribute
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	bl Event_SetStatus1c6
	movs r0, #228
	bl Func_02003ef4
	ldr r3, .L_0200b0f0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	str r3, [r6, #108]
	ldr r0, [r5]
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r2, #8
	movs r1, #0
	ldr r0, [r5]
	bl ObjectMotion_CommitPositionAndActivate
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, [r6, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	movs r2, #10
	ldr r0, [r5]
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r3, #3
	mov r2, r8
	strb r3, [r2]
	str r7, [r6, #108]
	bl Func_02003ed4
	bl Event_WaitValue1c8Frames
	bl Func_02003d84
.L_0200b0e2:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200b0e8:
	.4byte gPartyState
.L_0200b0ec:
	.4byte 0xfff00000
.L_0200b0f0:
	.4byte Func_02002ed4
	.section .text.x0200b0f4,"ax",%progbits
	.global Func_020030f4
	.thumb_func
Func_020030f4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	sub sp, #16
	adds r2, r1, #0
	adds r2, #228
	ldr r3, [r2]
	ldr r0, .L_0200b268
	str r3, [sp, #12]
	ldr r4, [sp, #12]
	ldr r3, .L_0200b26c
	adds r5, r0, #4
	ands r4, r3
	str r4, [sp, #12]
	ldr r2, [r2, #4]
	ands r2, r3
	ldr r3, [r1]
	mov r11, r2
	ldr r3, [r3, #4]
	ldr r2, .L_0200b270
	mov r10, r3
	ldrh r3, [r0]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r3, r3, #5
	str r3, [sp, #0]
	movs r3, #31
	str r3, [sp, #4]
.L_0200b13c:
	ldrh r0, [r5, #18]
	adds r3, r0, #0
	cmp r3, #0
	bne .L_0200b146
	b .L_0200b24c
.L_0200b146:
	ldr r4, [r5, #12]
	ldr r1, [r5, #20]
	ldr r2, [r5, #4]
	mov r12, r4
	cmp r1, #0
	beq .L_0200b170
	ldr r3, [r1]
	ldr r4, [sp, #12]
	subs r6, r3, r4
	ldr r3, [r1, #4]
	mov r4, r11
	adds r3, r3, r2
	mov r2, r10
	subs r2, r3, r2
	ldr r3, [r1, #8]
	mov r1, r10
	subs r3, r3, r4
	subs r7, r3, r1
	mov r8, r2
	subs r4, r7, r2
	b .L_0200b188
.L_0200b170:
	ldr r3, [r5]
	ldr r4, [sp, #12]
	mov r1, r10
	subs r6, r3, r4
	ldr r3, [r5, #8]
	subs r1, r2, r1
	mov r2, r11
	mov r4, r10
	subs r3, r3, r2
	subs r7, r3, r4
	mov r8, r1
	subs r4, r7, r1
.L_0200b188:
	ldr r3, [r5, #4]
	cmp r3, r12
	bne .L_0200b19a
	adds r3, r0, #0
	movs r0, #192
	lsls r0, r0, #4
	mov r9, r0
	cmp r3, #2
	bne .L_0200b1a0
.L_0200b19a:
	movs r1, #128
	lsls r1, r1, #4
	mov r9, r1
.L_0200b1a0:
	mov r0, r8
	adds r3, r0, r7
	asrs r3, r3, #16
	adds r3, #50
	asrs r2, r6, #16
	asrs r1, r4, #16
	str r3, [sp, #8]
	movs r3, #135
	adds r6, r2, #0
	adds r4, r1, #0
	adds r2, #7
	lsls r3, r3, #1
	subs r6, #8
	subs r4, #8
	cmp r2, r3
	bhi .L_0200b24c
	adds r3, r1, #0
	adds r3, #39
	cmp r3, #238
	bhi .L_0200b1fa
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r6, r3
	movs r3, #255
	ands r4, r3
	ldrh r3, [r5, #16]
	mov r0, r9
	lsls r1, r3, #3
	movs r3, #0
	str r3, [r5, #24]
	lsls r3, r6, #16
	orrs r4, r3
	ldr r3, .L_0200b274
	orrs r4, r3
	str r4, [r5, #28]
	ldr r4, [sp, #0]
	adds r3, r4, r1
	orrs r3, r0
	str r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #24
	ldr r1, [sp, #8]
	bl Func_02003c94
.L_0200b1fa:
	mov r1, r8
	subs r4, r7, r1
	asrs r3, r4, #16
	adds r4, r3, #0
	adds r3, #55
	adds r4, #8
	cmp r3, #238
	bhi .L_0200b24c
	ldr r3, [r5, #4]
	ldr r2, [r5, #12]
	subs r3, r3, r2
	asrs r1, r3, #16
	cmp r1, #0
	beq .L_0200b24c
	movs r3, #255
	adds r0, r5, #0
	subs r1, #1
	ands r4, r3
	adds r0, #36
	cmp r1, #7
	bls .L_0200b226
	movs r1, #7
.L_0200b226:
	lsls r3, r1, #2
	adds r1, r3, #0
	movs r3, #0
	str r3, [r0]
	lsls r3, r6, #16
	orrs r4, r3
	movs r3, #192
	lsls r3, r3, #7
	orrs r4, r3
	str r4, [r5, #40]
	ldr r2, [sp, #0]
	adds r1, #64
	adds r3, r2, r1
	mov r4, r9
	orrs r4, r3
	str r4, [r5, #44]
	ldr r1, [sp, #8]
	bl Func_02003c94
.L_0200b24c:
	ldr r0, [sp, #4]
	adds r5, #48
	subs r0, #1
	str r0, [sp, #4]
	cmp r0, #0
	blt .L_0200b25a
	b .L_0200b13c
.L_0200b25a:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200b268:
	.4byte Data_0200536c
.L_0200b26c:
	.4byte 0xffff0000
.L_0200b270:
	.4byte ResourceTableEntries
.L_0200b274:
	.4byte 0x40002000
	.section .text.x0200b278,"ax",%progbits
	.global Func_02003278
	.thumb_func
Func_02003278:
	push {r5, r6, lr}
	ldr r6, .L_0200b2c4
	movs r1, #192
	lsls r1, r1, #3
	ldr r3, .L_0200b2c8
	adds r1, #4
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	movs r0, #200
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	adds r1, r5, #0
	ldr r0, .L_0200b2cc
	bl Func_02003c7c
	bl Resource_FindFreeEntry
	strh r0, [r6]
	movs r1, #200
	lsls r1, r1, #4
	adds r2, r5, #0
	ldrh r0, [r6]
	bl VramBlock_LoadCached
	adds r0, r5, #0
	bl Sys_Free
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #28
	ldr r0, .L_0200b2d0
	bl Scheduler_AddOrUpdateCallback
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b2c4:
	.4byte Data_0200536c
.L_0200b2c8:
	.4byte IwramClearWords
.L_0200b2cc:
	.4byte Data_0200414c
.L_0200b2d0:
	.4byte Func_020030f4
	.section .text.x0200b2d4,"ax",%progbits
	.global Func_020032d4
	.thumb_func
Func_020032d4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #28
	str r1, [sp, #24]
	str r2, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	mov r10, r0
	mov r12, r3
	movs r3, #132
	lsls r3, r3, #1
	add r3, r12
	ldr r1, [r3, #48]
	mov r2, r12
	adds r2, #236
	ldr r0, [r2]
	mov r8, r1
	ldr r1, [r3, #8]
	adds r2, #4
	adds r1, r1, r0
	str r1, [sp, #16]
	ldr r1, [r2]
	ldr r3, [r3, #12]
	ldr r2, .L_0200b418
	adds r3, r3, r1
	str r3, [sp, #12]
	mov r3, r12
	adds r3, #244
	ldr r3, [r3]
	mov r9, r2
	subs r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #8]
	mov r3, r12
	adds r3, #248
	ldr r3, [r3]
	asrs r0, r0, #20
	subs r3, r3, r1
	asrs r3, r3, #20
	str r3, [sp, #4]
	asrs r1, r1, #20
	ldrh r2, [r2, #2]
	lsls r1, r1, #7
	lsls r3, r2, #1
	adds r1, r1, r0
	str r1, [sp, #0]
	adds r3, r3, r2
	ldr r1, [sp, #24]
	ldr r2, .L_0200b41c
	lsls r3, r3, #4
	add r3, r9
	adds r5, r3, #4
	lsls r3, r1, #8
	str r3, [r2]
	ldr r2, [sp, #0]
	ldr r1, [sp, #4]
	lsls r3, r2, #2
	add r8, r3
	movs r3, #0
	mov lr, r3
	cmp lr, r1
	bge .L_0200b408
.L_0200b35c:
	mov r2, lr
	lsls r2, r2, #16
	lsrs r3, r2, #7
	mov r11, r2
	ldr r2, [sp, #8]
	mov r1, r8
	movs r7, #0
	adds r6, r1, r3
	cmp r7, r2
	bge .L_0200b3f2
.L_0200b370:
	ldrb r4, [r6, #2]
	cmp r4, #0
	beq .L_0200b3de
	cmp r4, r10
	bcc .L_0200b3de
	mov r3, r10
	adds r3, #8
	cmp r4, r3
	bcs .L_0200b3de
	ldr r2, [sp, #16]
	lsls r1, r7, #16
	lsrs r1, r1, #16
	lsls r3, r1, #20
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [sp, #12]
	mov r3, r11
	lsrs r0, r3, #16
	lsls r3, r0, #20
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r2, [sp, #20]
	lsls r0, r0, #7
	lsls r3, r2, #19
	str r3, [r5, #12]
	ldr r2, [sp, #24]
	adds r1, r1, r0
	lsls r3, r2, #19
	mov r2, r10
	str r3, [r5, #4]
	subs r3, r4, r2
	strh r3, [r5, #16]
	movs r2, #0
	movs r3, #1
	str r2, [r5, #20]
	strh r3, [r5, #18]
	mov r2, r9
	ldrh r3, [r2, #2]
	adds r5, #48
	adds r3, #1
	strh r3, [r2, #2]
	movs r3, #158
	lsls r3, r3, #1
	add r3, r12
	ldr r2, [r3]
	ldr r3, [sp, #0]
	adds r2, r2, r3
	movs r3, #120
	strb r3, [r2, r1]
.L_0200b3de:
	movs r1, #128
	lsls r3, r7, #16
	lsls r1, r1, #9
	ldr r2, [sp, #8]
	adds r3, r3, r1
	asrs r7, r3, #16
	lsrs r3, r3, #16
	adds r6, #4
	cmp r3, r2
	blt .L_0200b370
.L_0200b3f2:
	mov r1, lr
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	ldr r2, [sp, #4]
	asrs r1, r3, #16
	lsrs r3, r3, #16
	mov lr, r1
	cmp r3, r2
	blt .L_0200b35c
.L_0200b408:
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b418:
	.4byte Data_0200536c
.L_0200b41c:
	.4byte Data_0202c001 + 0x1df
	.section .text.x0200b420,"ax",%progbits
	.global Func_02003420
	.thumb_func
Func_02003420:
	push {r5, r6, lr}
	adds r6, r1, #0
	adds r5, r2, #0
	bl Object_GetById
	ldr r4, .L_0200b45c
	ldrh r2, [r4, #2]
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #4
	adds r3, r3, r4
	adds r1, r3, #4
	cmp r0, #0
	beq .L_0200b45a
	movs r3, #0
	str r3, [r1]
	str r3, [r1, #8]
	lsls r3, r5, #19
	str r3, [r1, #12]
	str r3, [r1, #4]
	movs r3, #2
	strh r3, [r1, #18]
	adds r3, r0, #0
	adds r3, #8
	strh r6, [r1, #16]
	str r3, [r1, #20]
	ldrh r3, [r4, #2]
	adds r3, #1
	strh r3, [r4, #2]
.L_0200b45a:
	pop {r5, r6, pc}
.L_0200b45c:
	.4byte Data_0200536c
	.section .text.x0200b460,"ax",%progbits
	.global Func_02003460
	.thumb_func
Func_02003460:
	push {lr}
	ldr r1, .L_0200b490
	adds r3, r0, #0
	cmp r0, #0
	bge .L_0200b46e
	ldr r2, .L_0200b494
	adds r3, r0, r2
.L_0200b46e:
	movs r2, #255
	asrs r3, r3, #19
	ands r3, r2
	ldr r2, .L_0200b498
	lsls r3, r3, #8
	str r3, [r2]
	adds r2, r1, #4
	movs r1, #31
.L_0200b47e:
	ldr r3, [r2]
	cmp r3, #0
	beq .L_0200b486
	str r0, [r2, #4]
.L_0200b486:
	subs r1, #1
	adds r2, #48
	cmp r1, #0
	bge .L_0200b47e
	pop {pc}
.L_0200b490:
	.4byte Data_0200536c
.L_0200b494:
	.4byte 0x0007ffff
.L_0200b498:
	.4byte Data_0202c001 + 0x1df
	.section .text.x0200b49c,"ax",%progbits
	.global Func_0200349c
	.thumb_func
Func_0200349c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r4, r1, #0
	ldr r2, .L_0200b4f0
	ldr r1, [r3, #32]
	cmp r0, #31
	bhi .L_0200b4ec
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	adds r3, r3, r2
	adds r0, r3, #4
	ldr r3, [r0]
	cmp r3, #0
	beq .L_0200b4ec
	adds r3, r4, #0
	cmp r4, #0
	bge .L_0200b4c6
	ldr r2, .L_0200b4f4
	adds r3, r4, r2
.L_0200b4c6:
	movs r2, #255
	asrs r3, r3, #19
	ands r3, r2
	ldr r2, .L_0200b4f8
	lsls r3, r3, #8
	str r3, [r2]
	movs r2, #158
	lsls r2, r2, #1
	adds r3, r1, r2
	ldr r1, [r3]
	ldr r3, [r0, #8]
	ldr r2, [r0]
	asrs r3, r3, #20
	lsls r3, r3, #7
	asrs r2, r2, #20
	adds r2, r2, r3
	movs r3, #121
	strb r3, [r1, r2]
	str r4, [r0, #4]
.L_0200b4ec:
	pop {pc}
	.2byte 0x0000
.L_0200b4f0:
	.4byte Data_0200536c
.L_0200b4f4:
	.4byte 0x0007ffff
.L_0200b4f8:
	.4byte Data_0202c001 + 0x1e3
	.section .text.x0200b4fc,"ax",%progbits
	.global Func_020034fc
	.thumb_func
Func_020034fc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	sub sp, #16
	adds r2, r1, #0
	adds r2, #228
	ldr r3, [r2]
	ldr r0, .L_0200b618
	str r3, [sp, #12]
	ldr r4, [sp, #12]
	ldr r3, .L_0200b61c
	ands r4, r3
	str r4, [sp, #12]
	ldr r2, [r2, #4]
	ands r2, r3
	ldr r3, [r1]
	mov r11, r2
	ldr r3, [r3, #4]
	ldr r2, .L_0200b620
	str r3, [sp, #8]
	ldrh r3, [r0]
	adds r0, #36
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	mov r9, r0
	lsrs r3, r3, #5
	str r3, [sp, #0]
	movs r3, #31
	str r3, [sp, #4]
.L_0200b546:
	mov r4, r9
	ldrh r3, [r4, #12]
	cmp r3, #0
	beq .L_0200b5fa
	ldr r3, [r4]
	ldr r2, [sp, #12]
	ldr r5, [r4, #4]
	ldr r4, [r4, #8]
	subs r2, r3, r2
	ldr r3, [sp, #8]
	mov r8, r4
	ldr r4, [sp, #8]
	mov r10, r2
	subs r5, r5, r3
	mov r2, r8
	mov r3, r11
	subs r2, r2, r3
	subs r2, r2, r4
	subs r7, r2, r5
	mov r8, r2
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	mov r2, r10
	lsls r3, r3, #1
	asrs r6, r2, #16
	lsrs r3, r3, #16
	adds r6, r6, r3
	movs r3, #10
	negs r3, r3
	adds r3, r3, r6
	mov r10, r3
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	asrs r2, r7, #16
	add r5, r8
	lsrs r3, r3, #16
	adds r2, r2, r3
	asrs r5, r5, #16
	movs r4, #135
	adds r7, r2, #0
	adds r5, #50
	adds r6, #5
	lsls r4, r4, #1
	subs r7, #10
	mov r8, r5
	cmp r6, r4
	bhi .L_0200b5fa
	adds r3, r2, #0
	adds r3, #37
	cmp r3, #238
	bhi .L_0200b5fa
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	mov r2, r10
	ands r2, r3
	movs r3, #255
	mov r4, r9
	ands r7, r3
	movs r3, #0
	str r3, [r4, #16]
	lsls r3, r2, #16
	orrs r7, r3
	ldr r3, .L_0200b624
	mov r5, r9
	orrs r7, r3
	str r7, [r4, #20]
	bl Random16Far
	lsls r3, r0, #1
	adds r3, r3, r0
	ldr r2, [sp, #0]
	lsrs r3, r3, #16
	lsls r3, r3, #3
	movs r4, #128
	adds r3, r2, r3
	lsls r4, r4, #4
	adds r5, #24
	orrs r3, r4
	mov r0, r9
	str r3, [r5]
	adds r0, #16
	mov r1, r8
	bl Func_02003c94
.L_0200b5fa:
	ldr r2, [sp, #4]
	movs r3, #28
	subs r2, #1
	str r2, [sp, #4]
	add r9, r3
	cmp r2, #0
	bge .L_0200b546
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b618:
	.4byte Data_02005970
.L_0200b61c:
	.4byte 0xffff0000
.L_0200b620:
	.4byte ResourceTableEntries
.L_0200b624:
	.4byte 0x40002000
	.section .text.x0200b628,"ax",%progbits
	.global Func_02003628
	.thumb_func
Func_02003628:
	push {r5, r6, lr}
	ldr r6, .L_0200b678
	movs r1, #233
	ldr r3, .L_0200b67c
	lsls r1, r1, #2
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	movs r0, #128
	lsls r0, r0, #3
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	adds r1, r5, #0
	ldr r0, .L_0200b680
	bl Func_02003c7c
	bl Resource_FindFreeEntry
	strh r0, [r6]
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #3
	ldrh r0, [r6]
	bl VramBlock_LoadCached
	adds r0, r5, #0
	bl Sys_Free
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #28
	ldr r0, .L_0200b684
	bl Scheduler_AddOrUpdateCallback
	movs r0, #220
	bl Func_02003ef4
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b678:
	.4byte Data_02005970
.L_0200b67c:
	.4byte IwramClearWords
.L_0200b680:
	.4byte Data_02004350 + 0x1
.L_0200b684:
	.4byte Func_020034fc
	.section .text.x0200b688,"ax",%progbits
	.global Func_02003688
	.thumb_func
Func_02003688:
	push {r5, r6, lr}
	adds r5, r2, #0
	ldr r2, .L_0200b6b4
	adds r6, r3, #0
	adds r4, r2, #4
	ldr r3, [r4]
	cmp r3, #0
	beq .L_0200b69a
	adds r4, #16
.L_0200b69a:
	movs r2, #128
	lsls r2, r2, #12
	lsls r3, r0, #20
	adds r3, r3, r2
	str r3, [r4]
	lsls r3, r1, #20
	str r3, [r4, #4]
	lsls r3, r5, #20
	adds r3, r3, r2
	str r3, [r4, #8]
	strh r6, [r4, #14]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b6b4:
	.4byte Data_02005970
	.section .text.x0200b6b8,"ax",%progbits
	.global Func_020036b8
	.thumb_func
Func_020036b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r1, [sp, #12]
	movs r1, #0
	str r1, [sp, #8]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	ldr r4, .L_0200b83c
	str r3, [sp, #4]
	movs r2, #1
	ldrh r6, [r4, #2]
	mov r11, r0
	mov r10, r1
	adds r7, r4, #4
	mov r8, r2
.L_0200b6e4:
	ldr r3, [r7]
	cmp r3, #0
	beq .L_0200b7ac
	ldrh r2, [r4, #2]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r4, r3
	adds r5, r3, #0
	adds r5, #36
.L_0200b6f8:
	ldr r1, [sp, #4]
	movs r2, #156
	lsls r2, r2, #1
	adds r3, r1, r2
	ldr r1, [r3]
	ldr r3, [r7, #8]
	ldr r0, [r7]
	asrs r3, r3, #20
	asrs r2, r0, #20
	lsls r3, r3, #7
	adds r2, r2, r3
	lsls r2, r2, #2
	adds r1, r1, r2
	ldrb r1, [r1, #2]
	cmp r1, #255
	bne .L_0200b738
	str r0, [r5]
	ldr r3, [r7, #8]
	movs r1, #1
	str r3, [r5, #8]
	ldr r3, [r7, #4]
	mov r10, r1
	str r3, [r5, #4]
	movs r3, #1
	strh r3, [r5, #12]
	adds r5, #28
	ldrh r3, [r4, #2]
	adds r3, #1
	strh r3, [r4, #2]
	movs r3, #0
	mov r9, r3
	b .L_0200b782
.L_0200b738:
	cmp r1, r11
	blt .L_0200b75e
	mov r3, r11
	adds r3, #7
	cmp r1, r3
	bgt .L_0200b75e
	ldrh r3, [r7, #14]
	ldr r2, .L_0200b840
	lsls r3, r3, #3
	adds r3, r3, r1
	mov r1, r11
	subs r3, r3, r1
	ldrsb r3, [r2, r3]
	mov r9, r3
	cmp r3, #8
	beq .L_0200b75a
	ldrh r6, [r4, #2]
.L_0200b75a:
	movs r2, #0
	b .L_0200b780
.L_0200b75e:
	ldr r3, [sp, #12]
	cmp r1, r3
	bne .L_0200b776
	ldr r2, [sp, #8]
	movs r1, #8
	adds r2, #1
	movs r3, #0
	ldrh r6, [r4, #2]
	mov r9, r1
	str r2, [sp, #8]
	mov r10, r3
	b .L_0200b782
.L_0200b776:
	cmp r1, #0
	bne .L_0200b782
	movs r1, #8
	movs r2, #0
	mov r9, r1
.L_0200b780:
	mov r10, r2
.L_0200b782:
	mov r3, r9
	cmp r3, #8
	beq .L_0200b7a4
	ldrh r3, [r7, #14]
	movs r2, #3
	add r3, r9
	ands r3, r2
	strh r3, [r7, #14]
	movs r0, #128
	ldrh r1, [r7, #14]
	lsls r0, r0, #13
	lsls r1, r1, #14
	adds r2, r7, #0
	str r4, [sp, #0]
	bl Vector_AddPolarOffsetFar
	ldr r4, [sp, #0]
.L_0200b7a4:
	mov r1, r10
	cmp r1, #0
	bne .L_0200b6f8
	strh r6, [r4, #2]
.L_0200b7ac:
	movs r2, #1
	negs r2, r2
	add r8, r2
	mov r3, r8
	adds r7, #16
	cmp r3, #0
	bge .L_0200b6e4
	lsls r3, r6, #3
	subs r3, r3, r6
	lsls r3, r3, #2
	adds r3, r4, r3
	adds r5, r3, #0
	adds r5, #36
	cmp r6, #31
	bgt .L_0200b7e8
	movs r3, #32
	subs r3, r3, r6
	mov r8, r3
.L_0200b7d0:
	ldrh r3, [r5, #12]
	cmp r3, #0
	beq .L_0200b7da
	movs r3, #0
	strh r3, [r5, #12]
.L_0200b7da:
	movs r1, #1
	negs r1, r1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #0
	bne .L_0200b7d0
.L_0200b7e8:
	adds r5, r4, #0
	adds r5, #36
	cmp r6, #0
	beq .L_0200b82c
	mov r8, r6
.L_0200b7f2:
	ldrh r3, [r5, #12]
	cmp r3, #0
	beq .L_0200b81e
	ldr r1, [sp, #4]
	movs r2, #156
	lsls r2, r2, #1
	adds r3, r1, r2
	ldr r1, [r3]
	ldr r3, [r5, #8]
	ldr r2, [r5]
	asrs r3, r3, #20
	lsls r3, r3, #7
	asrs r2, r2, #20
	adds r2, r2, r3
	lsls r2, r2, #2
	adds r1, r1, r2
	ldrb r2, [r1, #3]
	movs r3, #0
	strb r3, [r1, #2]
	movs r3, #128
	orrs r3, r2
	strb r3, [r1, #3]
.L_0200b81e:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r1, r8
	adds r5, #28
	cmp r1, #0
	bne .L_0200b7f2
.L_0200b82c:
	ldr r0, [sp, #8]
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200b83c:
	.4byte Data_02005970
.L_0200b840:
	.4byte Data_0200534c
	.section .text.x0200b844,"ax",%progbits
	.global Func_02003844
	.thumb_func
Func_02003844:
	push {lr}
	ldr r4, [r0, #8]
	ldr r3, [r1, #8]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_0200b85a
	movs r3, #192
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_0200b864
	b .L_0200b894
.L_0200b85a:
	movs r2, #192
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_0200b894
.L_0200b864:
	ldr r2, [r1, #12]
	ldr r3, [r0, #12]
	subs r3, r3, r2
	ldr r2, .L_0200b898
	subs r3, #1
	cmp r3, r2
	bhi .L_0200b894
	ldr r0, [r0, #16]
	ldr r1, [r1, #16]
	subs r2, r0, r1
	cmp r2, #0
	blt .L_0200b886
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_0200b890
	b .L_0200b894
.L_0200b886:
	movs r2, #128
	subs r3, r1, r0
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_0200b894
.L_0200b890:
	movs r0, #1
	b .L_0200b896
.L_0200b894:
	movs r0, #0
.L_0200b896:
	pop {pc}
.L_0200b898:
	.4byte 0x000ffffe
	.section .text.x0200b89c,"ax",%progbits
	.global Func_0200389c
	.thumb_func
Func_0200389c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r2, .L_0200b98c
	ldr r3, .L_0200b990
	mov r8, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	bl Object_GetById
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
	adds r4, r0, #0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #16]
	str r4, [sp, #0]
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_0200b8ea
	adds r3, #15
.L_0200b8ea:
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
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_0200b936
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02003844
	cmp r0, #0
	beq .L_0200b936
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	movs r3, #0
	str r3, [r6, #76]
.L_0200b936:
	movs r3, #164
	lsls r3, r3, #1
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_0200b982
	adds r5, r2, r3
.L_0200b946:
	bl Object_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_0200b974
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02003844
	cmp r0, #0
	beq .L_0200b974
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	movs r3, #0
	str r3, [r6, #76]
.L_0200b974:
	adds r7, #1
	cmp r7, #3
	bgt .L_0200b982
	adds r5, #2
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_0200b946
.L_0200b982:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b98c:
	.4byte Data_020023c4 + 0x188
.L_0200b990:
	.4byte gPartyState
	.section .text.x0200b994,"ax",%progbits
	.global Func_02003994
	.thumb_func
Func_02003994:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r2, r5, #0
	adds r3, #4
	strb r6, [r3]
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r3, #3
	ldr r0, [r5, #80]
	ands r1, r3
	ldrb r2, [r0, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	lsls r1, r1, #2
	orrs r3, r1
	strb r3, [r0, #9]
	movs r1, #0
	adds r0, r5, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Func_02003cc4
	adds r0, r5, #0
	ldr r1, .L_0200b9ec
	bl Func_02003ccc
	adds r0, r5, #0
	movs r1, #10
	bl Object_SetPartAttribute
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
.L_0200b9ec:
	.4byte Data_020044c8
	.section .text.x0200b9f0,"ax",%progbits
	.global Func_020039f0
	.thumb_func
Func_020039f0:
	push {r5, r6, lr}
	ldr r3, [r0, #8]
	ldr r2, [r0, #4]
	adds r6, r1, #0
	ldr r1, [r0]
	ldr r0, .L_0200ba64
	adds r3, r3, r0
	movs r0, #30
	adds r0, #255
	bl Func_02003cd4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200ba62
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #12
	movs r3, #192
	lsls r3, r3, #6
	lsrs r0, r0, #16
	adds r0, r0, r3
	bl Math_Cosine
	str r0, [r5, #68]
	bl Random16Far
	lsls r0, r0, #16
	lsrs r0, r0, #16
	str r0, [r5, #72]
	bl Random16Far
	lsls r0, r0, #17
	lsrs r0, r0, #16
	adds r0, r0, r6
	str r0, [r5, #76]
	bl Random16Far
	ldr r3, .L_0200ba68
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	movs r1, #3
	adds r0, r5, #0
	bl Func_02003994
	ldr r3, .L_0200ba6c
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #1
	bl Animation_SetStateFlags
.L_0200ba62:
	pop {r5, r6, pc}
.L_0200ba64:
	.4byte 0xfffe0000
.L_0200ba68:
	.4byte 0xffff8000
.L_0200ba6c:
	.4byte Func_0200389c
	.section .text.x0200ba70,"ax",%progbits
	.global Func_02003a70
	.thumb_func
Func_02003a70:
	push {r5, r6, lr}
	ldr r3, .L_0200baac
	movs r6, #15
	ldr r0, [r3, #4]
	adds r5, r3, #0
	mov lr, r0
	.2byte 0xf800
	adds r5, #8
.L_0200ba80:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_0200baa2
	movs r1, #2
	ldrsh r3, [r5, r1]
	ldrh r2, [r5, #2]
	cmp r3, #0
	bgt .L_0200ba9e
	adds r0, r5, #4
	ldr r1, [r5, #16]
	bl Func_020039f0
	movs r3, #9
	b .L_0200baa0
.L_0200ba9e:
	subs r3, r2, #1
.L_0200baa0:
	strh r3, [r5, #2]
.L_0200baa2:
	subs r6, #1
	adds r5, #20
	cmp r6, #0
	bge .L_0200ba80
	pop {r5, r6, pc}
.L_0200baac:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200bab0,"ax",%progbits
	.global Func_02003ab0
	.thumb_func
Func_02003ab0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	ldr r0, .L_0200bb98
	mov r9, r2
	mov r10, r0
	movs r2, #8
	movs r0, #10
	add r2, r10
	adds r0, #255
	sub sp, #4
	adds r7, r1, #0
	mov r8, r2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bb58
	movs r1, #168
	lsls r1, r1, #1
	ldr r3, .L_0200bb9c
	mov r0, r10
	mov lr, r3
	.2byte 0xf800
	ldrh r1, [r6]
	movs r4, #0
	adds r6, #2
	cmp r1, #0
	ble .L_0200bb30
.L_0200baee:
	ldrh r2, [r6]
	mov r0, r8
	movs r3, #0
	ldrh r5, [r6, #2]
	strh r3, [r0]
	movs r3, #128
	lsls r3, r3, #13
	lsls r1, r1, #16
	lsls r2, r2, #16
	str r2, [r0, #12]
	str r1, [r0, #4]
	adds r2, r2, r3
	movs r0, #0
	str r4, [sp, #0]
	bl Map_GetTerrainHeight
	ldr r4, [sp, #0]
	mov r2, r8
	mov r3, r8
	lsls r5, r5, #16
	str r0, [r2, #8]
	str r5, [r2, #16]
	movs r0, #20
	strh r4, [r3, #2]
	adds r4, #1
	adds r6, #4
	add r8, r0
	cmp r4, #15
	bgt .L_0200bb30
	ldrh r1, [r6]
	adds r6, #2
	cmp r1, #0
	bgt .L_0200baee
.L_0200bb30:
	cmp r7, #0
	beq .L_0200bb58
	ldrh r1, [r7]
	movs r4, #0
	adds r7, #2
	cmp r1, #0
	ble .L_0200bb58
.L_0200bb3e:
	movs r2, #164
	lsls r3, r4, #1
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r0, r10
	adds r4, #1
	strh r1, [r0, r3]
	cmp r4, #3
	bgt .L_0200bb58
	ldrh r1, [r7]
	adds r7, #2
	cmp r1, #0
	bgt .L_0200bb3e
.L_0200bb58:
	movs r1, #128
	lsls r1, r1, #19
	mov r3, r10
	mov r2, r9
	adds r1, #80
	str r2, [r3, #4]
	ldrh r3, [r1]
	cmp r3, #0
	bne .L_0200bb80
	movs r3, #192
	movs r2, #128
	lsls r3, r3, #4
	lsls r2, r2, #19
	adds r3, #8
	adds r2, #82
	strh r3, [r2]
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #16
	strh r3, [r1]
.L_0200bb80:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200bba0
	bl Scheduler_AddOrUpdateCallback
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200bb98:
	.4byte Data_020023c4 + 0x188
.L_0200bb9c:
	.4byte IwramClearWords
.L_0200bba0:
	.4byte Func_02003a70
	.section .text.x0200bba4,"ax",%progbits
	.global Func_02003ba4
	.thumb_func
Func_02003ba4:
	ldr r2, .L_0200bbb4
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r3, r3, r2
	movs r2, #8
	ldrsh r0, [r3, r2]
	bx lr
.L_0200bbb4:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200bbb8,"ax",%progbits
	.global Func_02003bb8
	.thumb_func
Func_02003bb8:
	ldr r2, .L_0200bbc8
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r3, r3, r2
	strh r1, [r3, #8]
	bx lr
	.2byte 0x0000
.L_0200bbc8:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200bbcc,"ax",%progbits
	.global Func_02003bcc
	.thumb_func
Func_02003bcc:
	ldr r3, .L_0200bbd4
	ldr r0, [r3]
	bx lr
	.2byte 0x0000
.L_0200bbd4:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200bbd8,"ax",%progbits
	.global Func_02003bd8
	.thumb_func
Func_02003bd8:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #130
	lsls r0, r0, #1
	ldr r5, .L_0200bbfc
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bbf6
	ldr r3, [r5]
	adds r3, #1
	str r3, [r5]
	cmp r3, r6
	blt .L_0200bbf6
	str r0, [r5]
.L_0200bbf6:
	ldr r0, [r5]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200bbfc:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200bc00,"ax",%progbits
	.global Func_02003c00
	.thumb_func
Func_02003c00:
	ldr r2, .L_0200bc10
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r3, r3, r2
	str r1, [r3, #24]
	bx lr
	.2byte 0x0000
.L_0200bc10:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200bc14,"ax",%progbits
	.global Func_02003c14
	.thumb_func
Func_02003c14:
	push {lr}
	ldr r2, .L_0200bc28
	cmp r0, #3
	bhi .L_0200bc26
	lsls r3, r0, #1
	movs r0, #164
	lsls r0, r0, #1
	adds r3, r3, r0
	strh r1, [r2, r3]
.L_0200bc26:
	pop {pc}
.L_0200bc28:
	.4byte Data_020023c4 + 0x188
	.section .rodata.x0200bf04,"a",%progbits
.L_0200bf04:
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
.L_0200bf40:
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
.L_0200bf7c:
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
	.global Data_02003fb8
Data_02003fb8:
	.4byte 0x424b4243
	.4byte 0x425b4253
	.4byte 0x426b4263
	.4byte 0x51434273
	.4byte 0x5153514b
	.4byte 0x5163515b
	.4byte 0x5173516b
	.4byte 0x604b6043
	.global Data_02003fd8
Data_02003fd8:
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02003fe8
Data_02003fe8:
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02004004
Data_02004004:
	.4byte 0x0000002e
	.4byte Func_020013bc
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02004014
Data_02004014:
	.4byte 0x0000002e
	.4byte Func_0200147c
	.4byte 0x0000002e
	.4byte Func_02001520
	.4byte 0x0000002e
	.4byte Func_0200156c
	.4byte 0x00000026
	.4byte 0x00000011
	.global Data_02004034
Data_02004034:
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000004
	.4byte 0x00000011
	.global Data_02004058
Data_02004058:
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000004
	.4byte 0x00000011
	.global Data_0200407c
Data_0200407c:
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
	.global Data_020040ac
Data_020040ac:
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
	.global Data_020040dc
Data_020040dc:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000026
	.global Data_020040f4
Data_020040f4:
	.4byte 0x01580118
	.4byte 0x01380002
	.4byte 0x00020158
	.2byte 0x0000
	.global Data_02004102
Data_02004102:
	.2byte 0x0008
	.2byte 0x0000
	.global Data_02004106
Data_02004106:
	.2byte 0x0098
	.4byte 0x00020218
	.4byte 0x021800d8
	.4byte 0x00000002
	.global Data_02004114
Data_02004114:
	.4byte 0x000b000a
	.4byte 0x00000000
	.global Data_0200411c
Data_0200411c:
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000026
	.global Data_02004128
Data_02004128:
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000026
	.global Data_0200414c
Data_0200414c:
	.4byte 0x11071100
	.4byte 0xa8364b24
	.4byte 0x2d16a0e3
	.4byte 0xa0a20502
	.4byte 0x11e8f16b
	.4byte 0xd788a3cd
	.4byte 0x28f35902
	.4byte 0xc9358f10
	.4byte 0x5926b9f4
	.4byte 0x991188c7
	.4byte 0xd73ef8d4
	.4byte 0xc8c91e08
	.4byte 0x3c16be66
	.4byte 0x28384c90
	.4byte 0xe08b4750
	.4byte 0xc788a089
	.4byte 0x3a788868
	.4byte 0x2225c912
	.4byte 0x9927de2e
	.4byte 0xe3c78dd7
	.4byte 0xb5c78c08
	.4byte 0x1622129a
	.4byte 0x772456f3
	.4byte 0x2d1123f2
	.4byte 0x30896076
	.4byte 0x4f3a2452
	.4byte 0x508f1a20
	.4byte 0x40d70300
	.4byte 0xc0d62600
	.4byte 0x1e1ac783
	.4byte 0xf4406773
	.4byte 0x48bf9028
	.4byte 0x88c10064
	.4byte 0x1e168f1f
	.4byte 0x7b5e2ddf
	.4byte 0x9c63db7c
	.4byte 0xedbe3c78
	.4byte 0x31ec6e31
	.4byte 0xae3c787e
	.4byte 0x5baf3ff9
	.4byte 0xf1e37c78
	.4byte 0xf1e3c6f8
	.4byte 0x1be3c78d
	.4byte 0x1e37c78f
	.4byte 0x1e3c6f8f
	.4byte 0x8f1efcd7
	.4byte 0xf1e1386f
	.4byte 0x5d78be4d
	.4byte 0xf1e6f8f1
	.4byte 0x8febc886
	.4byte 0x7a6f8f3e
	.4byte 0xf17c813c
	.4byte 0x9c4f26f8
	.4byte 0xdf1e3c6f
	.4byte 0xebd8ce3c
	.4byte 0xd79f4df1
	.4byte 0x7e6f8f1f
	.4byte 0x7c78f840
	.4byte 0xa010f3f7
	.4byte 0x3e0421f0
	.4byte 0x38f9df1e
	.4byte 0x34ebe07c
	.4byte 0x7cef969f
	.4byte 0xf6b3e69d
	.4byte 0x8f1f429d
	.4byte 0xf192bcef
	.4byte 0xc7c6f831
	.4byte 0xaf1f5be6
	.4byte 0x3a187c6f
	.4byte 0xc3e34061
	.4byte 0x9cc12df2
	.4byte 0xf9a9f1e7
	.4byte 0xf7cf0f8e
	.4byte 0x89be4e7c
	.4byte 0x8f39f85a
	.4byte 0x7cb7cd4f
	.4byte 0xc067be6a
	.4byte 0x4a7c79e7
	.4byte 0xf263e7be
	.4byte 0xef9b1f0d
	.4byte 0x9faef9f9
	.4byte 0x8f3af8eb
	.4byte 0xfe7c0d57
	.4byte 0xf043e9be
	.4byte 0x47caf40d
	.4byte 0x04e27be0
	.4byte 0x0e431d73
	.4byte 0xe1a7c7df
	.4byte 0x7cb58e1b
	.4byte 0xf1e7be08
	.4byte 0x37ce452d
	.4byte 0x1df38fbc
	.4byte 0x706f839f
	.4byte 0x4b7c78f8
	.4byte 0xf033e04c
	.4byte 0x84f8d70d
	.4byte 0xc0e7ab7c
	.4byte 0x7b3d04a5
	.4byte 0xd3e00d2e
	.4byte 0xe80e9c0a
	.4byte 0x8f9eaf59
	.4byte 0x0b2007cb
	.4byte 0x8540be3c
	.4byte 0xbe5c7c3b
	.4byte 0x9cf8dbc0
	.4byte 0xc18f819c
	.4byte 0x1f3e7817
	.4byte 0x31f03390
	.4byte 0x73ef02f8
	.4byte 0x0be6c7ce
	.4byte 0xc7c0a3dc
	.4byte 0x3c7c0be5
	.4byte 0xe2c7c0ce
	.4byte 0xce2c7c0b
	.4byte 0x0be2c7c0
	.4byte 0xc3cfadbc
	.4byte 0x7c78f819
	.4byte 0x19c38f81
	.4byte 0x817c78f8
	.4byte 0x3841f1a7
	.4byte 0x2f931f03
	.4byte 0x020038f0
	.4byte 0x702f971f
	.4byte 0x33af8d0b
	.4byte 0x02f971f0
	.4byte 0x279c04c7
	.4byte 0x02f961f0
	.4byte 0xc03042cf
	.4byte 0x00f81ea7
	.global Data_02004350
Data_02004350:
	.4byte 0x2c010000
	.4byte 0xab795ca8
	.4byte 0xd13ad9e7
	.4byte 0x5c4cf087
	.4byte 0xa682d956
	.4byte 0xc359b2cb
	.4byte 0x29e78d9a
	.4byte 0xb4c85b0b
	.4byte 0x3e7a342c
	.4byte 0x9475a56c
	.4byte 0x59ee4489
	.4byte 0xf44cf7e6
	.4byte 0x3626d93c
	.4byte 0x221e1a35
	.4byte 0x0da41051
	.4byte 0xeb7a8d99
	.4byte 0xd21fcad3
	.4byte 0xa4466b66
	.4byte 0x65e4a0d3
	.4byte 0xe90bf28a
	.4byte 0xfaf466d1
	.4byte 0xbf3cf336
	.4byte 0xa79e7997
	.4byte 0x1e7e775e
	.4byte 0x31179c79
	.4byte 0x493121a1
	.4byte 0x22c0bc91
	.4byte 0xfb2d7d79
	.4byte 0x2907f5ed
	.4byte 0x1e79d0f7
	.4byte 0xc09e973f
	.4byte 0xf2afbf32
	.4byte 0x423b3f65
	.4byte 0xf04d61e4
	.4byte 0xda013474
	.4byte 0x1fd19108
	.4byte 0xcaa1cc78
	.4byte 0xdf06d234
	.4byte 0xf84c067c
	.4byte 0x0310c40c
	.4byte 0xd0e98406
	.4byte 0x43609e54
	.4byte 0x1e479336
	.4byte 0x2df07939
	.4byte 0xf9c883a4
	.4byte 0x07ed3139
	.4byte 0x02486aa7
	.4byte 0x398d1aa7
	.4byte 0x011c1926
	.4byte 0x40e3e338
	.4byte 0x04e027c4
	.4byte 0x1882625c
	.4byte 0xf87031c1
	.4byte 0xa581f868
	.4byte 0x4100e0f1
	.4byte 0xbe868f64
	.4byte 0x0260f982
	.4byte 0xbf34282f
	.4byte 0x573956bd
	.4byte 0x2ee62c89
	.4byte 0x1c996591
	.4byte 0xe39c8d62
	.4byte 0xc2d98115
	.4byte 0xf8fbc9f9
	.4byte 0x719a4f39
	.4byte 0xbf116411
	.4byte 0x8bb221cc
	.4byte 0xf12e4371
	.4byte 0x01b1c3f8
	.4byte 0xa6a67894
	.4byte 0xf9bf043e
	.4byte 0x5f17c1b3
	.4byte 0x7889063e
	.4byte 0xe91ce0b9
	.4byte 0xc3857023
	.4byte 0xc7c8e18e
	.4byte 0x0e1f0402
	.4byte 0x28067c77
	.4byte 0x1b1cec14
	.4byte 0xd1f1debc
	.4byte 0xce39c948
	.4byte 0x65eb6111
	.4byte 0x1ee4778f
	.4byte 0x3c78735f
	.4byte 0xb8f4246d
	.4byte 0xf9f80201
	.4byte 0x11c2e7ca
	.4byte 0x21fb603f
	.4byte 0x3ef04650
	.4byte 0xd614ce34
	.4byte 0xca56a6c8
	.4byte 0xed5da3a8
	.4byte 0x03efdcf9
	.4byte 0x00000000
	.global Data_020044c8
Data_020044c8:
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
	.global Data_020044f8
Data_020044f8:
	.4byte .L_0200bf04
	.4byte .L_0200bf40
	.4byte .L_0200bf7c
	.global gSceneEntrances
gSceneEntrances:
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
	.global gSceneExits
gSceneExits:
	.4byte 0x000000a0
	.4byte 0x001010a7
	.4byte 0x002030a0
	.4byte 0x003020a0
	.4byte 0x004050a0
	.4byte 0x005040a0
	.4byte 0x006070a0
	.4byte 0x007060a0
	.4byte 0x008090a0
	.4byte 0x009080a0
	.4byte 0x00a020a3
	.4byte 0x00b010a2
	.4byte 0x000000a1
	.4byte 0x001010a6
	.4byte 0x002030a1
	.4byte 0x003020a1
	.4byte 0x004050a7
	.4byte 0x005020a2
	.4byte 0x006080a2
	.4byte 0x007080a1
	.4byte 0x008070a1
	.4byte 0x009060a2
	.4byte 0x000000a2
	.4byte 0x0010b0a0
	.4byte 0x002050a1
	.4byte 0x003010a9
	.4byte 0x004040a3
	.4byte 0x005050a3
	.4byte 0x006090a1
	.4byte 0x007040a6
	.4byte 0x008060a1
	.4byte 0x000000a3
	.4byte 0x0010409e
	.4byte 0x0020a0a0
	.4byte 0x003020a8
	.4byte 0x004040a2
	.4byte 0x005050a2
	.4byte 0x006060a4
	.4byte 0x007080a3
	.4byte 0x008070a3
	.4byte 0x009040a7
	.4byte 0x000000a4
	.4byte 0x001010a5
	.4byte 0x002020a5
	.4byte 0x003030a5
	.4byte 0x004050a4
	.4byte 0x005040a4
	.4byte 0x006060a3
	.4byte 0x0070c0a5
	.4byte 0x000000a5
	.4byte 0x001010a4
	.4byte 0x002020a4
	.4byte 0x003030a4
	.4byte 0x004050a5
	.4byte 0x005040a5
	.4byte 0x006070a5
	.4byte 0x007060a5
	.4byte 0x008090a5
	.4byte 0x009080a5
	.4byte 0x00a0b0a5
	.4byte 0x00b0a0a5
	.4byte 0x00c070a4
	.4byte 0x00d0109f
	.4byte 0x000000a6
	.4byte 0x001010a1
	.4byte 0x002030a7
	.4byte 0x003020a7
	.4byte 0x004070a2
	.4byte 0x000000a7
	.4byte 0x001010a0
	.4byte 0x002030a6
	.4byte 0x003020a6
	.4byte 0x004090a3
	.4byte 0x005040a1
	.4byte 0x006040a8
	.4byte 0x007030a8
	.4byte 0x008010a8
	.4byte 0x000000a8
	.4byte 0x001080a7
	.4byte 0x002030a3
	.4byte 0x003070a7
	.4byte 0x004060a7
	.4byte 0x000000a9
	.4byte 0x001030a2
	.4byte 0x000001ff
	.global Data_02004688
Data_02004688:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020046a0
Data_020046a0:
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02a70000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02270000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004748
Data_02004748:
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x00f70000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0x005000f4
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020047a8
Data_020047a8:
	.4byte 0xffff017f
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004808
Data_02004808:
	.4byte 0xffff017f
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02870000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00570000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00670000
	.4byte 0x00024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x01024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020048c8
Data_020048c8:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0xffff017f
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004910
Data_02004910:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x02770000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020049e8
Data_020049e8:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004a00
Data_02004a00:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004a48
Data_02004a48:
	.4byte 0xffff017f
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00e70000
	.4byte 0x00024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004b98
Data_02004b98:
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x01024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff0143
	.4byte 0x00000001
	.4byte 0x00000064
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004c70
Data_02004c70:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004c7c
Data_02004c7c:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000051
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000051
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000051
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x0000c602
	.4byte 0xffff0032
	.4byte Func_020004b4
	.4byte 0x00008602
	.4byte 0xffff0032
	.4byte Func_020004b4
	.4byte 0x00004602
	.4byte 0xffff0033
	.4byte Func_020004b4
	.4byte 0x00008c15
	.4byte 0x095a000a
	.4byte Func_0200042c
	.4byte 0x00008c15
	.4byte 0x095b000b
	.4byte Func_0200042c
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_0200042c
	.4byte 0x00002115
	.4byte 0x02300008
	.4byte Func_020004bc
	.4byte 0x00002115
	.4byte 0x02310009
	.4byte Func_02000574
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte Data_020005f4 + 0x1
	.4byte 0x00001815
	.4byte 0x0250000d
	.4byte Func_020005f8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004d84
Data_02004d84:
	.4byte 0x00000100
	.global Data_02004d88
Data_02004d88:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000051
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000051
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000051
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000002
	.4byte 0xffff0028
	.4byte Func_02002ca4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_0200033c
	.4byte 0x00008c15
	.4byte 0x095c0009
	.4byte Func_02000644
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000644
	.4byte 0x50008a05
	.4byte 0x02400058
	.4byte Func_02000690
	.4byte 0x00000006
	.4byte 0x02400058
	.4byte Func_0200071c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004e48
Data_02004e48:
	.4byte 0x00000051
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000051
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000a02
	.4byte 0xffff0003
	.4byte Func_02000958
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x0000c402
	.4byte 0xffff0007
	.4byte Func_02002f60
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02000a24
	.4byte 0x0001c814
	.4byte 0x095d0009
	.4byte Func_020007d4
	.4byte 0x00002115
	.4byte 0x0a660008
	.4byte Func_02000a98
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004ed8
Data_02004ed8:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000051
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000051
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000051
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
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000202
	.4byte 0xffff0037
	.4byte Func_020004b4
	.4byte 0x10008c15
	.4byte 0xffff000c
	.4byte Func_02000bac
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000bac
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte Func_02000b84
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000b84
	.4byte 0x50008a05
	.4byte 0x0a6a0032
	.4byte Func_02000ad8
	.4byte 0x00002115
	.4byte 0x0a640008
	.4byte Func_02000bd4
	.4byte 0x00002115
	.4byte 0x02490009
	.4byte Func_02000c28
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004fb0
Data_02004fb0:
	.4byte 0x00000051
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000051
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000051
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
	.4byte 0xffff004d
	.4byte Func_02000cc8
	.4byte 0x00000002
	.4byte 0xffff004e
	.4byte Func_02000cc8
	.4byte 0x00004602
	.4byte 0xffff0046
	.4byte Func_020004b4
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0x00002115
	.4byte 0x0a670009
	.4byte Func_02000de4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200507c
Data_0200507c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000051
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000051
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000051
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000051
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000051
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte Func_02000e18
	.4byte 0x00000002
	.4byte 0xffff004d
	.4byte Func_02000cc8
	.4byte 0x00000002
	.4byte 0xffff004e
	.4byte Func_02000cc8
	.4byte 0x00008c15
	.4byte 0x02380008
	.4byte Func_02000ecc
	.4byte 0x00001815
	.4byte 0x0251000d
	.4byte Func_0200101c
	.4byte 0x00001815
	.4byte 0x0252000e
	.4byte Func_0200101c
	.4byte 0x00001815
	.4byte 0x0253000f
	.4byte Func_0200101c
	.4byte 0x10008c15
	.4byte 0xffff000a
	.4byte Func_02001060
	.4byte 0x10008c15
	.4byte 0xffff000b
	.4byte Func_02001060
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02001060
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte Func_0200107c
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte Func_0200107c
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_0200107c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020051b4
Data_020051b4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0xffff001e
	.4byte Func_020010ec
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020051fc
Data_020051fc:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
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
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005268
Data_02005268:
	.4byte 0x00000051
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000051
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000051
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0xffff0028
	.4byte Func_02002ca4
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte Func_02001138
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte 0x00000000
	.4byte 0x00002115
	.4byte 0x0a650008
	.4byte Func_02001170
	.4byte 0x00002115
	.4byte 0x024b0009
	.4byte Func_020011d4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020052f8
Data_020052f8:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c403
	.4byte 0xffff001e
	.4byte Func_02001a1c
	.4byte 0x00004403
	.4byte 0xffff001e
	.4byte Func_02001c68
	.4byte 0x00008403
	.4byte 0xffff001e
	.4byte Func_02001cd8
	.4byte 0x00000403
	.4byte 0xffff001e
	.4byte Func_02001d40
	.4byte 0x50008a05
	.4byte 0x13490028
	.4byte Func_02001328
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200534c
Data_0200534c:
	.4byte 0x08000808
	.4byte 0x080108ff
	.4byte 0xff080008
	.4byte 0x08080801
	.4byte 0x01000808
	.4byte 0x0808ff08
	.4byte 0x08080008
	.4byte 0x08ff0108
	.section .bss,"aw",%nobits
	.global Data_0200536c
Data_0200536c:
	.space 0x00000604
	.global Data_02005970
Data_02005970:
