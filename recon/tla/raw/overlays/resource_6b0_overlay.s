.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {r5, r6, lr}
	adds r4, r1, #0
	movs r1, #192
	lsls r1, r1, #18
	adds r1, #128
	ldr r6, [r1]
	ldr r1, .L_020080dc
	adds r5, r0, #0
	str r5, [r1]
	ldr r1, .L_020080e0
	str r4, [r1]
	ldr r1, .L_020080e4
	str r2, [r1]
	ldr r2, .L_020080e8
	str r3, [r2]
	movs r2, #255
	ldrh r3, [r5]
	b .L_02008082
.L_0200805c:
	ldrh r0, [r4]
	adds r4, #2
	ldrh r2, [r4]
	adds r4, #2
	ldrh r1, [r5]
	ldrh r3, [r4]
	adds r5, #2
	adds r4, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	movs r2, #160
	lsls r2, r2, #19
	lsls r1, r1, #1
	orrs r3, r0
	adds r1, r1, r2
	strh r3, [r1]
	ldrh r3, [r5]
	movs r2, #255
.L_02008082:
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_02008090
	ldrh r3, [r4]
	cmp r3, r2
	bne .L_0200805c
.L_02008090:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r0, #160
	lsls r2, r2, #24
	adds r3, #212
	lsls r0, r0, #19
	adds r1, r6, #0
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #224
	lsls r2, r2, #1
	adds r1, r6, r2
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, .L_020080ec
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02003458
	ldr r3, .L_020080f0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_020080da
	bl Func_02000328
.L_020080da:
	pop {r5, r6, pc}
.L_020080dc:
	.4byte Data_02004444
.L_020080e0:
	.4byte Data_02004448
.L_020080e4:
	.4byte Data_0200444c
.L_020080e8:
	.4byte Data_02004438
.L_020080ec:
	.4byte 0x05000200
.L_020080f0:
	.4byte gPartyState
	.section .text.x020081d4,"ax",%progbits
	.balign 4
	.global Func_020001d4
	.thumb_func
Func_020001d4:
	push {lr}
	ldr r1, .L_020081ec
	ldrh r3, [r0]
	movs r2, #0
	cmp r3, r1
	beq .L_020081f0
.L_020081e0:
	adds r0, #2
	ldrh r3, [r0]
	adds r2, #1
	cmp r3, r1
	bne .L_020081e0
	b .L_020081f0
.L_020081ec:
	.4byte 0x0000ffff
.L_020081f0:
	subs r2, #1
	adds r0, r2, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x020081f8,"ax",%progbits
	.global Func_020001f8
	.thumb_func
Func_020001f8:
	push {r5, r6, r7, lr}
	ldr r1, .L_020082a8
	movs r4, #0
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_02008232
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02008232
	ldr r0, .L_020082ac
	movs r4, #1
	ldrh r2, [r0]
	strh r2, [r1]
	movs r1, #128
	lsls r3, r2, #16
	lsls r1, r1, #10
	cmp r3, r1
	bls .L_02008232
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	strh r3, [r0]
.L_02008232:
	cmp r4, #0
	bne .L_02008238
	b .L_02008324
.L_02008238:
	ldr r3, .L_020082b0
	ldr r6, .L_020082b4
	ldr r1, [r3]
	ldrh r3, [r6]
	movs r5, #0
	cmp r5, r3
	bcs .L_02008286
	ldr r3, .L_020082b8
	ldr r2, .L_020082bc
	ldr r7, [r3]
	mov lr, r2
	mov r12, r6
.L_02008250:
	mov r3, lr
	ldrh r2, [r3]
	ldrh r3, [r6]
	movs r0, #160
	muls r3, r2
	adds r3, r3, r5
	lsls r3, r3, #1
	ldrh r3, [r3, r7]
	lsls r0, r0, #19
	lsls r3, r3, #1
	adds r4, r3, r0
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	adds r1, #2
	ldrh r3, [r1]
	adds r1, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	adds r5, #1
	mov r2, r12
	ldrh r3, [r2]
	cmp r5, r3
	bcc .L_02008250
.L_02008286:
	ldr r3, .L_020082b4
	movs r0, #160
	ldrh r1, [r3]
	ldr r3, .L_020082c0
	lsls r2, r1, #1
	ldr r3, [r3]
	lsls r0, r0, #19
	ldrh r3, [r2, r3]
	adds r2, r2, r1
	lsls r3, r3, #1
	adds r4, r3, r0
	ldr r3, .L_020082c4
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_020082cc
	ldr r3, .L_020082c8
	b .L_020082ce
.L_020082a8:
	.4byte Data_0200443c
.L_020082ac:
	.4byte Data_02004440
.L_020082b0:
	.4byte Data_0200444c
.L_020082b4:
	.4byte Data_02004450
.L_020082b8:
	.4byte Data_02004438
.L_020082bc:
	.4byte Data_02004454
.L_020082c0:
	.4byte Data_02004444
.L_020082c4:
	.4byte Data_02004434
.L_020082c8:
	.4byte Data_02004448
.L_020082cc:
	ldr r3, .L_02008314
