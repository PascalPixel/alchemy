.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008080,"ax",%progbits
	.global Func_02000080
	.thumb_func
Func_02000080:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	adds r6, r1, #0
	strb r3, [r2]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	adds r1, r6, #0
	bl Object_SetPartAttribute
	pop {r5, r6, pc}
	.section .text.x020080a4,"ax",%progbits
	.global Func_020000a4
	.thumb_func
Func_020000a4:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl Func_02001b3c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020080d8
	movs r1, #0
	bl Object_SetSpritePriority
	adds r0, r5, #0
	movs r1, #14
	bl Func_02000080
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	adds r0, r5, #0
	b .L_020080da
.L_020080d8:
	movs r0, #0
.L_020080da:
	pop {r5, r6, pc}
	.section .text.x020080dc,"ax",%progbits
	.global Func_020000dc
	.thumb_func
Func_020000dc:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl Func_02001b3c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008114
	movs r1, #1
	bl Object_SetSpritePriority
	adds r0, r5, #0
	movs r1, #15
	bl Func_02000080
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	b .L_02008116
.L_02008114:
	movs r0, #0
.L_02008116:
	pop {r5, r6, pc}
	.section .text.x02008118,"ax",%progbits
	.global Func_02000118
	.thumb_func
Func_02000118:
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
	.section .text.x02008150,"ax",%progbits
	.global Func_02000150
	.thumb_func
Func_02000150:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_02008304
	sub sp, #4
	mov r10, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r8, r1
	ldr r7, [sp, #48]
	bl Engine_ActorGet
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r8
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_02008198
	cmp r7, #0
	beq .L_02008198
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_020081a0
.L_02008198:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_020081a0:
	mov r3, r10
	bl Func_02001b3c
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020081ae
	b .L_020082f6
.L_020081ae:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02001b2c
	ldr r2, .L_02008308
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02001b34
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	ldr r3, [sp, #0]
	mov r1, r11
	strb r5, [r3, #26]
	ldr r3, .L_0200830c
	str r1, [r6, #68]
	str r3, [r6, #108]
	ldr r3, [sp, #36]
	mov r2, r9
	str r3, [r6, #72]
	ldr r3, [sp, #40]
	adds r0, r6, #0
	str r3, [r6, #76]
	ldr r3, [r2, #80]
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Object_SetSpritePriority
	movs r3, #100
	adds r3, r3, r6
	mov r9, r3
	ldr r3, .L_02008310
	mov r2, r8
	mov r1, r9
	ands r3, r2
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r1]
	cmp r3, #0
	beq .L_020082f6
	cmp r7, #0
	beq .L_020082f6
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r2
	cmp r3, #0
	beq .L_0200822c
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_0200822c:
	movs r3, #128
	lsls r3, r3, #10
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200824c
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_0200824c:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_02008260
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02008260:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_020082a6
	ldr r3, .L_02008308
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_0200828e
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_020082a0
.L_0200828e:
	ldr r2, .L_02008310
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02008310
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_020082a0:
	bl Engine_MathDivide
	str r0, [r6, #52]
.L_020082a6:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_020082c2
	adds r0, r6, #0
	movs r1, #1
	bl Func_02001b2c
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02001b34
.L_020082c2:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020082d4
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_020082d4:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020082e6
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_020082e6:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020082f6
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_020082f6:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008304:
	.4byte gPartyState
.L_02008308:
	.4byte Data_02001dcc
.L_0200830c:
	.4byte Func_02000118
.L_02008310:
	.4byte 0xffff0000
	.section .text.x02008314,"ax",%progbits
	.global Func_02000314
	.thumb_func
Func_02000314:
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
	ldr r3, .L_02008348
	mov lr, r3
	.2byte 0xf800
	pop {r5, pc}
.L_02008348:
	.4byte IwramFillWords + 0x74
	.section .text.x0200834c,"ax",%progbits
	.global Func_0200034c
	.thumb_func
Func_0200034c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r1, r0, #0
	adds r5, r3, #0
	movs r4, #8
	adds r5, #52
.L_0200835c:
	ldmia r5!, {r0}
	ldr r2, [r1]
	ldr r3, [r0, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02008382
	ldr r2, [r1, #4]
	ldr r3, [r0, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02008382
	ldr r2, [r1, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_0200838a
.L_02008382:
	adds r4, #1
	cmp r4, #63
	bls .L_0200835c
	movs r0, #0
.L_0200838a:
	pop {r5, pc}
	.section .text.x0200838c,"ax",%progbits
	.global Func_0200038c
	.thumb_func
Func_0200038c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02008504
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #12
	bl Engine_ActorGet
	ldrh r3, [r0, #6]
	ldr r1, .L_02008508
	lsrs r3, r3, #12
	lsls r5, r3, #2
	ldr r2, .L_0200850c
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
	bl Func_0200034c
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020083e2
	b .L_020084f8
.L_020083e2:
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
	bl Func_0200034c
	cmp r0, #0
	beq .L_02008418
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_020084f8
.L_02008418:
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
	bl Func_0200034c
	cmp r0, #0
	beq .L_02008444
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_020084f8
.L_02008444:
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
	bl Func_02001b64
	cmp r0, #0
	bgt .L_020084f8
	adds r3, r6, #0
	adds r3, #98
	ldrb r3, [r3]
	mov r10, r3
	cmp r3, #0
	bne .L_020084f8
	movs r1, #8
	mov r0, r8
	movs r5, #204
	bl Func_02001b2c
	lsls r5, r5, #6
	movs r0, #15
	bl WaitFrames
	adds r5, #51
	movs r0, #185
	bl Engine_AudioPlayCue
	str r5, [r6, #48]
	str r5, [r6, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl Func_02001b44
	mov r0, r8
	str r5, [r0, #48]
	str r5, [r0, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl Func_02001b44
	adds r0, r6, #0
	bl Func_02001b4c
	bl Func_02001cbc
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
	bl Func_02001b2c
.L_020084f8:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008504:
	.4byte gPartyState
.L_02008508:
	.4byte Data_02001d8c
.L_0200850c:
	.4byte 0xffff0000
	.section .text.x02008510,"ax",%progbits
	.global Func_02000510
	.thumb_func
Func_02000510:
	push {r5, r6, lr}
	adds r5, r3, #0
	ldr r3, [sp, #12]
	ldr r6, [sp, #16]
	mov r12, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	beq .L_02008562
	cmp r0, #2
	bhi .L_02008538
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r0, r0, #1
	lsls r3, r3, #3
	adds r3, r3, r0
	ldr r0, [r4, r3]
	b .L_0200853a
.L_02008538:
	ldr r0, .L_02008568
.L_0200853a:
	lsls r3, r2, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	movs r1, #0
	adds r0, r0, r3
	cmp r1, r12
	bcs .L_02008562
.L_02008548:
	lsls r3, r1, #9
	movs r2, #0
	adds r3, r0, r3
	cmp r2, r5
	bcs .L_0200855c
.L_02008552:
	adds r2, #1
	strb r6, [r3, #2]
	adds r3, #4
	cmp r2, r5
	bcc .L_02008552
.L_0200855c:
	adds r1, #1
	cmp r1, r12
	bcc .L_02008548
.L_02008562:
	movs r0, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008568:
	.4byte gMapCellBuffer
	.4byte 0x00004770
	.section .text.x02008570,"ax",%progbits
	.global Func_02000570
	.thumb_func
Func_02000570:
	push {lr}
	ldr r3, .L_020085a4
	movs r2, #3
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02008586
	movs r1, #7
	bl Object_SetPartAttribute
	b .L_0200858c
.L_02008586:
	movs r1, #0
	bl Object_SetPartAttribute
.L_0200858c:
	ldr r3, .L_020085a4
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200859e
	movs r0, #138
	bl Engine_AudioPlayCue
.L_0200859e:
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_020085a4:
	.4byte Data_0300122c
	.section .text.x020085a8,"ax",%progbits
	.global Func_020005a8
	.thumb_func
Func_020005a8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200864c
	sub sp, #56
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_02008640
	bl Random16Far
	lsls r0, r0, #1
	lsrs r0, r0, #16
	movs r2, #16
	movs r3, #3
	add r2, sp
	subs r3, r3, r0
	str r3, [r2]
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #14
	str r3, [r2, #4]
	mov r8, r2
	bl Random16Far
	lsls r3, r0, #3
	mov r2, r10
	adds r3, r3, r0
	ldr r6, [r2, #8]
	lsrs r3, r3, #16
	subs r3, #4
	lsls r3, r3, #16
	adds r6, r6, r3
	bl Random16Far
	mov r2, r10
	lsls r0, r0, #5
	ldr r5, [r2, #12]
	lsrs r0, r0, #16
	movs r3, #32
	subs r3, r3, r0
	lsls r3, r3, #16
	adds r5, r5, r3
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsrs r0, r0, #16
	movs r3, #160
	lsls r3, r3, #11
	lsls r0, r0, #16
	adds r0, r0, r3
	movs r1, #10
	bl Engine_MathDivide
	mov r3, r10
	ldr r2, [r3, #16]
	movs r3, #176
	lsls r3, r3, #12
	str r3, [sp, #8]
	mov r3, r8
	str r0, [sp, #0]
	str r3, [sp, #12]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r3, #0
	str r7, [sp, #4]
	bl Func_02000150
.L_02008640:
	movs r0, #0
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200864c:
	.4byte Data_0300122c
	.section .text.x02008650,"ax",%progbits
	.global Func_02000650
	.thumb_func
Func_02000650:
	ldr r0, .L_02008654
	bx lr
.L_02008654:
	.4byte Data_02001e58
	.section .text.x02008658,"ax",%progbits
	.global Func_02000658
	.thumb_func
Func_02000658:
	movs r0, #0
	bx lr
	.section .text.x0200865c,"ax",%progbits
	.global Func_0200065c
	.thumb_func
Func_0200065c:
	ldr r0, .L_02008660
	bx lr
.L_02008660:
	.4byte Data_020020f8
	.section .text.x02008664,"ax",%progbits
	.global Func_02000664
	.thumb_func
Func_02000664:
	push {lr}
	adds r3, r0, #0
	adds r3, #100
	ldrh r3, [r3]
	movs r1, #15
	ands r1, r3
	bl Object_SetPartAttribute
	movs r0, #0
	pop {pc}
	.section .text.x02008678,"ax",%progbits
	.global Func_02000678
	.thumb_func
Func_02000678:
	push {lr}
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {pc}
	.section .text.x02008684,"ax",%progbits
	.global Func_02000684
	.thumb_func
Func_02000684:
	push {lr}
	ldr r3, .L_020086a0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_0200869a
	ldr r0, .L_020086a4
	b .L_0200869c
.L_0200869a:
	ldr r0, .L_020086a8
.L_0200869c:
	pop {pc}
	.2byte 0x0000
.L_020086a0:
	.4byte gPartyState
.L_020086a4:
	.4byte Data_02002378
.L_020086a8:
	.4byte Data_020021b0
	.section .text.x020086ac,"ax",%progbits
	.global Func_020006ac
	.thumb_func
Func_020006ac:
	push {r5, r6, lr}
	bl Engine_PartyGetLeaderActor
	adds r5, r0, #0
	bl Engine_ActorGet
	adds r6, r0, #0
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	ldr r1, .L_02008784
	adds r0, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r0, r5, #0
	bl Object_RefreshSelectorById
	adds r0, r5, #0
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r1, #128
	lsls r1, r1, #11
	movs r2, #128
	str r1, [r6, #40]
	adds r0, r5, #0
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	ldr r3, [r6, #16]
	asrs r3, r3, #20
	cmp r3, #54
	bgt .L_0200870a
	adds r0, r5, #0
	bl Engine_ActorGet
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r2, #210
	b .L_0200871c
.L_0200870a:
	adds r0, r5, #0
	bl Engine_ActorGet
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r2, #238
.L_0200871c:
	movs r3, #10
	ldrsh r1, [r6, r3]
	lsls r2, r2, #2
	adds r0, r5, #0
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Engine_EventWait
	adds r0, r5, #0
	bl Engine_ActorGet
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl Engine_EventWait
	adds r0, r6, #0
	movs r1, #0
	bl Object_SetPartAttribute
	ldr r3, .L_02008788
	movs r1, #129
	str r3, [r6, #108]
	movs r2, #60
	adds r0, r5, #0
	lsls r1, r1, #1
	bl Func_02001c34
	adds r0, r5, #0
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	adds r0, r5, #0
	bl Engine_ActorGet
	movs r1, #0
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r3, #0
	str r3, [r6, #108]
	bl Engine_EventEnd
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008784:
	.4byte Data_02001dd8
.L_02008788:
	.4byte Func_020005a8
	.section .text.x0200878c,"ax",%progbits
	.global Func_0200078c
	.thumb_func
Func_0200078c:
	push {r5, lr}
	sub sp, #8
	bl Engine_PartyGetLeaderActor
	bl Engine_ActorGet
	adds r5, r0, #0
	movs r1, #10
	ldrsh r3, [r5, r1]
	movs r1, #18
	ldrsh r2, [r5, r1]
	ldr r1, .L_0200886c
	adds r3, r3, r1
	cmp r3, #7
	bhi .L_020087ba
	movs r3, #197
	lsls r3, r3, #2
	cmp r2, r3
	blt .L_020087ba
	movs r1, #199
	lsls r1, r1, #2
	cmp r2, r1
	blt .L_020087fe
.L_020087ba:
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #53
	movs r1, #50
	movs r2, #42
	movs r3, #49
	bl Engine_MapCopyCellsTo
	movs r3, #3
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #117
	movs r2, #41
	movs r3, #117
	movs r0, #55
	bl Engine_MapCopyCellsTo
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	adds r0, r5, #0
	adds r0, #85
	ldrb r1, [r0]
	movs r3, #1
	movs r2, #0
	orrs r3, r1
	strb r3, [r0]
	str r2, [r5, #20]
	str r2, [r5, #12]
	b .L_02008868
.L_020087fe:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008868
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	movs r0, #5
	bl Engine_EventWait
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #52
	movs r1, #50
	movs r2, #42
	movs r3, #49
	bl Engine_MapCopyCellsTo
	movs r3, #3
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #117
	movs r2, #41
	movs r3, #117
	movs r0, #52
	bl Engine_MapCopyCellsTo
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Engine_GameFlagSet
	movs r0, #161
	bl Engine_AudioPlayCue
	adds r1, r5, #0
	adds r1, #85
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	ldr r3, .L_02008870
	str r3, [r5, #20]
	str r3, [r5, #12]
	bl Engine_EventEnd
.L_02008868:
	add sp, #8
	pop {r5, pc}
.L_0200886c:
	.4byte 0xfffffd5c
.L_02008870:
	.4byte 0xfffe0000
	.section .text.x02008a0c,"ax",%progbits
	.global Func_02000a0c
	.thumb_func
Func_02000a0c:
	ldr r0, .L_02008a10
	bx lr
.L_02008a10:
	.4byte Data_02002408
	.section .text.x02008a14,"ax",%progbits
	.global Func_02000a14
	.thumb_func
Func_02000a14:
	movs r0, #0
	bx lr
	.section .text.x02008a18,"ax",%progbits
	.global Func_02000a18
	.thumb_func
Func_02000a18:
	push {lr}
	movs r0, #140
	movs r1, #1
	bl Func_02001c7c
	movs r1, #10
	movs r0, #4
	bl Func_02001c84
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r2, [r3]
	ldr r3, .L_02008a50
	str r3, [r2, #36]
	bl Func_02001c9c
	movs r0, #10
	bl Engine_EventWait
	movs r0, #1
	bl Field_DispatchTypeHandler
	bl Func_02001c8c
	bl Func_02001c94
	pop {pc}
.L_02008a50:
	.4byte Func_02000a14
	.section .text.x02008a54,"ax",%progbits
	.global Func_02000a54
	.thumb_func
Func_02000a54:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	ldr r1, .L_02008b20
	str r2, [r3]
	subs r2, #34
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #99
	bne .L_02008aa2
	ldr r3, .L_02008b24
	movs r0, #242
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
	movs r3, #243
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #22
	strh r3, [r2]
	bl Engine_EventBegin
	movs r0, #0
	bl Engine_EventPrepareSpeakers
	bl Func_02000b2c
	bl Func_02000d30
	bl Engine_EventEnd
	b .L_02008b1a
.L_02008aa2:
	movs r0, #10
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #17
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #18
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #19
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #20
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #21
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #22
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #23
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #24
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
	movs r0, #25
	bl Engine_ActorGet
	movs r1, #6
	bl Object_SetPartAttribute
.L_02008b1a:
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_02008b20:
	.4byte gPartyState
.L_02008b24:
	.4byte 0x00000005
	.section .text.x02008b28,"ax",%progbits
	.global Func_02000b28
	.thumb_func
Func_02000b28:
	movs r0, #0
	bx lr
	.section .text.x02008b2c,"ax",%progbits
	.global Func_02000b2c
	.thumb_func
Func_02000b2c:
	push {lr}
	movs r1, #178
	movs r2, #204
	movs r0, #4
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02001be4
	movs r1, #174
	movs r2, #210
	movs r0, #5
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02001be4
	movs r1, #182
	movs r2, #210
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02001be4
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #166
	movs r1, #1
	movs r2, #206
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #0
	bl Motion_CamBounds
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #172
	str r3, [r2]
	adds r3, #180
	adds r2, r1, r3
	movs r3, #60
	str r3, [r2]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #20
	bl Engine_EventWait
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Engine_EventWait
	movs r0, #5
	movs r1, #3
	bl Engine_ActorSetAnimation
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Engine_EventWait
	movs r1, #166
	movs r2, #206
	movs r0, #4
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Engine_EventWait
	movs r1, #178
	movs r2, #206
	movs r0, #8
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	bl Func_02000a18
	bl SceneState_RunActor13AtColumn42Setup
	movs r0, #20
	bl Engine_EventWait
	movs r1, #182
	movs r2, #210
	movs r0, #8
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #178
	movs r2, #204
	movs r0, #4
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Engine_EventWait
	movs r0, #5
	movs r1, #3
	bl Engine_ActorSetAnimation
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Engine_EventWait
	movs r0, #5
	movs r1, #4
	bl Object_LinkObjectAndSetCallback
	movs r0, #8
	movs r1, #4
	bl Object_LinkObjectAndSetCallback
	movs r1, #186
	movs r2, #204
	movs r0, #4
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #186
	movs r2, #195
	lsls r2, r2, #2
	lsls r1, r1, #2
	movs r0, #4
	bl ObjectMotion_SetPositionAndReset
	movs r0, #129
	bl Engine_AudioPlayCue
	movs r1, #1
	negs r1, r1
	movs r0, #4
	bl Func_02001ccc
	movs r0, #50
	bl Engine_EventWait
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl Func_02001be4
	pop {pc}
	.2byte 0x0000
	.section .text.x02008d30,"ax",%progbits
	.global Func_02000d30
	.thumb_func
Func_02000d30:
	push {r5, lr}
	ldr r0, .L_02009130
	sub sp, #8
	bl Func_02001c1c
	movs r1, #1
	movs r0, #8
	bl Object_AttachWorkTargetToObject
	bl Func_02001c4c
	movs r0, #30
	bl Engine_EventWait
	movs r1, #166
	movs r2, #210
	movs r0, #5
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #174
	movs r2, #210
	lsls r2, r2, #2
	lsls r1, r1, #2
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Engine_EventWait
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Engine_EventWait
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Engine_EventWait
	movs r1, #174
	movs r2, #230
	lsls r1, r1, #2
	lsls r2, r2, #2
	movs r0, #8
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #10
	bl Engine_EventWait
	movs r1, #166
	movs r2, #222
	lsls r1, r1, #2
	lsls r2, r2, #2
	movs r0, #5
	bl ObjectMotion_SetPositionAndReset
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_02001c34
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r1, #174
	movs r2, #222
	movs r0, #8
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r0, #174
	movs r1, #1
	movs r2, #230
	movs r3, #1
	lsls r2, r2, #18
	negs r1, r1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02001c4c
	movs r0, #20
	bl Engine_EventWait
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #2
	movs r2, #50
	adds r1, #255
	movs r0, #8
	bl Func_02001c34
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Engine_EventWait
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #8
	bl Func_02001c34
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02001c34
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #30
	bl Engine_EventWait
	movs r0, #198
	bl Engine_AudioPlayCue
	movs r1, #170
	movs r2, #230
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02001be4
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #9
	bl Func_02001cd4
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Engine_EventWait
	movs r0, #20
	bl Engine_EventWait
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r0, #5
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Engine_EventWait
	movs r0, #5
	movs r1, #8
	movs r2, #40
	bl Object_LinkPair
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02001c34
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02001c34
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_02001c34
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Engine_EventWait
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	movs r0, #5
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #5
	bl Object_LinkObjectAndSetCallback
	movs r2, #32
	negs r2, r2
	movs r1, #0
	movs r0, #5
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_02001c34
	movs r1, #8
	movs r2, #8
	negs r2, r2
	movs r0, #9
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Engine_EventWait
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_02001c34
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #9
	bl Func_02001c34
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #8
	bl Func_02001c34
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #5
	b .L_02009134
	.2byte 0x0000
.L_02009130:
	.4byte 0x00001570
.L_02009134:
	bl Func_02001c34
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_02001c34
	movs r1, #240
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #20
	bl Engine_EventWait
	movs r1, #131
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #5
	bl Func_02001c34
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #9
	bl Func_02001c34
	movs r0, #20
	bl Engine_EventWait
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #4
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Engine_EventWait
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #8
	movs r2, #30
	adds r1, #255
	movs r0, #5
	bl Func_02001c34
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #7
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Engine_EventWait
	movs r1, #144
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02001c34
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #5
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r0, #5
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r0, #5
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #6
	movs r2, #50
	adds r1, #255
	movs r0, #5
	bl Func_02001c34
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Engine_EventWait
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r2, #0
	movs r1, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Engine_EventWait
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Engine_EventWait
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02001c34
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r2, #0
	movs r1, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #8
	bl Func_02001c34
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #5
	bl Func_02001c34
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #6
	movs r2, #30
	adds r1, #255
	movs r0, #8
	bl Func_02001c34
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #8
	movs r2, #30
	adds r1, #255
	movs r0, #5
	bl Func_02001c34
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Engine_EventWait
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #5
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Engine_EventWait
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #224
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #50
	bl Engine_EventWait
	movs r1, #144
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #5
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_02001c34
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02001c24
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #240
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #25
	bl Engine_EventWait
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Engine_EventWait
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #5
	bl Func_02001c34
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r0, #5
	movs r1, #8
	movs r2, #50
	bl Object_LinkPair
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02001c34
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #6
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Engine_EventWait
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02001c34
	movs r0, #10
	bl Engine_EventWait
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_02001c34
	movs r1, #240
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #5
	bl Func_02001c34
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #9
	bl Func_02001c34
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #240
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Engine_EventWait
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #20
	bl Engine_EventWait
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #9
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Engine_EventWait
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Engine_EventWait
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Engine_EventWait
	movs r1, #128
	lsls r1, r1, #5
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Engine_EventWait
	movs r1, #6
	movs r2, #30
	adds r1, #255
	movs r0, #5
	bl Func_02001c34
	movs r1, #0
	movs r0, #5
	bl Func_02001c24
	movs r0, #10
	bl Engine_EventWait
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Engine_EventWait
	movs r0, #9
	movs r1, #3
	bl Engine_ActorSetAnimation
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Engine_EventWait
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #8
	ldr r1, .L_02009af4
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #2
	bl Engine_ActorSetAnimation
	movs r0, #5
	bl Engine_ActorGet
	cmp r0, #0
	beq .L_0200998e
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #8
	bl ObjectMotion_ResetAndSetPosition
.L_0200998e:
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02001be4
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_02009af4
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #2
	bl Engine_ActorSetAnimation
	movs r0, #5
	bl Engine_ActorGet
	cmp r0, #0
	beq .L_020099cc
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
.L_020099cc:
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl Func_02001be4
	movs r0, #10
	bl Engine_EventWait
	movs r0, #5
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #170
	movs r2, #236
	lsls r1, r1, #2
	lsls r2, r2, #2
	movs r0, #5
	bl ObjectMotion_SetPositionAndReset
	movs r0, #15
	bl Engine_EventWait
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #50
	bl Engine_EventWait
	movs r1, #0
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #15
	bl Engine_EventWait
	movs r1, #182
	movs r2, #236
	movs r0, #5
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #182
	movs r2, #232
	movs r0, #5
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #182
	movs r2, #228
	lsls r2, r2, #18
	lsls r1, r1, #18
	movs r0, #5
	bl Func_02001be4
	movs r0, #129
	bl Engine_AudioPlayCue
	movs r1, #1
	negs r1, r1
	movs r0, #5
	bl Func_02001ccc
	movs r0, #40
	bl Engine_EventWait
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #53
	movs r1, #50
	movs r2, #42
	movs r3, #49
	bl Engine_MapCopyCellsTo
	movs r3, #3
	str r3, [sp, #0]
	movs r5, #5
	movs r3, #117
	movs r1, #117
	movs r2, #41
	movs r0, #55
	str r5, [sp, #4]
	bl Engine_MapCopyCellsTo
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	movs r1, #166
	movs r2, #198
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02001be4
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r0, r0
	negs r2, r2
	movs r3, #0
	bl Motion_CamBounds
	ldr r3, .L_02009af8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	str r5, [r3]
	movs r0, #4
	bl Party_RemoveActiveOwner
	movs r0, #50
	bl Engine_EventWait
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #218
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #60
	str r3, [r2]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #172
	str r3, [r2]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #22
	bl Func_02001c54
	add sp, #8
	pop {r5, pc}
.L_02009af4:
	.4byte 0x00013333
.L_02009af8:
	.4byte gPartyState
	.section .rodata.x02009ce4,"a",%progbits
.L_02009ce4:
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
.L_02009d1c:
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
.L_02009d54:
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
	.global Data_02001d8c
Data_02001d8c:
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
	.global Data_02001dcc
Data_02001dcc:
	.4byte .L_02009ce4
	.4byte .L_02009d1c
	.4byte .L_02009d54
	.global Data_02001dd8
Data_02001dd8:
	.4byte 0x00000027
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte Func_02000570
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00011999
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00011999
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000e666
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000e666
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000011
	.global Data_02001e58
Data_02001e58:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0xc00000c8
	.4byte 0x00000000
	.4byte 0x00f00038
	.4byte 0x000000d8
	.4byte 0xffff0002
	.4byte 0x00000078
	.4byte 0x40000068
	.4byte 0x00000000
	.4byte 0x00f00038
	.4byte 0x000000d8
	.4byte 0xffff0003
	.4byte 0x00000158
	.4byte 0xc00000e8
	.4byte 0x01000000
	.4byte Data_02000000 + 0x30
	.4byte 0x000000f8
	.4byte 0xffff0004
	.4byte 0x000001d8
	.4byte 0x400000b8
	.4byte 0x01000000
	.4byte Data_02000000 + 0x30
	.4byte 0x000000f8
	.4byte 0xffff0005
	.4byte 0x00000178
	.4byte 0x400000b8
	.4byte 0x01000000
	.4byte Data_02000000 + 0x30
	.4byte 0x000000f8
	.4byte 0xffff0006
	.4byte 0x00000158
	.4byte 0x40000068
	.4byte 0x01000000
	.4byte Data_02000000 + 0x30
	.4byte 0x000000f8
	.4byte 0xffff0007
	.4byte 0x00000348
	.4byte 0xc00000f8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff0008
	.4byte 0x000003c8
	.4byte 0x400000a8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff0009
	.4byte 0x00000358
	.4byte 0x40000058
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000a
	.4byte 0x00000328
	.4byte 0x40000058
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000b
	.4byte 0x000002c8
	.4byte 0x400000a8
	.4byte 0x02a00000
	.4byte 0x03f00020
	.4byte 0x00000100
	.4byte 0xffff000c
	.4byte 0x000000c8
	.4byte 0xc0000198
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff000d
	.4byte 0x00000068
	.4byte 0xc00001f8
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff000e
	.4byte 0x00000068
	.4byte 0x400001d8
	.4byte 0x00100000
	.4byte 0x01000120
	.4byte 0x00000208
	.4byte 0xffff0014
	.4byte 0x00000208
	.4byte 0x400002b8
	.4byte 0x01300000
	.4byte 0x02400158
	.4byte 0x000002d8
	.4byte 0xffff0015
	.4byte 0x00000158
	.4byte 0x40000238
	.4byte 0x01300000
	.4byte 0x02400158
	.4byte 0x000002d8
	.4byte 0xffff0016
	.4byte 0x000002d8
	.4byte 0x400003a8
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff0017
	.4byte 0x000002e8
	.4byte 0x40000318
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff0018
	.4byte 0x00000188
	.4byte 0x400003b8
	.4byte 0x01100000
	.4byte Data_0200024c + 0xdc
	.4byte 0x000003d8
	.4byte 0xffff0019
	.4byte 0x00000188
	.4byte 0x40000358
	.4byte 0x01100000
	.4byte Data_0200024c + 0xdc
	.4byte 0x000003d8
	.4byte 0xffff001a
	.4byte 0x000002f8
	.4byte 0x400003a8
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff001e
	.4byte 0x00000058
	.4byte 0x40000368
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0xffff001f
	.4byte 0x000000b8
	.4byte 0x40000368
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0xffff0020
	.4byte 0x000000b8
	.4byte 0x40000288
	.4byte 0x00100000
	.4byte 0x01000258
	.4byte 0x00000398
	.4byte 0xffff0062
	.4byte 0x000002e8
	.4byte 0x40000318
	.4byte 0x02580000
	.4byte 0x034802e0
	.4byte 0x000003b8
	.4byte 0xffff0063
	.4byte 0x000002e8
	.4byte 0x40000318
	.4byte 0x02400000
	.4byte 0xffff02e0
	.4byte 0x000003c8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020020f8
Data_020020f8:
	.4byte 0x00000005
	.4byte 0x00109004
	.4byte 0x00203005
	.4byte 0x00302005
	.4byte 0x0041f005
	.4byte 0x0051e005
	.4byte 0x00607005
	.4byte 0x00706005
	.4byte 0x00820005
	.4byte 0x00916005
	.4byte 0x00a0c005
	.4byte 0x00b0d005
	.4byte 0x00c0a005
	.4byte 0x00d0b005
	.4byte 0x00e18005
	.4byte 0x0141a005
	.4byte 0x01519005
	.4byte 0x01609005
	.4byte 0x0180e005
	.4byte 0x01915005
	.4byte 0x01a14005
	.4byte 0x01e05005
	.4byte 0x01f04005
	.4byte 0x02008005
	.4byte 0x000001ff
.L_0200a15c:
	.4byte 0x00000016
	.4byte 0x00000023
	.4byte 0x00002126
	.4byte 0x00000016
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_02000664
	.4byte 0x00000011
.L_0200a180:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte Func_02000678
	.4byte 0x00000011
	.global Data_020021b0
Data_020021b0:
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x000c0000
	.4byte 0x00024000
	.4byte 0xffff00e6
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x000c0000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0xffff013b
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002c000
	.4byte 0xffff013b
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x0102c000
	.4byte 0xffff01c4
	.4byte .L_0200a180
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002378
Data_02002378:
	.4byte 0xffff0004
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0150
	.4byte .L_0200a15c
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002408
Data_02002408:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
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
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000021
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
	.4byte 0x00000021
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000031
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000031
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000031
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000031
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000031
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000021
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x00000021
	.4byte 0xffff001a
	.4byte 0x0000001a
	.4byte 0x00000021
	.4byte 0xffff001e
	.4byte 0x0000001e
	.4byte 0x00000021
	.4byte 0xffff001f
	.4byte 0x0000001f
	.4byte 0x00000021
	.4byte 0xffff0020
	.4byte 0x00000020
	.4byte 0x00000002
	.4byte Data_02000000 + 0x2d
	.4byte Func_020006ac
	.4byte 0x00000002
	.4byte 0xffff002e
	.4byte Func_0200078c
	.4byte 0x00000003
	.4byte 0xffff0023
	.4byte VinasuChojo_ReadRelief
	.4byte 0x00000003
	.4byte 0xffff0024
	.4byte VinasuChojo_ReadRelief
	.4byte 0x00000003
	.4byte 0xffff0025
	.4byte VinasuChojo_ReadRelief
	.4byte 0x00000003
	.4byte 0xffff0026
	.4byte VinasuChojo_ReadRelief
	.4byte 0x00000003
	.4byte 0xffff0027
	.4byte VinasuChojo_ReadRelief
	.4byte 0x00000003
	.4byte 0xffff0028
	.4byte VinasuChojo_ReadTrueHeart
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte SceneState_ApplySixRectsAfterFlag161
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte SceneState_ApplySixRectsAfter161
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