.L_020082ce:
	lsls r2, r2, #1
	ldr r3, [r3]
	adds r1, r3, r2
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	ldrh r3, [r1, #2]
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	ldr r1, .L_02008318
	ldr r2, .L_0200830c
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r1, .L_0200831c
	ldr r2, .L_02008320
	ldrh r3, [r1]
	adds r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	ldrh r2, [r2]
	lsrs r3, r3, #16
	cmp r3, r2
	bne .L_02008324
	ldr r3, .L_02008310
	strh r3, [r1]
	b .L_02008324
	.2byte 0x0000
.L_0200830c:
	.4byte 0x00000001
.L_02008310:
	.4byte 0x00000000
.L_02008314:
	.4byte Data_0200444c
.L_02008318:
	.4byte Data_02004434
.L_0200831c:
	.4byte Data_02004454
.L_02008320:
	.4byte Data_02004450
.L_02008324:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020083f6,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008446,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200847e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008480,"ax",%progbits
	.global Func_02000480
	.thumb_func
Func_02000480:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r5, r0, #0
	movs r0, #0
	mov r8, r2
	str r3, [sp, #0]
	mov r10, r1
	ldr r7, [sp, #48]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r2, r10
	ands r3, r2
	mov r9, r0
	cmp r3, #0
	beq .L_020084be
	cmp r7, #0
	beq .L_020084be
	movs r3, #24
	ldrsh r0, [r7, r3]
	adds r2, r6, #0
	b .L_020084c4
.L_020084be:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
.L_020084c4:
	adds r1, r5, #0
	mov r3, r8
	bl Engine_ObjectCreate
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020084d4
	b .L_02008638
.L_020084d4:
	ldr r1, [r6, #80]
	movs r5, #15
	mov r8, r1
	mov r1, r10
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	bl Func_020032e0
	ldr r2, .L_02008648
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_020032e8
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r3, .L_0200864c
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
	ldr r3, .L_02008650
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_02008638
	cmp r7, #0
	beq .L_02008638
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02008556
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_02008556:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200858e
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
.L_0200858e:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_020085a2
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_020085a2:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_020085e8
	ldr r3, .L_02008648
	mov r1, r11
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_020085d0
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_020085e2
.L_020085d0:
	ldr r2, .L_02008650
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02008650
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_020085e2:
	bl __divsi3
	str r0, [r6, #52]
.L_020085e8:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_02008604
	adds r0, r6, #0
	movs r1, #1
	bl Func_020032e0
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_020032e8
.L_02008604:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02008616
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #18]
.L_02008616:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02008628
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_02008628:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02008638
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_02008638:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008648:
	.4byte Data_02003840
.L_0200864c:
	.4byte BattleFx_UpdateObjectMotionScaleAndLinkedAngle
.L_02008650:
	.4byte 0xffff0000
	.section .text.x020086f8,"ax",%progbits
	.global Func_020006f8
	.thumb_func
Func_020006f8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	ldr r2, [r7, #76]
	ldr r3, [r7, #48]
	movs r0, #0
	cmp r2, r3
	blt .L_0200878c
	movs r3, #102
	adds r3, r3, r7
	ldrh r2, [r3]
	adds r5, r7, #0
	adds r5, #100
	mov r8, r3
	ldrh r3, [r5]
	lsls r2, r2, #16
	asrs r2, r2, #17
	adds r3, r3, r2
	strh r3, [r5]
	ldr r3, [r7, #68]
	adds r6, r7, #0
	str r3, [r7, #8]
	ldr r3, [r7, #52]
	adds r6, #8
	str r3, [r7, #12]
	ldr r3, [r7, #72]
	adds r2, r6, #0
	str r3, [r7, #16]
	ldr r0, [r7, #76]
	movs r3, #0
	ldrsh r1, [r5, r3]
	bl Vector_AddPolarOffsetFar
	adds r0, r7, #0
	bl Func_020006a4
	ldr r2, [r7, #48]
	ldr r3, [r7, #76]
	asrs r2, r2, #1
	subs r3, r3, r2
	str r3, [r7, #76]
	mov r3, r8
	ldrh r2, [r3]
	ldrh r3, [r5]
	lsls r2, r2, #16
	asrs r2, r2, #17
	adds r3, r3, r2
	strh r3, [r5]
	ldr r3, [r7, #68]
	adds r2, r6, #0
	str r3, [r6]
	ldr r3, [r7, #52]
	ldr r0, [r7, #76]
	str r3, [r7, #12]
	ldr r3, [r7, #72]
	str r3, [r7, #16]
	movs r3, #0
	ldrsh r1, [r5, r3]
	bl Vector_AddPolarOffsetFar
	adds r0, r7, #0
	bl Func_020006a4
	ldr r2, [r7, #48]
	ldr r3, [r7, #76]
	asrs r2, r2, #1
	subs r3, r3, r2
	str r3, [r7, #76]
	ldr r2, [r7, #20]
	ldr r3, [r7, #52]
	movs r0, #1
	subs r3, r3, r2
	str r3, [r7, #52]
.L_0200878c:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008794,"ax",%progbits
	.global Func_02000794
	.thumb_func
Func_02000794:
	push {r5, lr}
	sub sp, #56
	add r4, sp, #16
	movs r3, #2
	str r3, [r4]
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #98
	ldrb r3, [r3]
	ldr r0, [r5, #8]
	str r3, [r4, #4]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #8]
	ldr r1, [r5, #12]
	ldr r2, [r5, #16]
	movs r3, #0
	str r4, [sp, #12]
	bl Func_02000480
	ldr r2, [r5, #68]
	ldr r3, [r5, #72]
	asrs r2, r2, #20
	asrs r3, r3, #20
	adds r2, #64
	adds r1, r3, #0
	movs r0, #1
	str r0, [sp, #0]
	str r0, [sp, #4]
	adds r1, #26
	adds r0, r2, #0
	bl Func_02003328
	movs r0, #0
	add sp, #56
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200888c,"ax",%progbits
	.global Func_0200088c
	.thumb_func
Func_0200088c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	lsls r2, r2, #16
	sub sp, #8
	adds r6, r0, #0
	asrs r2, r2, #16
	movs r0, #168
	str r2, [sp, #4]
	mov r9, r1
	ldr r2, [r6, #4]
	ldr r1, [r6]
	ldr r3, [r6, #8]
	lsls r0, r0, #2
	bl Engine_ObjectCreate
	mov r2, r9
	ldr r3, [r2]
	ldr r1, [r6]
	adds r7, r0, #0
	subs r1, r1, r3
	asrs r3, r1, #16
	mov r10, r3
	ldr r0, [r6, #8]
	ldr r3, [r2, #8]
	subs r0, r0, r3
	asrs r2, r0, #16
	mov r8, r2
	bl ArcTan2
	adds r3, r7, #0
	adds r3, #100
	ldr r1, .L_02008914
	adds r2, r3, #0
	str r3, [sp, #0]
	strh r0, [r2]
	mov r11, r1
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	movs r2, #2
	ands r2, r5
	movs r3, #15
	subs r2, #1
	ands r3, r0
	muls r3, r2
	add r1, sp, #4
	adds r2, r7, #0
	ldrb r1, [r1]
	lsls r3, r3, #8
	adds r2, #102
	strh r3, [r2]
	adds r3, r7, #0
	adds r3, #98
	strb r1, [r3]
	mov r2, r11
	subs r3, #13
	strb r2, [r3]
	mov r1, r9
	ldr r3, [r1]
	b .L_02008918
	.2byte 0x0000
.L_02008914:
	.4byte 0x00000000
.L_02008918:
	mov r2, r10
	str r3, [r7, #68]
	mov r0, r10
	muls r0, r2
	ldr r3, [r1, #8]
	mov r1, r8
	str r3, [r7, #72]
	mov r3, r8
	muls r3, r1
	adds r0, r0, r3
	ldr r3, .L_020089b0
	mov lr, r3
	.2byte 0xf800
	lsls r0, r0, #16
	str r0, [r7, #76]
	cmp r0, #0
	bge .L_0200893c
	adds r0, #15
.L_0200893c:
	asrs r3, r0, #4
	str r3, [r7, #48]
	mov r1, r9
	ldr r3, [r6, #4]
	str r3, [r7, #52]
	ldr r2, [r6, #4]
	ldr r3, [r1, #4]
	subs r0, r2, r3
	cmp r0, #0
	bge .L_02008952
	adds r0, #15
.L_02008952:
	asrs r3, r0, #4
	str r3, [r7, #20]
	adds r0, r7, #0
	movs r1, #1
	bl Animation_SetStateFlags
	ldr r2, [sp, #4]
	adds r0, r7, #0
	lsls r1, r2, #16
	lsrs r1, r1, #16
	bl Object_SetPartAttribute
	adds r0, r7, #0
	movs r1, #2
	bl Object_SetSpritePriority
	adds r0, r7, #0
	movs r1, #7
	bl Func_020032e0
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r2, [sp, #0]
	ldr r0, [r7, #76]
	movs r3, #0
	ldrsh r1, [r2, r3]
	adds r2, r7, #0
	adds r2, #8
	bl Vector_AddPolarOffsetFar
	ldr r1, .L_020089b4
	adds r0, r7, #0
	bl Func_020032e8
	adds r0, r7, #0
	bl Func_020006f8
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020089b0:
	.4byte IwramFillWords + 0x74
.L_020089b4:
	.4byte Data_02003684
	.section .text.x020089b8,"ax",%progbits
	.global Func_020009b8
	.thumb_func
Func_020009b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	lsls r2, r2, #16
	lsls r3, r3, #16
	asrs r2, r2, #16
	asrs r3, r3, #16
	mov r9, r0
	mov r10, r1
	mov r8, r2
	mov r11, r3
	movs r7, #0
.L_020089d8:
	movs r0, #168
	movs r3, #152
	lsls r3, r3, #16
	mov r1, r9
	mov r2, r10
	lsls r0, r0, #2
	bl Engine_ObjectCreate
	adds r5, r0, #0
	lsls r6, r7, #12
	adds r2, r5, #0
	movs r0, #128
	adds r2, #8
	lsls r0, r0, #14
	adds r1, r6, #0
	bl Vector_AddPolarOffsetFar
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	mov r1, r11
	adds r3, #2
	strh r1, [r3]
	subs r3, #4
	strb r7, [r3]
	mov r1, r8
	adds r3, #1
	strb r1, [r3]
	mov r3, r9
	str r3, [r5, #68]
	movs r3, #128
	ldr r2, .L_02008a54
	lsls r3, r3, #14
	str r3, [r5, #76]
	adds r3, r5, #0
	adds r3, #85
	mov r1, r10
	str r1, [r5, #72]
	adds r0, r5, #0
	strb r2, [r3]
	movs r1, #1
	bl Animation_SetStateFlags
	mov r3, r8
	lsls r1, r3, #16
	adds r0, r5, #0
	lsrs r1, r1, #16
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetSpritePriority
	adds r0, r5, #0
	movs r1, #7
	bl Func_020032e0
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	b .L_02008a58
.L_02008a54:
	.4byte 0x00000000
.L_02008a58:
	adds r7, #2
	adds r0, r5, #0
	ldr r1, .L_02008a74
	bl Func_020032e8
	cmp r7, #16
	bne .L_020089d8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a74:
	.4byte Data_02003584
	.section .text.x02008a78,"ax",%progbits
	.global Func_02000a78
	.thumb_func
Func_02000a78:
	push {r5, r6, lr}
	adds r3, r1, #0
	adds r3, #3
	adds r4, r0, #0
	lsls r3, r3, #16
	lsls r4, r4, #16
	lsls r5, r2, #16
	movs r0, #168
	adds r2, r3, #0
	movs r3, #152
	adds r1, r4, #0
	lsls r3, r3, #16
	lsls r0, r0, #2
	bl Engine_ObjectCreate
	asrs r5, r5, #16
	adds r6, r0, #0
	adds r2, r6, #0
	lsls r5, r5, #16
	adds r2, #85
	movs r3, #4
	lsrs r5, r5, #16
	strb r3, [r2]
	adds r1, r5, #0
	bl Object_SetPartAttribute
	adds r0, r6, #0
	movs r1, #2
	bl Object_SetSpritePriority
	adds r0, r6, #0
	movs r1, #7
	bl Func_020032e0
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008ac8,"ax",%progbits
	.global Func_02000ac8
	.thumb_func
Func_02000ac8:
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
	sub sp, #108
	str r3, [sp, #36]
	ldr r3, .L_02008bc8
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #106
	adds r3, #82
	ldrh r3, [r3]
	add r1, sp
	mov r9, r1
	mov r2, r9
	strh r3, [r2]
	ldr r4, [sp, #36]
	movs r3, #0
	str r3, [sp, #24]
	str r3, [sp, #20]
	movs r5, #170
	lsls r5, r5, #1
	adds r3, r4, r5
	ldrh r2, [r3]
	mov r8, r0
	adds r3, r2, #0
	adds r3, #193
	lsls r2, r2, #16
	asrs r0, r2, #16
	lsls r3, r3, #16
	asrs r3, r3, #16
	adds r1, r0, #0
	subs r1, #50
	mov r10, r3
	movs r3, #1
	ands r3, r1
	movs r2, #204
	lsls r3, r3, #8
	lsls r2, r2, #1
	adds r2, r3, r2
	str r2, [sp, #32]
	movs r2, #2
	ands r2, r1
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #5
	movs r6, #0
	adds r3, #168
	mov r11, r6
	str r6, [sp, #16]
	str r3, [sp, #28]
	cmp r0, #51
	beq .L_02008b70
	cmp r0, #51
	bgt .L_02008b54
	cmp r0, #50
	beq .L_02008b5e
	b .L_02008bcc
.L_02008b54:
	cmp r0, #52
	beq .L_02008b86
	cmp r0, #53
	beq .L_02008b9e
	b .L_02008bcc
.L_02008b5e:
	movs r0, #160
	movs r3, #130
	lsls r0, r0, #4
	movs r6, #48
	lsls r3, r3, #2
	adds r0, #135
	str r3, [sp, #24]
	str r6, [sp, #20]
	b .L_02008bb4
.L_02008b70:
	movs r0, #48
	str r0, [sp, #20]
	movs r0, #160
	movs r5, #138
	lsls r0, r0, #4
	movs r4, #3
	lsls r5, r5, #2
	adds r0, #136
	str r4, [sp, #16]
	movs r6, #68
	b .L_02008bb2
.L_02008b86:
	movs r0, #160
	movs r2, #252
	lsls r0, r0, #4
	movs r1, #10
	lsls r2, r2, #1
	movs r3, #40
	adds r0, #137
	str r1, [sp, #16]
	movs r6, #88
	str r2, [sp, #24]
	str r3, [sp, #20]
	b .L_02008bb4
.L_02008b9e:
	movs r0, #40
	str r0, [sp, #20]
	movs r0, #160
	movs r5, #142
	lsls r0, r0, #4
	movs r4, #2
	lsls r5, r5, #2
	adds r0, #138
	str r4, [sp, #16]
	movs r6, #108
.L_02008bb2:
	str r5, [sp, #24]
.L_02008bb4:
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008bcc
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02009078
	.2byte 0x0000
.L_02008bc8:
	.4byte gPartyState
.L_02008bcc:
	lsls r5, r6, #16
	lsrs r7, r5, #16
	adds r3, r7, #0
	adds r3, #18
	cmp r7, r3
	beq .L_02008bfe
.L_02008bd8:
	adds r0, r7, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008bf4
	subs r3, r7, r6
	mov r1, r11
	add r2, sp, #84
	strb r3, [r2, r1]
	mov r3, r11
	adds r3, #1
	lsls r3, r3, #24
	lsrs r3, r3, #24
	mov r11, r3
.L_02008bf4:
	lsrs r3, r5, #16
	adds r7, #1
	adds r3, #18
	cmp r7, r3
	bne .L_02008bd8
.L_02008bfe:
	mov r2, r11
	cmp r2, #0
	bne .L_02008c06
	b .L_02009078
.L_02008c06:
	bl Func_02003398
	movs r0, #0
	bl Func_02003488
	ldr r5, .L_02008d14
	movs r3, #133
	lsls r3, r3, #2
	movs r1, #204
	movs r2, #204
	adds r5, r5, r3
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r2, #102
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02003420
	ldr r0, [r5]
	ldr r1, [sp, #32]
	ldr r2, [sp, #28]
	bl ObjectMotion_ResetAndSetPositionInMode2
	ldr r1, [sp, #28]
	ldr r4, [sp, #32]
	lsls r2, r1, #16
	movs r1, #1
	lsls r0, r4, #16
	movs r3, #1
	negs r1, r1
	bl Motion_CamBounds
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_02003400
	ldr r0, [r5]
	movs r1, #28
	bl Object_SetModeById
	mov r2, r10
	mov r3, r8
	lsls r0, r2, #16
	ldr r2, [r3, #12]
	movs r4, #128
	lsls r4, r4, #12
	adds r2, r2, r4
	ldr r1, [r3, #8]
	lsrs r0, r0, #16
	ldr r3, [r3, #16]
	bl Engine_ObjectCreate
	mov r10, r0
	mov r2, r10
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	mov r5, r10
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r1, .L_02008d18
	mov r0, r10
	bl Func_020032e8
	mov r0, r10
	movs r1, #1
	bl Animation_SetStateFlags
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02003460
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_02003458
	movs r0, #60
	bl Func_02003468
	bl Func_02003338
	mov r0, r9
	ldrb r3, [r0]
	mov r1, r9
	strh r3, [r1]
	movs r2, #128
	ldr r3, .L_02008d10
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r2, [sp, #32]
	ldr r3, [sp, #28]
	asrs r2, r2, #4
	asrs r3, r3, #4
	str r2, [sp, #12]
	adds r1, r3, #0
	adds r2, #63
	movs r0, #3
	movs r4, #5
	str r3, [sp, #8]
	str r0, [sp, #0]
	subs r3, #3
	adds r1, #23
	adds r0, r2, #0
	str r4, [sp, #4]
	bl Func_02003328
	mov r4, r9
	ldrh r3, [r4]
	movs r7, #0
	b .L_02008d1c
	.2byte 0x0000
.L_02008d10:
	.4byte 0x00001000
.L_02008d14:
	.4byte gPartyState
.L_02008d18:
	.4byte Data_020035cc
.L_02008d1c:
	cmp r3, #0
	beq .L_02008d44
.L_02008d20:
	ldr r2, .L_02008d40
	movs r1, #128
	adds r3, r7, #0
	lsls r1, r1, #19
	orrs r3, r2
	adds r1, #82
	strh r3, [r1]
	movs r0, #8
	mov r5, r9
	bl WaitFrames
	ldrh r3, [r5]
	adds r7, #1
	cmp r7, r3
	bne .L_02008d20
	b .L_02008d44
.L_02008d40:
	.4byte 0x00001000
.L_02008d44:
	bl Func_02003330
	movs r0, #246
	bl Func_020034b8
	movs r0, #48
	bl Battle_WaitMode0
	mov r3, r11
	adds r3, #255
	lsls r3, r3, #24
	mov r0, r11
	lsrs r5, r3, #24
	movs r7, #0
	cmp r0, #0
	beq .L_02008e18
.L_02008d64:
	bl Random16Far
	add r3, sp, #84
	ands r0, r5
	ldrb r2, [r3, r0]
	add r1, sp, #64
	strb r2, [r1, r7]
	adds r7, #1
	ldrb r2, [r3, r5]
	strb r2, [r3, r0]
	adds r3, r5, #0
	adds r3, #255
	lsls r3, r3, #24
	lsrs r5, r3, #24
	cmp r7, r11
	bne .L_02008d64
	mov r1, r11
	movs r7, #0
	cmp r1, #0
	beq .L_02008e18
.L_02008d8c:
	bl Random16Far
	mov r4, r8
	ldr r3, [r4, #8]
	movs r2, #31
	ands r2, r0
	ldr r5, .L_02008e94
	lsls r2, r2, #16
	adds r3, r3, r2
	add r6, sp, #52
	adds r3, r3, r5
	str r3, [r6]
	bl Random16Far
	mov r1, r8
	ldr r3, [r1, #12]
	movs r5, #15
	ands r0, r5
	lsls r0, r0, #16
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r0
	adds r3, r3, r2
	str r3, [r6, #4]
	bl Random16Far
	mov r4, r8
	ldr r3, [r4, #16]
	ands r0, r5
	ldr r5, .L_02008e98
	lsls r0, r0, #16
	adds r3, r3, r0
	adds r3, r3, r5
	str r3, [r6, #8]
	add r3, sp, #64
	ldrb r0, [r3, r7]
	ldr r4, .L_02008e9c
	lsls r0, r0, #3
	ldr r2, [r4, r0]
	mov r5, r8
	ldr r3, [r5, #8]
	lsls r2, r2, #16
	add r1, sp, #40
	adds r3, r3, r2
	str r3, [r1]
	adds r0, #4
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	ldr r2, [r4, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	ldr r2, [sp, #16]
	bl Func_0200088c
	movs r3, #1
	ands r3, r7
	cmp r3, #0
	beq .L_02008e0c
	movs r0, #195
	bl Func_020034b8
.L_02008e0c:
	movs r0, #8
	adds r7, #1
	bl Battle_WaitMode0
	cmp r7, r11
	bne .L_02008d8c
.L_02008e18:
	mov r0, r11
	cmp r0, #18
	beq .L_02008eea
	movs r0, #96
	bl Battle_WaitMode0
	ldr r1, .L_02008ea0
	mov r0, r10
	bl Func_020032e8
	bl Func_02003338
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	ldrh r2, [r3]
	mov r5, sp
	movs r3, #255
	ands r3, r2
	adds r5, #106
	movs r0, #128
	strh r3, [r5]
	lsls r0, r0, #9
	movs r1, #1
	bl Func_02003458
	movs r0, #60
	bl Func_02003468
	ldrh r7, [r5]
	cmp r7, #0
	beq .L_02008e72
.L_02008e58:
	ldr r2, .L_02008e90
	movs r1, #128
	adds r3, r7, #0
	lsls r1, r1, #19
	adds r1, #82
	orrs r3, r2
	strh r3, [r1]
	movs r0, #8
	subs r7, #1
	bl WaitFrames
	cmp r7, #0
	bne .L_02008e58
.L_02008e72:
	mov r1, r11
	movs r7, #0
	cmp r1, #0
	beq .L_02008ec8
.L_02008e7a:
	add r3, sp, #64
	ldrb r1, [r3, r7]
	ldr r0, .L_02008e9c
	lsls r1, r1, #3
	ldr r3, [r0, r1]
	mov r4, r8
	ldr r2, [r4, #8]
	adds r1, #4
	ldr r1, [r0, r1]
	b .L_02008ea4
	.2byte 0x0000
.L_02008e90:
	.4byte 0x00001000
.L_02008e94:
	.4byte 0xfff00000
.L_02008e98:
	.4byte 0xfff80000
.L_02008e9c:
	.4byte Data_020036fc
.L_02008ea0:
	.4byte Data_02003620
.L_02008ea4:
	lsls r3, r3, #16
	adds r2, r2, r3
	ldr r3, [r4, #16]
	lsls r1, r1, #16
	adds r3, r3, r1
	asrs r2, r2, #20
	movs r1, #1
	str r1, [sp, #0]
	str r1, [sp, #4]
	asrs r3, r3, #20
	adds r2, #64
	movs r0, #64
	movs r1, #0
	adds r7, #1
	bl Func_02003328
	cmp r7, r11
	bne .L_02008e7a
.L_02008ec8:
	ldr r2, [sp, #12]
	ldr r3, [sp, #8]
	movs r1, #3
	movs r0, #5
	str r1, [sp, #0]
	str r0, [sp, #4]
	adds r2, #63
	subs r3, #3
	movs r0, #64
	movs r1, #0
	bl Func_02003328
	bl Func_02003330
	bl Event_WaitTransitionFrames
	b .L_0200904c
.L_02008eea:
	ldr r5, [sp, #36]
	movs r0, #170
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r2, #160
	lsls r2, r2, #4
	adds r2, #85
	adds r0, r0, r2
	bl GameFlag_SetBit
	ldr r1, .L_02009088
	mov r0, r10
	bl Func_020032e8
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl Func_02003420
	movs r0, #134
	movs r1, #1
	movs r2, #240
	negs r1, r1
	lsls r2, r2, #15
	movs r3, #1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02003430
	movs r0, #220
	bl Func_020034b8
	ldr r3, [sp, #24]
	ldr r4, [sp, #20]
	lsls r5, r3, #16
	lsls r6, r4, #16
	movs r3, #128
	ldr r2, [sp, #16]
	lsls r3, r3, #4
	adds r1, r6, #0
	adds r0, r5, #0
	bl Func_020009b8
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #144
	ldr r2, [sp, #16]
	lsls r3, r3, #4
	adds r1, r6, #0
	adds r0, r5, #0
	bl Func_020009b8
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #160
	ldr r2, [sp, #16]
	lsls r3, r3, #4
	adds r1, r6, #0
	adds r0, r5, #0
	bl Func_020009b8
	movs r0, #12
	bl Battle_WaitMode0
	movs r0, #16
	bl WaitFrames
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02003458
	movs r0, #1
	bl Func_02003468
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02003458
	movs r0, #32
	bl Func_02003468
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #62
	bl Func_020034b8
	ldr r0, [sp, #24]
	ldr r1, [sp, #20]
	ldr r2, [sp, #16]
	bl Func_02000a78
	movs r0, #80
	bl Battle_WaitMode0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #135
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200904c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #136
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200904c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #137
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200904c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #138
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200904c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #139
	bl GameFlag_SetBit
	movs r0, #188
	bl Func_020034b8
	movs r5, #1
	movs r1, #7
	movs r2, #33
	movs r3, #7
	movs r6, #2
	movs r0, #52
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003328
	movs r0, #5
	bl WaitFrames
	movs r0, #54
	movs r1, #7
	movs r2, #33
	movs r3, #7
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003328
	movs r0, #134
	movs r1, #136
	lsls r0, r0, #18
	lsls r1, r1, #16
	movs r2, #0
	movs r3, #0
	bl Func_02003380
	movs r0, #60
	bl WaitFrames
.L_0200904c:
	mov r0, r10
	bl Func_020032f8
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	ldr r3, .L_0200908c
	movs r5, #133
	lsls r5, r5, #2
	adds r3, r3, r5
	ldr r0, [r3]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02003430
	movs r0, #60
	bl Battle_WaitMode0
	bl Func_020033a0
.L_02009078:
	add sp, #108
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009088:
	.4byte Data_02003620
.L_0200908c:
	.4byte gPartyState
	.section .text.x02009160,"ax",%progbits
	.global Func_02001160
	.thumb_func
Func_02001160:
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
	ldr r5, [r3]
	sub sp, #8
	bl Func_02003398
	movs r0, #0
	bl Func_02003488
	bl Func_020034a0
	ldr r3, [r5, #8]
	movs r2, #240
	asrs r7, r3, #19
	ldr r3, [r5, #16]
	lsls r2, r2, #1
	asrs r3, r3, #19
	mov r8, r3
	ldr r3, .L_02009388
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200938c
	cmp r2, r3
	bne .L_020091c6
	movs r1, #144
	movs r2, #220
	movs r0, #2
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #232
	bl Func_02003378
	cmp r7, #8
	beq .L_020091c6
	movs r1, #144
	movs r2, #212
	movs r0, #2
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #232
	bl Func_02003378
.L_020091c6:
	ldr r3, .L_02009388
	movs r2, #240
	lsls r2, r2, #1
	adds r5, r3, r2
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, .L_02009390
	cmp r2, r3
	bne .L_02009218
	movs r1, #140
	movs r2, #172
	movs r0, #2
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r3, #0
	bl Func_02003378
	movs r1, #140
	movs r2, #188
	movs r0, #2
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r3, #232
	bl Func_02003378
	movs r1, #148
	movs r2, #180
	movs r0, #2
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r3, #0
	bl Func_02003378
	movs r1, #130
	movs r2, #158
	movs r0, #2
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r3, #232
	bl Func_02003378
.L_02009218:
	movs r1, #0
	ldrsh r2, [r5, r1]
	ldr r3, .L_02009394
	cmp r2, r3
	bne .L_02009256
	cmp r7, #37
	bne .L_0200923c
	mov r2, r8
	cmp r2, #31
	bne .L_0200923c
	movs r0, #168
	lsls r0, r0, #4
	bl GameFlag_SetBit
	movs r0, #8
	movs r1, #2
	bl ObjectMotion_SetActionVariant
.L_0200923c:
	cmp r7, #55
	beq .L_02009242
	b .L_02009356
.L_02009242:
	mov r3, r8
	cmp r3, #95
	beq .L_0200924a
	b .L_02009356
.L_0200924a:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #129
	bl GameFlag_SetBit
	b .L_02009356
.L_02009256:
	ldr r3, .L_0200938c
	cmp r2, r3
	bne .L_02009356
	cmp r7, #7
	bne .L_02009270
	mov r1, r8
	cmp r1, #53
	bne .L_02009270
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #130
	bl GameFlag_SetBit
.L_02009270:
	cmp r7, #67
	bne .L_020092f2
	mov r2, r8
	cmp r2, #45
	bne .L_020092f2
	movs r3, #22
	movs r5, #1
	movs r0, #50
	movs r1, #22
	movs r2, #33
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003328
	movs r1, #0
	movs r2, #0
	movs r0, #14
	bl Func_020033e0
	movs r0, #146
	lsls r0, r0, #2
	bl Func_020034b8
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #188
	bl Func_020034b8
	movs r6, #2
	movs r1, #20
	movs r2, #31
	movs r3, #20
	movs r0, #50
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003328
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #20
	movs r2, #31
	movs r3, #20
	movs r0, #51
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003328
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #248
	movs r1, #168
	lsls r0, r0, #17
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #0
	bl Func_02003380
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #131
	bl GameFlag_SetBit
.L_020092f2:
	cmp r7, #93
	bne .L_02009306
	mov r3, r8
	cmp r3, #57
	bne .L_02009306
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #132
	bl GameFlag_SetBit
.L_02009306:
	cmp r7, #91
	bne .L_02009356
	mov r1, r8
	cmp r1, #59
	bne .L_02009356
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #132
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200934c
	movs r0, #12
	bl Object_GetById
	movs r2, #10
	ldrsh r5, [r0, r2]
	movs r0, #12
	bl Object_GetById
	movs r3, #18
	ldrsh r2, [r0, r3]
	lsls r5, r5, #16
	lsls r2, r2, #16
	movs r0, #13
	adds r1, r5, #0
	bl Func_020033e0
	movs r1, #182
	movs r2, #236
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020033e0
.L_0200934c:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #133
	bl GameFlag_SetBit
.L_02009356:
	ldr r3, .L_02009388
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009390
	cmp r2, r3
	bne .L_0200937c
	cmp r7, #33
	bne .L_0200937c
	mov r2, r8
	cmp r2, #45
	bne .L_0200937c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #134
	bl GameFlag_SetBit
.L_0200937c:
	bl Func_020033a0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009388:
	.4byte gPartyState
.L_0200938c:
	.4byte 0x00000131
.L_02009390:
	.4byte 0x00000132
.L_02009394:
	.4byte 0x00000130
	.section .text.x02009398,"ax",%progbits
	.global Func_02001398
	.thumb_func
Func_02001398:
	push {r5, r6, r7, lr}
	ldr r5, .L_0200950c
	movs r3, #192
	movs r1, #133
	lsls r1, r1, #2
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	adds r3, r5, r1
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009510
	adds r6, r0, #0
	ldr r1, [r6, #8]
	ldr r0, [r6, #16]
	cmp r2, r3
	bne .L_020093dc
	asrs r3, r0, #20
	asrs r2, r1, #20
	movs r1, #1
	str r1, [sp, #0]
	str r1, [sp, #4]
	adds r3, #64
	movs r0, #17
	movs r1, #71
	bl Func_02003328
	b .L_020093f0
.L_020093dc:
	asrs r3, r0, #20
	asrs r2, r1, #20
	movs r1, #1
	str r1, [sp, #0]
	str r1, [sp, #4]
	adds r3, #64
	movs r0, #54
	movs r1, #87
	bl Func_02003328
.L_020093f0:
	movs r0, #106
	bl Func_020034b8
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	asrs r1, r1, #20
	asrs r3, r3, #20
	movs r2, #128
	lsls r2, r2, #12
	lsls r1, r1, #20
	lsls r3, r3, #20
	movs r0, #94
	adds r1, r1, r2
	adds r3, r3, r2
	adds r0, #255
	movs r2, #0
	bl Engine_ObjectCreate
	adds r5, r0, #0
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	adds r3, r5, #0
	adds r3, #85
	movs r1, #0
	strb r1, [r3]
	adds r0, r5, #0
	movs r1, #4
	bl Object_SetPartAttribute
	movs r3, #176
	lsls r3, r3, #9
	str r3, [r5, #28]
	str r3, [r5, #24]
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetSpritePriority
	adds r0, r5, #0
	movs r1, #3
	bl Func_020032e0
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r1, .L_02009514
	adds r0, r5, #0
	bl Func_020032e8
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r1, #0
	ldrsh r0, [r3, r1]
	adds r2, #122
	adds r0, r0, r2
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	movs r6, #1
	adds r5, r0, #0
.L_02009476:
	movs r3, #128
	lsls r3, r3, #2
	adds r0, r6, r3
	bl GameFlag_Test
	adds r6, #1
	ands r5, r0
	cmp r6, #7
	bne .L_02009476
	cmp r5, #0
	beq .L_02009506
	bl Func_02003398
	movs r0, #0
	bl Func_02003488
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r1, #153
	adds r0, #204
	bl Func_02003420
	ldr r3, .L_0200950c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009510
	movs r5, #9
	cmp r2, r3
	bne .L_020094c2
	movs r5, #8
.L_020094c2:
	movs r1, #1
	adds r0, r5, #0
	bl Func_02003438
	bl Func_02003430
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #70
	bl Func_020034b8
	movs r1, #2
	adds r0, r5, #0
	bl Object_SetModeById
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	adds r0, r5, #0
	bl Object_GetById
	movs r3, #4
	adds r0, #85
	strb r3, [r0]
	movs r0, #80
	bl Battle_WaitMode0
	bl Func_020033a0
.L_02009506:
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200950c:
	.4byte gPartyState
.L_02009510:
	.4byte 0x00000132
.L_02009514:
	.4byte Data_02003574
	.section .text.x02009518,"ax",%progbits
	.global Func_02001518
	.thumb_func
Func_02001518:
	push {r5, r6, r7, lr}
	ldr r5, .L_0200960c
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02003398
	movs r0, #0
	bl Func_02003488
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009610
	cmp r2, r3
	bne .L_02009554
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #13
	bl Func_02003450
	b .L_02009562
.L_02009554:
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #11
	bl Func_02003450
.L_02009562:
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	ldr r5, .L_0200960c
	movs r3, #133
	lsls r3, r3, #2
	asrs r1, r1, #20
	asrs r2, r2, #20
	adds r5, r5, r3
	lsls r1, r1, #4
	lsls r2, r2, #4
	ldr r0, [r5]
	adds r1, #8
	adds r2, #8
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #10
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl Motion_CamBounds
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #48]
	movs r3, #128
	lsls r3, r3, #4
	str r3, [r6, #52]
	movs r0, #198
	bl Func_020034b8
	ldr r2, [r6, #12]
	movs r3, #128
	lsls r3, r3, #11
	adds r2, r2, r3
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	adds r0, r6, #0
	bl Func_02003310
	adds r0, r6, #0
	bl Func_02003318
	adds r0, r7, #0
	movs r1, #1
	bl Object_SetSpritePriority
	adds r0, r6, #0
	movs r1, #1
	bl Object_SetSpritePriority
	ldr r2, [r6, #12]
	movs r3, #128
	lsls r3, r3, #16
	adds r2, r2, r3
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	adds r0, r6, #0
	bl Func_02003310
	movs r0, #80
	bl Battle_WaitMode0
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_020033a0
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200960c:
	.4byte gPartyState
.L_02009610:
	.4byte 0x00000132
	.section .text.x020097b4,"ax",%progbits
	.balign 4
	.global Func_020017b4
	.thumb_func
Func_020017b4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_02009928
	adds r6, r0, #0
	movs r0, #133
	lsls r0, r0, #2
	adds r5, r5, r0
	ldr r0, [r5]
	sub sp, #20
	bl Object_GetById
	adds r7, r0, #0
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #18
	ldrsh r3, [r0, r1]
	cmp r3, #248
	ble .L_02009800
	movs r0, #128
	movs r3, #0
	lsls r0, r0, #2
	str r3, [r6, #108]
	bl GameFlag_ClearBit
	ldr r3, [r6, #8]
	ldr r1, .L_0200992c
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	str r3, [r6, #8]
	ldr r3, [r6, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [r6, #16]
	b .L_02009920
.L_02009800:
	add r2, sp, #8
	ldr r3, [r7, #8]
	mov r8, r2
	ldr r2, [r6, #76]
	ldr r1, [r6, #8]
	subs r3, r3, r2
	subs r3, r1, r3
	mov r5, r8
	str r3, [r5]
	str r3, [sp, #0]
	ldr r3, [r7, #12]
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	ldr r0, [r6, #52]
	ldr r4, [r6, #16]
	subs r3, r3, r0
	adds r3, r4, r3
	str r3, [r5, #8]
	str r3, [sp, #4]
	ldr r3, [r7, #8]
	cmp r2, r3
	bne .L_02009832
	ldr r3, [r7, #16]
	cmp r0, r3
	beq .L_020098b2
.L_02009832:
	movs r0, #168
	adds r3, r4, #0
	ldr r2, [r6, #12]
	lsls r0, r0, #2
	bl Engine_ObjectCreate
	adds r5, r0, #0
	bl Random16Far
	movs r3, #15
	ldr r2, [r5, #8]
	ands r3, r0
	subs r3, #8
	lsls r3, r3, #16
	adds r2, r2, r3
	str r2, [r5, #8]
	bl Random16Far
	movs r3, #31
	ldr r2, [r5, #12]
	ands r3, r0
	subs r3, #4
	lsls r3, r3, #16
	adds r2, r2, r3
	str r2, [r5, #12]
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r3, r6, #0
	adds r3, #98
	ldrb r1, [r3]
	adds r0, r5, #0
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetSpritePriority
	adds r0, r5, #0
	movs r1, #1
	bl Func_020032e0
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r1, .L_02009930
	adds r0, r5, #0
	bl Func_020032e8
	ldr r3, [r7, #8]
	movs r2, #3
	str r3, [r6, #76]
	ldr r3, [r7, #16]
	str r3, [r6, #52]
	ldr r3, .L_02009934
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_020098b2
	movs r0, #106
	bl Func_020034b8
.L_020098b2:
	mov r1, r8
	movs r0, #10
	ldrsh r3, [r1, r0]
	cmp r3, #248
	bgt .L_02009920
	ldrh r3, [r7, #6]
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #224
	subs r1, r1, r3
	lsls r0, r0, #11
	mov r2, r8
	adds r5, r6, #0
	bl Vector_AddPolarOffsetFar
	adds r5, #34
	mov r2, r8
	ldr r1, [r2]
	ldrb r0, [r5]
	ldr r2, [r2, #8]
	bl Func_02003348
	cmp r0, #255
	beq .L_02009920
	ldr r3, [sp, #0]
	ldr r2, [sp, #4]
	str r3, [r6, #8]
	str r2, [r6, #16]
	adds r1, r3, #0
	ldrb r0, [r5]
	bl Func_02003348
	cmp r0, #52
	bne .L_02009908
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r5, #181
	lsls r5, r5, #1
	adds r1, r3, r5
	movs r2, #0
	movs r3, #201
	b .L_0200991c
.L_02009908:
	cmp r0, #51
	bne .L_02009920
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #181
	lsls r0, r0, #1
	adds r1, r3, r0
	movs r2, #0
	movs r3, #200
.L_0200991c:
	strh r3, [r1]
	str r2, [r6, #108]
.L_02009920:
	add sp, #20
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009928:
	.4byte gPartyState
.L_0200992c:
	.4byte 0xfff00000
.L_02009930:
	.4byte Data_02003594
.L_02009934:
	.4byte gFrameCount
	.section .text.x020099a8,"ax",%progbits
	.global Func_020019a8
	.thumb_func
Func_020019a8:
	push {r5, r6, r7, lr}
	ldr r3, .L_020099c8
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	sub sp, #12
	cmp r3, #1
	bne .L_020099c0
	movs r0, #8
	b .L_020099ce
.L_020099c0:
	cmp r3, #3
	bne .L_020099cc
	movs r0, #9
	b .L_020099ce
.L_020099c8:
	.4byte gPartyState
.L_020099cc:
	movs r0, #10
.L_020099ce:
	bl Object_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	ldr r1, .L_02009a7c
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	mov r5, sp
	adds r3, r3, r2
	str r3, [r5]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	str r3, [r7, #48]
	bl Func_02003398
	movs r0, #0
	bl Func_02003488
	ldr r1, [r5]
	ldr r3, [r5, #8]
	adds r0, r7, #0
	movs r2, #0
	bl Func_02003310
	ldr r2, [r5, #8]
	movs r3, #236
	lsls r3, r3, #14
	adds r2, r2, r3
	movs r3, #143
	ldr r0, [r7, #8]
	lsls r3, r3, #1
	movs r1, #0
	bl OverlayObject_PrepareObject
	movs r3, #192
	adds r6, r0, #0
	lsls r3, r3, #9
	str r3, [r6, #24]
	str r3, [r6, #28]
	ldr r1, [r6, #80]
	movs r3, #13
	ldrb r2, [r1, #5]
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r1, #5]
	movs r2, #128
	ldr r3, .L_02009a74
	lsls r2, r2, #19
	adds r2, #74
	strh r3, [r2]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_02009a78
	movs r0, #204
	orrs r3, r2
	strh r3, [r1]
	bl Func_020034b8
	adds r2, r7, #0
	movs r3, #3
	adds r2, #85
	strb r3, [r2]
	movs r5, #0
	adds r0, r7, #0
	bl OverlayObject_WaitUntilIdle
	movs r1, #0
	movs r2, #0
	str r5, [r7, #8]
	str r5, [r7, #12]
	str r5, [r7, #16]
	movs r0, #8
	b .L_02009a80
	.2byte 0x0000
.L_02009a74:
	.4byte 0x00002f3f
.L_02009a78:
	.4byte 0x00008000
.L_02009a7c:
	.4byte 0xfff00000
.L_02009a80:
	bl Func_020033e0
	adds r0, r6, #0
	bl Func_020032f8
	bl Func_020033a0
	add sp, #12
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02009a94,"ax",%progbits
	.global Func_02001a94
	.thumb_func
Func_02001a94:
	push {r5, r6, r7, lr}
	ldr r3, .L_02009b80
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	sub sp, #20
	cmp r3, #1
	bne .L_02009abe
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_SetBit
	movs r7, #11
	b .L_02009aec
.L_02009abe:
	cmp r3, #3
	bne .L_02009ad8
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	movs r7, #26
	b .L_02009aec
.L_02009ad8:
	movs r0, #10
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_SetBit
	movs r7, #53
.L_02009aec:
	bl Func_02003398
	movs r0, #0
	bl Func_02003488
	ldr r3, [r6, #8]
	ldr r1, .L_02009b84
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	add r5, sp, #8
	adds r3, r3, r2
	str r3, [r5]
	ldr r3, [r6, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #52]
	str r3, [r6, #48]
	bl Func_02003398
	movs r0, #0
	bl Func_02003488
	ldr r1, [r5]
	ldr r3, [r5, #8]
	movs r2, #0
	adds r0, r6, #0
	bl Func_02003310
	adds r0, r6, #0
	bl Func_02003318
	movs r0, #188
	bl Func_020034b8
	movs r5, #1
	adds r2, r7, #0
	movs r6, #2
	movs r1, #4
	movs r3, #4
	movs r0, #18
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003328
	movs r0, #5
	bl WaitFrames
	adds r2, r7, #0
	movs r1, #7
	movs r3, #4
	movs r0, #18
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003328
	movs r0, #5
	bl WaitFrames
	movs r1, #176
	lsls r0, r7, #20
	lsls r1, r1, #15
	movs r2, #0
	movs r3, #0
	bl Func_02003380
	bl Func_020033a0
	add sp, #20
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009b80:
	.4byte gPartyState
.L_02009b84:
	.4byte 0xfff00000
	.section .text.x02009c94,"ax",%progbits
	.global Func_02001c94
	.thumb_func
Func_02001c94:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Func_02003398
	movs r0, #0
	bl Func_02003488
	movs r5, #8
.L_02009ca8:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02009cba
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02009cba:
	adds r5, #1
	cmp r5, #63
	bls .L_02009ca8
	movs r0, #158
	bl Func_020034b8
	ldr r3, .L_02009d58
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009d5c
	cmp r2, r3
	bne .L_02009ce4
	ldr r0, .L_02009d60
	movs r1, #33
	movs r2, #7
	bl Func_02003320
	b .L_02009d0a
.L_02009ce4:
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r6, r2
	ldrh r3, [r3]
	ldr r4, .L_02009d64
	lsls r3, r3, #16
	asrs r0, r3, #16
	lsrs r3, r3, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #1
	lsls r0, r0, #3
	adds r3, r0, #4
	ldrh r1, [r4, r3]
	adds r3, r3, r4
	ldrh r2, [r3, #2]
	ldr r0, [r4, r0]
	bl Func_02003320
.L_02009d0a:
	ldr r5, .L_02009d58
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r2, #4
	negs r2, r2
	ldr r0, [r5]
	movs r1, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #6
	bl Battle_WaitMode0
	movs r1, #170
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_02003450
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_020033a0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009d58:
	.4byte gPartyState
.L_02009d5c:
	.4byte 0x0000012f
.L_02009d60:
	.4byte Data_020043e4
.L_02009d64:
	.4byte Data_02004410
	.section .text.x02009e64,"ax",%progbits
	.global Func_02001e64
	.thumb_func
Func_02001e64:
	push {r5, r6, r7, lr}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r0, #214
	lsls r0, r0, #1
	movs r7, #129
	adds r3, r3, r0
	lsls r7, r7, #2
	ldr r6, .L_0200a194
	str r7, [r3]
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r0, #0
	ldrsh r1, [r3, r0]
	ldr r3, .L_0200a198
	sub sp, #8
	cmp r1, r3
	bne .L_02009f86
	ldr r0, .L_0200a19c
	ldr r1, .L_0200a1a0
	ldr r2, .L_0200a1a4
	ldr r3, .L_0200a1a8
	bl Func_02000038
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #135
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009ec6
	movs r0, #130
	lsls r0, r0, #2
	movs r1, #48
	movs r2, #0
	bl Func_02000a78
	movs r3, #9
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #85
	movs r1, #34
	movs r2, #85
	movs r3, #8
	bl Func_02003328
.L_02009ec6:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #136
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009ef4
	movs r0, #138
	lsls r0, r0, #2
	movs r1, #48
	movs r2, #3
	bl Func_02000a78
	movs r3, #9
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #101
	movs r1, #34
	movs r2, #101
	movs r3, #8
	bl Func_02003328
.L_02009ef4:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #137
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009f22
	movs r0, #252
	lsls r0, r0, #1
	movs r1, #40
	movs r2, #10
	bl Func_02000a78
	movs r3, #9
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #85
	movs r1, #46
	movs r2, #85
	movs r3, #20
	bl Func_02003328
.L_02009f22:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #138
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009f50
	movs r0, #142
	lsls r0, r0, #2
	movs r1, #40
	movs r2, #2
	bl Func_02000a78
	movs r3, #9
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #101
	movs r1, #46
	movs r2, #101
	movs r3, #20
	bl Func_02003328
.L_02009f50:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #139
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009f60
	b .L_0200a6e6
.L_02009f60:
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #54
	movs r1, #7
	movs r2, #33
	movs r3, #7
	bl Func_02003328
	movs r0, #134
	movs r1, #136
	lsls r0, r0, #18
	lsls r1, r1, #16
	movs r2, #0
	movs r3, #0
	bl Func_02003380
	b .L_0200a6e6
.L_02009f86:
	ldr r3, .L_0200a1ac
	cmp r1, r3
	beq .L_02009f8e
	b .L_0200a1b4
.L_02009f8e:
	movs r0, #8
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #168
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009fb8
	movs r1, #148
	movs r2, #248
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_020033e0
	movs r0, #8
	movs r1, #2
	bl ObjectMotion_SetActionVariant
.L_02009fb8:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #129
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009fd4
	movs r1, #220
	movs r2, #190
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_020033e0
.L_02009fd4:
	ldr r0, .L_0200a1b0
	bl Func_02003490
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r6, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	movs r2, #241
	lsls r2, r2, #1
	strb r3, [r0]
	adds r3, r6, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #17
	bne .L_0200a03c
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a03c
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	movs r1, #182
	movs r2, #240
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #15
	bl Func_020033e0
.L_0200a03c:
	ldr r3, .L_0200a194
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #18
	bne .L_0200a05c
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a05c
	bl Func_02001d68
.L_0200a05c:
	ldr r5, .L_0200a194
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #10
	beq .L_0200a06e
	b .L_0200a6e6
.L_0200a06e:
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a084
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_020033e0
.L_0200a084:
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a09e
	movs r0, #9
	bl Object_GetById
	movs r3, #4
	adds r0, #85
	strb r3, [r0]
.L_0200a09e:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a0bc
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #87
	movs r2, #44
	movs r3, #86
	bl Func_02003328
.L_0200a0bc:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a0dc
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #87
	movs r2, #46
	movs r3, #86
	bl Func_02003328
.L_0200a0dc:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a0fc
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #87
	movs r2, #43
	movs r3, #87
	bl Func_02003328
.L_0200a0fc:
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a11c
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #87
	movs r2, #45
	movs r3, #87
	bl Func_02003328
.L_0200a11c:
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a13a
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #87
	movs r2, #47
	movs r3, #87
	bl Func_02003328
.L_0200a13a:
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a15a
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #87
	movs r2, #44
	movs r3, #88
	bl Func_02003328
.L_0200a15a:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a17a
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #87
	movs r2, #46
	movs r3, #88
	bl Func_02003328
.L_0200a17a:
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	b .L_0200a6e6
	.2byte 0x0000
.L_0200a194:
	.4byte gPartyState
.L_0200a198:
	.4byte 0x0000012f
.L_0200a19c:
	.4byte Data_0200384c
.L_0200a1a0:
	.4byte Data_0200385c
.L_0200a1a4:
	.4byte Data_02003888
.L_0200a1a8:
	.4byte Data_020038b4
.L_0200a1ac:
	.4byte 0x00000130
.L_0200a1b0:
	.4byte Data_020036e4
.L_0200a1b4:
	ldr r3, .L_0200a518
	cmp r1, r3
	beq .L_0200a1bc
	b .L_0200a2ce
.L_0200a1bc:
	movs r0, #14
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #85
	movs r7, #2
	strb r7, [r3]
	ldr r3, .L_0200a51c
	movs r5, #1
	str r3, [r0, #20]
	str r3, [r0, #12]
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r6, r0
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #130
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a218
	movs r1, #224
	movs r2, #212
	movs r0, #8
	lsls r1, r1, #14
	lsls r2, r2, #17
	bl Func_020033e0
.L_0200a218:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #131
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a254
	movs r1, #134
	movs r2, #180
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020033e0
	movs r0, #51
	movs r1, #20
	movs r2, #31
	movs r3, #20
	str r5, [sp, #0]
	str r7, [sp, #4]
	bl Func_02003328
	movs r0, #248
	movs r1, #168
	lsls r0, r0, #17
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #0
	bl Func_02003380
.L_0200a254:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #132
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a270
	movs r1, #186
	movs r2, #228
	movs r0, #13
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020033e0
.L_0200a270:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #133
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a2c6
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a2b8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #132
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a2b8
	movs r0, #12
	bl Object_GetById
	movs r1, #10
	ldrsh r5, [r0, r1]
	movs r0, #12
	bl Object_GetById
	movs r3, #18
	ldrsh r2, [r0, r3]
	lsls r5, r5, #16
	lsls r2, r2, #16
	movs r0, #13
	adds r1, r5, #0
	bl Func_020033e0
.L_0200a2b8:
	movs r1, #182
	movs r2, #236
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020033e0
.L_0200a2c6:
	ldr r0, .L_0200a520
	bl Func_02003490
	b .L_0200a6e6
.L_0200a2ce:
	ldr r3, .L_0200a524
	cmp r1, r3
	beq .L_0200a2d6
	b .L_0200a52c
.L_0200a2d6:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #134
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a2f2
	movs r1, #132
	movs r2, #180
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_020033e0
.L_0200a2f2:
	ldr r0, .L_0200a528
	bl Func_02003490
	movs r0, #241
	lsls r0, r0, #1
	adds r2, r6, r0
	ldrh r3, [r2]
	movs r1, #0
	ldrsh r2, [r2, r1]
	cmp r2, #1
	beq .L_0200a30a
	b .L_0200a4a2
.L_0200a30a:
	movs r3, #68
	movs r1, #160
	movs r2, #224
	str r3, [sp, #0]
	lsls r1, r1, #2
	lsls r2, r2, #2
	movs r3, #18
	movs r0, #33
	bl Func_02002a74
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200a35e
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #8
	bl Object_GetById
	adds r0, #85
	strb r5, [r0]
	movs r0, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	b .L_0200a3a8
.L_0200a35e:
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl Func_020033e0
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	adds r0, r7, #0
	bl GameFlag_SetBit
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_SetBit
.L_0200a3a8:
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a3c2
	movs r0, #8
	bl Object_GetById
	movs r3, #4
	adds r0, #85
	strb r3, [r0]
.L_0200a3c2:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a3e0
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #71
	movs r2, #8
	movs r3, #69
	bl Func_02003328
.L_0200a3e0:
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a400
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #71
	movs r2, #6
	movs r3, #71
	bl Func_02003328
.L_0200a400:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a420
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #71
	movs r2, #5
	movs r3, #75
	bl Func_02003328
.L_0200a420:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a440
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #71
	movs r2, #8
	movs r3, #75
	bl Func_02003328
.L_0200a440:
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a460
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #71
	movs r2, #11
	movs r3, #75
	bl Func_02003328
.L_0200a460:
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a47e
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #71
	movs r2, #8
	movs r3, #73
	bl Func_02003328
.L_0200a47e:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a48e
	b .L_0200a6e6
.L_0200a48e:
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #71
	movs r2, #10
	movs r3, #71
	bl Func_02003328
	b .L_0200a6e6
.L_0200a4a2:
	cmp r2, #11
	bne .L_0200a4c0
	movs r3, #68
	movs r1, #160
	movs r2, #224
	lsls r1, r1, #2
	lsls r2, r2, #2
	str r3, [sp, #0]
	movs r0, #33
	movs r3, #18
	bl Func_02002a74
	movs r0, #22
	movs r1, #0
	b .L_0200a4ec
.L_0200a4c0:
	cmp r2, #12
	bne .L_0200a4f4
	movs r3, #68
	movs r1, #160
	movs r2, #224
	str r3, [sp, #0]
	lsls r1, r1, #2
	lsls r2, r2, #2
	movs r0, #35
	movs r3, #18
	bl Func_02002a74
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a4e6
	b .L_0200a6e6
.L_0200a4e6:
	movs r1, #36
	negs r1, r1
	movs r0, #36
.L_0200a4ec:
	adds r2, r7, #0
	bl Func_02003190
	b .L_0200a6e6
.L_0200a4f4:
	subs r3, #4
	movs r0, #128
	lsls r3, r3, #16
	lsls r0, r0, #9
	cmp r3, r0
	bls .L_0200a502
	b .L_0200a6e6
.L_0200a502:
	movs r3, #68
	movs r1, #160
	movs r2, #224
	str r3, [sp, #0]
	lsls r1, r1, #2
	lsls r2, r2, #2
	movs r0, #35
	movs r3, #18
	bl Func_02002a74
	b .L_0200a6e6
.L_0200a518:
	.4byte 0x00000131
.L_0200a51c:
	.4byte 0xfff20000
.L_0200a520:
	.4byte Data_020036ea
.L_0200a524:
	.4byte 0x00000132
.L_0200a528:
	.4byte Data_020036f6
.L_0200a52c:
	ldr r3, .L_0200a6ec
	cmp r1, r3
	beq .L_0200a534
	b .L_0200a6e6
.L_0200a534:
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r6, r1
	ldr r5, [r2, #32]
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #11
	bls .L_0200a548
	b .L_0200a6e6
.L_0200a548:
	ldr r2, .L_0200a6f0
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0200a550:
	.4byte .L_0200a606
	.4byte .L_0200a606
	.4byte .L_0200a63a
	.4byte .L_0200a63a
	.4byte .L_0200a686
	.4byte .L_0200a686
	.4byte .L_0200a580
	.4byte .L_0200a6e6
	.4byte .L_0200a6e6
	.4byte .L_0200a6e6
	.4byte .L_0200a6e6
	.4byte .L_0200a596
.L_0200a580:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a596
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_020033e0
.L_0200a596:
	ldr r6, .L_0200a6f4
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r6, r0
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #11
	bl Object_SetModeById
	movs r0, #11
	bl Object_GetById
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #11
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #11
	bl Object_SetModeById
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a5fa
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #12
	bne .L_0200a5fa
	bl Func_02001df4
	b .L_0200a6e6
.L_0200a5fa:
	movs r0, #11
	bl Object_GetById
	ldr r3, .L_0200a6f8
	str r3, [r0, #12]
	b .L_0200a6e6
.L_0200a606:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a61e
	movs r1, #240
	movs r2, #240
	movs r0, #8
	lsls r1, r1, #15
	b .L_0200a650
.L_0200a61e:
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #18
	movs r1, #1
	movs r2, #11
	movs r3, #4
	bl Func_02003328
	movs r0, #184
	movs r1, #176
	lsls r0, r0, #16
	b .L_0200a67a
.L_0200a63a:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a660
	movs r1, #138
	movs r2, #240
	movs r0, #9
	lsls r1, r1, #18
.L_0200a650:
	lsls r2, r2, #15
	bl Func_020033e0
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_0200a6e6
.L_0200a660:
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #18
	movs r1, #1
	movs r2, #26
	movs r3, #4
	bl Func_02003328
	movs r0, #212
	movs r1, #176
	lsls r0, r0, #17
.L_0200a67a:
	lsls r1, r1, #15
	movs r2, #0
	movs r3, #2
	bl Func_02003380
	b .L_0200a6e6
.L_0200a686:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a6ac
	movs r1, #198
	movs r2, #240
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #15
	bl Func_020033e0
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_0200a6d0
.L_0200a6ac:
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #18
	movs r1, #1
	movs r2, #53
	movs r3, #4
	bl Func_02003328
	movs r0, #214
	movs r1, #176
	lsls r0, r0, #18
	lsls r1, r1, #15
	movs r2, #0
	movs r3, #2
	bl Func_02003380
.L_0200a6d0:
	adds r2, r5, #0
	adds r2, #236
	ldr r3, [r2]
	movs r1, #128
	lsls r1, r1, #12
	adds r3, r3, r1
	str r3, [r2]
	adds r2, #8
	ldr r3, [r2]
	adds r3, r3, r1
	str r3, [r2]
.L_0200a6e6:
	movs r0, #0
	add sp, #8
	pop {r5, r6, r7, pc}
.L_0200a6ec:
	.4byte 0x00000133
.L_0200a6f0:
	.4byte .L_0200a550
.L_0200a6f4:
	.4byte gPartyState
.L_0200a6f8:
	.4byte 0xfff00000
	.section .text.x0200a746,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200a780,"ax",%progbits
	.balign 4
	.global Func_02002780
	.thumb_func
Func_02002780:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_0200a938
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
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r8
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_0200a7c8
	cmp r7, #0
	beq .L_0200a7c8
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_0200a7d0
.L_0200a7c8:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_0200a7d0:
	mov r3, r10
	bl Engine_ObjectCreate
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200a7de
	b .L_0200a92a
.L_0200a7de:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_020032e0
	ldr r2, .L_0200a93c
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_020032e8
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r3, .L_0200a940
	mov r1, r9
	str r3, [r6, #108]
	mov r3, r11
	str r3, [r6, #68]
	ldr r3, [sp, #36]
	adds r0, r6, #0
	str r3, [r6, #72]
	ldr r3, [sp, #40]
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Object_SetSpritePriorityB
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_0200a944
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200a92a
	cmp r7, #0
	beq .L_0200a92a
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200a860
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_0200a860:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200a880
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriorityB
.L_0200a880:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_0200a894
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_0200a894:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200a8da
	ldr r3, .L_0200a93c
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_0200a8c2
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200a8d4
.L_0200a8c2:
	ldr r2, .L_0200a944
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200a944
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200a8d4:
	bl __divsi3
	str r0, [r6, #52]
.L_0200a8da:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200a8f6
	adds r0, r6, #0
	movs r1, #1
	bl Func_020032e0
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_020032e8
.L_0200a8f6:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200a908
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_0200a908:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200a91a
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200a91a:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200a92a
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200a92a:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a938:
	.4byte gPartyState
.L_0200a93c:
	.4byte Data_02004428
.L_0200a940:
	.4byte Effect_MoveLocal
.L_0200a944:
	.4byte 0xffff0000
	.section .text.x0200a948,"ax",%progbits
	.global Func_02002948
	.thumb_func
Func_02002948:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r4, .L_0200aa60
	movs r1, #1
	movs r0, #12
	ldrsh r3, [r4, r0]
	negs r1, r1
	sub sp, #4
	cmp r3, r1
	beq .L_0200aa54
	lsls r3, r3, #3
	adds r3, r3, r4
	adds r3, #32
	mov r8, r3
	ldr r3, .L_0200aa64
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	str r4, [sp, #0]
	bl Object_GetById
	mov r1, r8
	ldr r3, [r0, #8]
	movs r5, #0
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	ldr r4, [sp, #0]
	cmp r3, r2
	bne .L_0200a994
	ldr r3, [r0, #16]
	movs r5, #2
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	cmp r3, r2
	beq .L_0200a99c
.L_0200a994:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r4, #12]
.L_0200a99c:
	movs r0, #12
	ldrsh r3, [r4, r0]
	movs r2, #1
	negs r2, r2
	ldr r1, .L_0200aa68
	cmp r3, r2
	beq .L_0200aa54
	movs r5, #14
	ldrsh r3, [r4, r5]
	cmp r3, #0
	beq .L_0200aa54
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	str r4, [sp, #0]
	adds r3, r2, #0
	adds r3, #228
	ldr r0, [r3]
	ldr r5, [r3, #4]
	ldr r3, [r2]
	ands r0, r1
	ands r5, r1
	ldr r6, [r3, #4]
	movs r1, #16
	ldrsh r3, [r4, r1]
	ldr r2, .L_0200aa6c
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r3, r3, #5
	mov r10, r3
	mov r3, r8
	movs r2, #0
	ldrsh r1, [r3, r2]
	lsls r1, r1, #20
	subs r7, r1, r0
	movs r0, #2
	ldrsh r2, [r3, r0]
	movs r0, #0
	lsls r2, r2, #20
	bl Map_GetTerrainHeight
	mov r2, r8
	movs r1, #2
	ldrsh r3, [r2, r1]
	subs r0, r0, r6
	lsls r3, r3, #20
	subs r3, r3, r5
	subs r3, r3, r6
	subs r2, r3, r0
	asrs r7, r7, #16
	adds r0, r0, r3
	asrs r0, r0, #16
	adds r3, r7, #0
	movs r5, #167
	asrs r2, r2, #16
	adds r1, r0, #0
	adds r3, #15
	lsls r5, r5, #1
	adds r2, #14
	adds r1, #58
	ldr r4, [sp, #0]
	cmp r3, r5
	bhi .L_0200aa54
	movs r0, #15
	negs r0, r0
	cmp r2, r0
	blt .L_0200aa54
	cmp r2, #239
	bgt .L_0200aa54
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r7, r3
	movs r3, #255
	ands r2, r3
	movs r3, #0
	str r3, [r4, #20]
	lsls r3, r7, #16
	orrs r2, r3
	ldr r3, .L_0200aa70
	adds r0, r4, #0
	orrs r2, r3
	movs r3, #128
	str r2, [r4, #24]
	lsls r3, r3, #3
	mov r2, r10
	orrs r2, r3
	str r2, [r4, #28]
	adds r0, #20
	bl Func_020032b0
.L_0200aa54:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200aa60:
	.4byte Data_02004458
.L_0200aa64:
	.4byte gPartyState
.L_0200aa68:
	.4byte 0xffff0000
.L_0200aa6c:
	.4byte ResourceTableEntries
.L_0200aa70:
	.4byte 0x80008800
	.section .text.x0200aa74,"ax",%progbits
	.global Func_02002a74
	.thumb_func
Func_02002a74:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_0200aca4
	str r1, [sp, #40]
	mov r8, r0
	movs r1, #32
	add r1, r8
	mov r9, r1
	mov r12, r9
	adds r5, r2, #0
	mov r2, r12
	adds r6, r3, #0
	str r2, [sp, #8]
	ldr r3, .L_0200aca8
	movs r1, #4
	ldr r7, [sp, #80]
	mov lr, r3
	.2byte 0xf800
	add r0, sp, #44
	ldrh r0, [r0]
	mov r1, r8
	strh r0, [r1, #4]
	add r1, sp, #40
	ldrh r1, [r1]
	mov r3, r8
	strh r1, [r3]
	strh r5, [r3, #2]
	movs r3, #255
	lsls r3, r3, #8
	mov r5, r8
	mov r0, r8
	adds r3, #255
	mov r1, r8
	strh r6, [r5, #6]
	movs r2, #0
	strh r7, [r0, #8]
	strh r3, [r1, #12]
	mov r3, r8
	strh r2, [r3, #10]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	mov r12, r3
	lsls r2, r2, #1
	mov r1, r12
	add r2, r12
	adds r1, #236
	ldr r0, [r1]
	ldr r3, [r2, #8]
	ldr r5, [r2, #48]
	adds r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #32]
	adds r1, #4
	ldr r3, [r2, #12]
	ldr r2, [r1]
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [sp, #28]
	mov r3, r12
	adds r3, #244
	ldr r3, [r3]
	subs r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #24]
	mov r3, r12
	adds r3, #248
	ldr r3, [r3]
	asrs r0, r0, #20
	subs r3, r3, r2
	asrs r2, r2, #20
	lsls r2, r2, #7
	adds r2, r2, r0
	lsls r2, r2, #2
	asrs r3, r3, #20
	adds r5, r5, r2
	movs r0, #0
	str r3, [sp, #20]
	str r5, [sp, #36]
	str r0, [sp, #12]
	cmp r0, r3
	bge .L_0200abf8
.L_0200ab28:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_0200abec
.L_0200ab3c:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_0200abdc
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_0200abdc
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_0200abdc
	ldr r2, [sp, #16]
	ldr r3, [sp, #32]
	mov r0, r9
	adds r7, r2, r3
	strh r7, [r0]
	ldr r1, [sp, #12]
	ldr r2, [sp, #28]
	add r0, sp, #40
	ldrh r0, [r0]
	adds r6, r1, r2
	mov r3, r9
	mov r1, r9
	strh r6, [r3, #2]
	strh r0, [r1, #4]
	ldr r1, [sp, #40]
	movs r0, #10
	adds r1, #1
	adds r0, #255
	str r1, [sp, #40]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ab90
	cmp r5, r10
	bne .L_0200abce
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_0200abce
.L_0200ab90:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200abce
	mov r2, r8
	ldrh r4, [r2, #6]
	ldrh r5, [r2, #8]
	movs r3, #8
	ldrsh r1, [r2, r3]
	movs r3, #6
	ldrsh r0, [r2, r3]
	movs r2, #64
	adds r3, r2, #0
	ands r3, r4
	ands r2, r5
	lsls r3, r3, #16
	lsls r2, r2, #16
	asrs r3, r3, #16
	asrs r2, r2, #16
	orrs r7, r3
	orrs r6, r2
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl Func_02003350
.L_0200abce:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_0200abdc:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_0200ab3c
.L_0200abec:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_0200ab28
.L_0200abf8:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ac50
	ldr r3, .L_0200acac
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #0
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	mov r0, r8
	asrs r1, r3, #20
	ldr r3, [sp, #8]
	mov r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	cmp r2, r3
	bge .L_0200ac50
.L_0200ac2a:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_0200ac40
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_0200ac40
	mov r0, r8
	strh r2, [r0, #12]
.L_0200ac40:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_0200ac2a
.L_0200ac50:
	movs r0, #128
	lsls r0, r0, #1
	bl MemAlloc
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_0200ac5e:
	ldr r3, .L_0200acb0
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_0200ac5e
	bl Resource_FindFreeEntry
	mov r1, r8
	strh r0, [r1, #16]
	lsls r0, r0, #16
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #1
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r5, #0
	bl MemFree
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200acb4
	bl Scheduler_AddOrUpdateCallback
	mov r3, r8
	movs r2, #10
	ldrsh r0, [r3, r2]
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200aca4:
	.4byte Data_02004458
.L_0200aca8:
	.4byte IwramClearWords
.L_0200acac:
	.4byte gPartyState
.L_0200acb0:
	.4byte 0x11111111
.L_0200acb4:
	.4byte Func_02002948
	.section .text.x0200acb8,"ax",%progbits
	.global Func_02002cb8
	.thumb_func
Func_02002cb8:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r1, .L_0200ad38
	movs r2, #133
	mov r8, r1
	lsls r2, r2, #2
	add r8, r2
	mov r3, r8
	ldr r0, [r3]
	bl Object_GetById
	mov r1, r8
	ldr r5, [r0, #8]
	ldr r6, [r0, #16]
	mov r10, r0
	movs r2, #128
	ldr r0, [r1]
	movs r1, #128
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	asrs r5, r5, #20
	mov r2, r8
	asrs r6, r6, #20
	ldr r0, [r2]
	lsls r1, r5, #4
	lsls r2, r6, #4
	adds r1, #8
	adds r2, #8
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #1
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #12
	lsls r5, r5, #20
	lsls r6, r6, #20
	adds r5, r5, r3
	mov r1, r10
	adds r6, r6, r3
	ldr r2, [r1, #12]
	adds r3, r6, #0
	adds r1, r5, #0
	mov r0, r10
	bl Object_SetPositionAndResetMotion
	movs r0, #4
	bl Battle_WaitMode0
	bl Func_02003440
	ldr r2, .L_0200ad3c
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200ad38:
	.4byte gPartyState
.L_0200ad3c:
	.4byte 0xfff80000
	.section .text.x0200ad40,"ax",%progbits
	.global Func_02002d40
	.thumb_func
Func_02002d40:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, .L_0200adac
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	mov r8, r0
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	mov r2, r8
	ldrh r1, [r2, #6]
	movs r2, #64
	ldr r6, [r0, #8]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	asrs r6, r6, #20
	orrs r6, r3
	ldrh r3, [r1, #8]
	ldr r5, [r0, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_02002cb8
	movs r0, #161
	bl Func_020034b8
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003350
	movs r0, #12
	bl Battle_WaitMode0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200adac:
	.4byte gPartyState
	.section .text.x0200adb0,"ax",%progbits
	.global Func_02002db0
	.thumb_func
Func_02002db0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_0200ae60
	movs r2, #133
	lsls r2, r2, #2
	adds r1, r1, r2
	mov r8, r0
	ldr r0, [r1]
	sub sp, #8
	mov r10, r1
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	movs r2, #64
	asrs r7, r3, #20
	mov r3, r8
	ldrh r1, [r3, #6]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	orrs r7, r3
	ldrh r3, [r1, #8]
	ldr r5, [r6, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_02002cb8
	movs r0, #229
	bl Func_020034b8
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #2
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003350
	movs r0, #12
	bl Battle_WaitMode0
	movs r3, #128
	ldr r2, .L_0200ae58
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r2, #226
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #255
	adds r3, #74
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200ae5c
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_0200ae64
	.2byte 0x0000
.L_0200ae58:
	.4byte 0x00000000
.L_0200ae5c:
	.4byte 0x00008000
.L_0200ae60:
	.4byte gPartyState
.L_0200ae64:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
.L_0200ae78:
	cmp r7, #5
	bne .L_0200ae82
	movs r0, #204
	bl Func_020034b8
.L_0200ae82:
	ldr r3, [r6, #24]
	ldr r1, .L_0200aee0
	ldr r2, .L_0200aee4
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_0200aee8
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_0200ae78
	ldr r3, .L_0200aeec
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
	mov r1, r8
	strh r3, [r1, #14]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200aee0:
	.4byte 0xfffffc00
.L_0200aee4:
	.4byte 0xfffffd00
.L_0200aee8:
	.4byte 0xffff6667
.L_0200aeec:
	.4byte gPartyState
	.section .text.x0200af46,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200af48,"ax",%progbits
	.global Func_02002f48
	.thumb_func
Func_02002f48:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200b0cc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02003398
	movs r0, #0
	bl Func_02003488
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02003308
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
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #204
	bl Func_020034b8
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200b0d0
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200afe2:
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
	ldr r3, .L_0200b0d4
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200b0d8
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
	ldr r4, .L_0200b0dc
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_02002780
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200afe2
	movs r0, #188
	bl Func_020034b8
	ldr r5, .L_0200b0cc
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02003410
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02003368
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02003368
	bl Func_02003370
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02003410
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	bl Func_020033a0
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200b0cc:
	.4byte gPartyState
.L_0200b0d0:
	.4byte Effect_AdvanceMotion
.L_0200b0d4:
	.4byte 0xffffa000
.L_0200b0d8:
	.4byte 0xffffd000
.L_0200b0dc:
	.4byte 0x01090001
	.section .text.x0200b0e0,"ax",%progbits
	.global Func_020030e0
	.thumb_func
Func_020030e0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200b188
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200b18c
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r0, #16]
	adds r5, r6, #0
	asrs r3, r3, #20
	mov r10, r3
	movs r1, #10
	ldrsh r3, [r6, r1]
	movs r7, #0
	adds r5, #32
	ldrh r2, [r6, #10]
	cmp r7, r3
	bge .L_0200b17c
.L_0200b114:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200b170
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200b170
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b144
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_02002d40
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_0200b17c
.L_0200b144:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_0200b17c
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_02002db0
	movs r2, #2
	ldrsh r0, [r6, r2]
	mov r1, r8
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r6, r3]
	mov r1, r10
	adds r0, #8
	bl GameFlag_SetByte
	movs r0, #1
	b .L_0200b17e
.L_0200b170:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_0200b114
.L_0200b17c:
	movs r0, #0
.L_0200b17e:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b188:
	.4byte gPartyState
.L_0200b18c:
	.4byte Data_02004458
	.section .text.x0200b190,"ax",%progbits
	.global Func_02003190
	.thumb_func
Func_02003190:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r3, .L_0200b240
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r9, r0
	ldr r0, [r3]
	mov r11, r1
	bl Object_GetById
	movs r3, #192
	ldr r5, .L_0200b244
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r6, r0, #0
	movs r2, #2
	ldrsh r0, [r5, r2]
	mov r10, r3
	bl GameFlag_GetByte
	adds r7, r0, #0
	movs r3, #2
	ldrsh r0, [r5, r3]
	adds r0, #8
	bl GameFlag_GetByte
	mov r8, r0
	cmp r7, #0
	bne .L_0200b1de
	cmp r0, #0
	beq .L_0200b232
.L_0200b1de:
	movs r2, #2
	ldrsh r0, [r5, r2]
	movs r1, #0
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r5, r3]
	movs r1, #0
	adds r0, #8
	bl GameFlag_SetByte
	mov r3, r9
	adds r2, r7, r3
	mov r3, r8
	movs r1, #128
	add r3, r11
	lsls r1, r1, #12
	lsls r3, r3, #20
	adds r3, r3, r1
	str r3, [r6, #16]
	movs r3, #230
	lsls r3, r3, #1
	add r3, r10
	lsls r2, r2, #20
	adds r2, r2, r1
	ldr r1, [r3]
	str r2, [r6, #8]
	str r2, [r1, #8]
	ldr r3, [r6, #16]
	str r3, [r1, #16]
	bl Func_02003308
	bl Func_02002f48
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #0]
	str r2, [r3]
.L_0200b232:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200b240:
	.4byte gPartyState
.L_0200b244:
	.4byte Data_02004458
	.section .bss,"aw",%nobits
	.global Data_02004434
Data_02004434:
	.space 0x00000004
	.global Data_02004438
Data_02004438:
	.space 0x00000004
	.global Data_0200443c
Data_0200443c:
	.space 0x00000004
	.global Data_02004440
Data_02004440:
	.space 0x00000004
	.global Data_02004444
Data_02004444:
	.space 0x00000004
	.global Data_02004448
Data_02004448:
	.space 0x00000004
	.global Data_0200444c
Data_0200444c:
	.space 0x00000004
	.global Data_02004450
Data_02004450:
	.space 0x00000004
	.global Data_02004454
Data_02004454:
	.space 0x00000004
	.global Data_02004458
Data_02004458:
