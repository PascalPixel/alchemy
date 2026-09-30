.syntax unified
	.thumb
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	bl Func_02001f60
	cmp r0, #0
	beq .L_02008064
	movs r0, #13
	bl Func_02002270
.L_02008064:
	pop {pc}
	.2byte 0x0000
	.section .text.x02008068,"ax",%progbits
	.global Func_02000068
	.thumb_func
Func_02000068:
	push {r5, lr}
	sub sp, #32
	add r5, sp, #8
	adds r0, r5, #0
	bl Func_02000c44
	cmp r0, #0
	beq .L_0200808c
	mov r2, sp
	add r3, sp, #24
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r5]
	ldr r1, [r5, #4]
	ldr r2, [r5, #8]
	ldr r3, [r5, #12]
	bl Func_02000ec8
.L_0200808c:
	add sp, #32
	pop {r5, pc}
	.global Data_02000090
Data_02000090:
	.4byte 0x00004770
	.section .text.x02008094,"ax",%progbits
	.global Func_02000094
	.thumb_func
Func_02000094:
	push {r5, r6, lr}
	adds r0, r1, #0
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #6
	bne .L_02008104
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #14
	bne .L_02008104
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #24
	bl GameFlag_SetBit
	movs r0, #10
	bl WaitFrames
	movs r0, #134
	bl Func_020022c8
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	movs r6, #0
	b .L_020080dc
.L_020080cc:
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #29
	bgt .L_020080e6
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
.L_020080dc:
	cmp r2, r3
	bgt .L_020080cc
	ldr r3, [r5, #40]
	cmp r3, #0
	bne .L_020080cc
.L_020080e6:
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r2, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	bl Func_020021e0
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #0
	movs r3, #128
	bl Func_020022c0
.L_02008104:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008110,"ax",%progbits
	.global Func_02000110
	.thumb_func
Func_02000110:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	movs r6, #129
	adds r3, r3, r2
	lsls r6, r6, #2
	str r6, [r3]
	movs r0, #8
	sub sp, #4
	bl Object_GetById
	movs r5, #192
	lsls r5, r5, #8
	str r5, [r0, #24]
	movs r0, #8
	bl Object_GetById
	movs r1, #3
	str r5, [r0, #28]
	movs r0, #12
	bl ObjectMotion_SetActionVariant
	movs r1, #3
	movs r0, #14
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	bl Func_02002248
	movs r0, #12
	bl Func_02002248
	movs r0, #13
	bl Func_02002248
	movs r0, #14
	bl Func_02002248
	movs r0, #15
	bl Func_02002248
	movs r0, #11
	bl Func_02000af0
	movs r0, #12
	bl Func_02000af0
	movs r0, #13
	bl Func_02000af0
	movs r0, #14
	bl Func_02000af0
	movs r0, #15
	bl Func_02000af0
	movs r3, #1
	movs r1, #160
	movs r2, #224
	lsls r2, r2, #2
	str r3, [sp, #0]
	lsls r1, r1, #2
	movs r3, #0
	movs r0, #33
	bl Func_020018f4
	ldr r3, .L_020081d0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #13
	bne .L_020081b6
	movs r0, #19
	movs r1, #0
	adds r2, r6, #0
	bl Func_02002010
	b .L_020081c8
.L_020081b6:
	cmp r3, #14
	bne .L_020081c8
	movs r0, #20
	movs r1, #22
	negs r0, r0
	negs r1, r1
	adds r2, r6, #0
	bl Func_02002010
.L_020081c8:
	movs r0, #0
	add sp, #4
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020081d0:
	.4byte gPartyState
	.section .text.x020081d4,"ax",%progbits
	.global Func_020001d4
	.thumb_func
Func_020001d4:
	push {r5, lr}
	movs r0, #0
	bl Func_02000244
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020081f2
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #24
	bl GameFlag_ClearBit
.L_020081f2:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #24
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200823c
	movs r0, #10
	bl Object_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, .L_02008240
	movs r1, #208
	movs r2, #232
	str r3, [r5, #20]
	str r3, [r5, #12]
	movs r0, #10
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Func_02002228
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #0
	movs r3, #0
	bl Func_020021e0
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #0
	movs r3, #128
	bl Func_020022c0
.L_0200823c:
	movs r0, #0
	pop {r5, pc}
.L_02008240:
	.4byte 0xffe00000
	.section .text.x02008244,"ax",%progbits
	.global Func_02000244
	.thumb_func
Func_02000244:
	push {lr}
	movs r0, #20
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #37
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200826c
	movs r0, #98
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
.L_0200826c:
	pop {pc}
	.2byte 0x0000
	.section .text.x02008270,"ax",%progbits
	.global Func_02000270
	.thumb_func
Func_02000270:
	ldr r3, .L_02008278
	str r0, [r3]
	bx lr
	.2byte 0x0000
.L_02008278:
	.4byte Data_02002a88
	.section .text.x0200827c,"ax",%progbits
	.global Func_0200027c
	.thumb_func
Func_0200027c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008328
	sub sp, #32
	ldr r0, [r3]
	cmp r0, #0
	bge .L_0200828e
	adds r0, #3
.L_0200828e:
	asrs r0, r0, #2
	movs r1, #5
	bl Engine_MathRemainder
	ldr r3, .L_0200832c
	mov r8, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_020082e2
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r2, r0
	ldrh r1, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_020082c2
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r1
	cmp r3, #153
	bne .L_0200831e
.L_020082c2:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0200831e
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200831e
.L_020082e2:
	movs r5, #0
	movs r6, #4
.L_020082e6:
	mov r2, r8
	adds r0, r2, r5
	movs r1, #5
	mov r7, sp
	bl Engine_MathRemainder
	ldr r3, .L_02008330
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r5, #1
	strh r3, [r7, r0]
	adds r6, #2
	cmp r5, #4
	ble .L_020082e6
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	ldr r1, .L_02008334
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02008328
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200831e:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008328:
	.4byte Data_02002a84
.L_0200832c:
	.4byte Data_02002a88
.L_02008330:
	.4byte gOverlayArea + 0x2ab0
.L_02008334:
	.4byte 0x05000184
	.section .text.x02008338,"ax",%progbits
	.global Func_02000338
	.thumb_func
Func_02000338:
	push {r5, r6, lr}
	ldr r2, .L_02008394
	movs r3, #1
	adds r6, r0, #0
	str r3, [r2]
	cmp r6, #2
	beq .L_0200835c
	ldr r1, .L_02008398
	movs r2, #32
	ldr r0, .L_0200839c
	ldr r5, .L_020083a0
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_020083a4
	ldr r1, .L_020083a8
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
.L_0200835c:
	movs r0, #160
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200836c
	cmp r6, #1
	bne .L_0200837e
.L_0200836c:
	ldr r3, .L_020083ac
	movs r2, #0
	movs r1, #144
	str r2, [r3]
	ldr r0, .L_020083b0
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	b .L_02008392
.L_0200837e:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_020083b4
	ldr r1, .L_020083b8
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_02008392:
	pop {r5, r6, pc}
.L_02008394:
	.4byte Data_02002a88
.L_02008398:
	.4byte 0x05000180
.L_0200839c:
	.4byte gOverlayArea + 0x2ab0
.L_020083a0:
	.4byte IwramCopyWords
.L_020083a4:
	.4byte gOverlayArea + 0x2ad0
.L_020083a8:
	.4byte 0x050001a0
.L_020083ac:
	.4byte Data_02002a84
.L_020083b0:
	.4byte Func_0200027c
.L_020083b4:
	.4byte gOverlayArea + 0x2ad4
.L_020083b8:
	.4byte 0x05000184
	.section .text.x020083bc,"ax",%progbits
	.global Func_020003bc
	.thumb_func
Func_020003bc:
	push {r5, lr}
	bl Object_GetById
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #85
	movs r3, #4
	strb r3, [r1]
	movs r2, #0
	ldr r3, [r5, #20]
	str r2, [r5, #68]
	movs r2, #128
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r5, #12]
	subs r1, #50
	ldrb r2, [r1]
	movs r3, #128
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r3]
	bl Map_GetTerrainHeight
	adds r3, r0, #0
	asrs r3, r3, #19
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	adds r3, #6
	movs r2, #0
	bl Func_020021e0
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #0
	movs r3, #128
	bl Func_020022c0
	pop {r5, pc}
	.section .text.x02008410,"ax",%progbits
	.global Func_02000410
	.thumb_func
Func_02000410:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_0200851c
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r7, [r6, #104]
	bl Func_020021f0
	movs r0, #0
	bl Func_02002290
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	movs r5, #0
	strh r5, [r3]
	movs r3, #85
	adds r3, r3, r6
	mov r9, r3
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #99
	adds r3, r3, r7
	mov r8, r3
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_020084b0
.L_0200846a:
	ldr r3, [r7, #8]
	ldr r2, .L_02008520
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	cmp r5, r2
	bgt .L_02008486
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r5, r5, r3
.L_02008486:
	ldr r3, .L_02008524
	adds r1, r6, #0
	ldr r2, [r3]
	ldrb r3, [r3]
	adds r1, #35
	lsls r3, r3, #12
	strh r3, [r6, #6]
	movs r3, #1
	ands r2, r3
	movs r3, #2
	lsls r3, r2
	ldrb r2, [r1]
	movs r0, #1
	eors r3, r2
	strb r3, [r1]
	bl WaitFrames
	mov r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_0200846a
.L_020084b0:
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #68
	movs r2, #1
	add r3, r10
	strh r2, [r3]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	strh r2, [r3]
	ldr r3, [r7, #8]
	ldrh r1, [r7, #6]
	subs r2, #3
	asrs r3, r3, #19
	ands r3, r2
	asrs r1, r1, #13
	adds r3, r3, r1
	subs r3, #1
	lsls r3, r3, #19
	str r3, [r6, #8]
	ldr r3, [r7, #16]
	ldr r0, .L_02008518
	asrs r3, r3, #19
	ands r3, r2
	movs r2, #2
	ands r1, r2
	subs r3, r3, r1
	adds r3, #1
	lsls r3, r3, #19
	str r3, [r6, #16]
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r6, #40]
	adds r3, r6, #0
	adds r3, #35
	strb r0, [r3]
	mov r2, r9
	movs r3, #3
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	b .L_02008528
.L_02008518:
	.4byte 0x00000001
.L_0200851c:
	.4byte gPartyState
.L_02008520:
	.4byte 0x0003ffff
.L_02008524:
	.4byte Data_0300122c
.L_02008528:
	bl Motion_CamBounds
	bl Func_02002268
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	ldr r5, .L_02008570
	lsls r3, r3, #9
	str r3, [r7, #52]
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	movs r0, #0
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r6, #16]
	adds r2, r0, #0
	ldr r1, [r6, #8]
	adds r0, r7, #0
	bl Func_02002190
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	movs r5, #0
	cmp r2, r3
	ble .L_0200858e
	b .L_02008574
	.2byte 0x0000
.L_02008570:
	.4byte 0x00000000
.L_02008574:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_0200858e
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	cmp r2, r3
	bgt .L_02008574
.L_0200858e:
	movs r0, #127
	bl Func_020022c8
	ldr r3, [r6, #40]
	movs r5, #0
	cmp r3, #0
	beq .L_020085ae
.L_0200859c:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_020085ae
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_0200859c
.L_020085ae:
	adds r0, r7, #0
	bl Func_02002198
	ldr r5, .L_020085f8
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
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #70
	add r2, r10
	movs r3, #1
	strh r3, [r2]
	movs r3, #170
	lsls r3, r3, #1
	movs r6, #0
	add r3, r10
	strh r6, [r3]
	bl Func_020021f8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_020085f8:
	.4byte gPartyState
	.section .text.x020085fc,"ax",%progbits
	.global Func_020005fc
	.thumb_func
Func_020005fc:
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
	ldr r3, .L_02008624
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_02008624:
	.4byte IwramFillWords + 0x74
	.section .text.x02008628,"ax",%progbits
	.global Func_02000628
	.thumb_func
Func_02000628:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_02008690
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	movs r3, #179
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r3, #0
	bne .L_02008686
	movs r3, #173
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008686
	movs r3, #175
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008686
	movs r3, #180
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02008694
.L_02008686:
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildren
	b .L_020087d2
.L_02008690:
	.4byte gPartyState
.L_02008694:
	adds r0, r6, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildren
	adds r3, r6, #0
	adds r3, #100
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #31
	ands r3, r2
	cmp r3, #31
	bne .L_020086b4
	movs r0, #231
	bl Func_020022c8
.L_020086b4:
	ldr r3, [r7, #80]
	ldr r0, [r6, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r0, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r0, #9]
	movs r2, #2
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	bl Func_020022b8
	cmp r0, #255
	beq .L_020087b6
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_02002298
	ldr r5, [r5]
	movs r3, #136
	lsls r3, r3, #17
	cmp r5, r3
	bgt .L_020087b6
	ldr r2, .L_020087a4
	cmp r5, r2
	blt .L_020087b6
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0200877c
	ldr r2, [r7, #12]
	ldr r3, [r6, #12]
	subs r5, r2, r3
	cmp r5, #0
	bge .L_02008714
	subs r5, r3, r2
.L_02008714:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	adds r0, #8
	adds r1, #8
	mov r8, r2
	bl Func_020005fc
	cmp r0, #12
	bgt .L_02008734
	movs r3, #192
	lsls r3, r3, #12
	cmp r5, r3
	bge .L_02008734
	movs r2, #1
	mov r8, r2
.L_02008734:
	mov r3, r8
	cmp r3, #0
	beq .L_0200877c
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200877c
	ldrh r3, [r6, #6]
	str r6, [r7, #104]
	strh r3, [r7, #6]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #181
	lsls r2, r2, #1
	strb r3, [r1]
	add r2, r10
	movs r3, #200
	strh r3, [r2]
	ldr r3, .L_020087a8
	movs r2, #128
	ldr r0, .L_020087a0
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	strb r0, [r3]
	mov r2, r9
	movs r3, #1
	strb r3, [r2]
	adds r2, r6, #0
	adds r2, #99
	strb r3, [r2]
.L_0200877c:
	ldrh r0, [r6, #6]
	bl Math_Cosine
	ldr r1, [r6, #48]
	ldr r5, .L_020087ac
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r3, r3, r0
	ldrh r0, [r6, #6]
	str r3, [r6, #8]
	bl Math_Sine
	ldr r1, [r6, #48]
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #16]
	b .L_020087b0
.L_020087a0:
	.4byte 0x00000000
.L_020087a4:
	.4byte 0xffe00000
.L_020087a8:
	.4byte gPartyState
.L_020087ac:
	.4byte IwramMulQ16
.L_020087b0:
	adds r3, r3, r0
	str r3, [r6, #16]
	b .L_020087d2
.L_020087b6:
	adds r3, r6, #0
	adds r3, #99
	movs r5, #0
	strb r5, [r3]
	ldr r1, .L_020087e0
	adds r0, r6, #0
	str r5, [r6, #108]
	bl Func_02002170
	movs r0, #228
	bl Func_020022c8
	ldr r3, .L_020087e4
	str r5, [r3]
.L_020087d2:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020087e0:
	.4byte Data_02002a8c
.L_020087e4:
	.4byte gOverlayArea + 0x2aac
	.section .text.x020087e8,"ax",%progbits
	.global Func_020007e8
	.thumb_func
Func_020007e8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #222
	sub sp, #68
	bl Func_020022c8
	ldrh r0, [r5, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r6, .L_0200889c
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #8]
	add r2, sp, #56
	adds r3, r3, r0
	str r3, [r2]
	mov r8, r2
	ldrh r0, [r5, #6]
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #16]
	mov r2, r8
	adds r3, r3, r0
	str r3, [r2, #8]
	movs r0, #140
	ldr r1, [r2]
	lsls r0, r0, #1
	ldr r2, [r5, #12]
	bl Func_02002178
	movs r1, #2
	adds r7, r0, #0
	bl Func_02002160
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r7, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	ldr r2, .L_02008898
	ldrh r3, [r5, #6]
	add r4, sp, #16
	strh r3, [r7, #6]
	adds r3, r7, #0
	adds r3, #100
	strh r6, [r3]
	subs r3, #2
	strb r2, [r3]
	adds r3, #1
	strb r2, [r3]
	ldr r3, .L_020088a0
	str r3, [r7, #108]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #1
	str r3, [r4]
	movs r3, #7
	str r3, [r4, #4]
	mov r3, r8
	ldr r0, [r3]
	ldr r2, [r3, #8]
	ldr r3, .L_020088a4
	ldr r1, [r5, #12]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #8]
	b .L_020088a8
.L_02008898:
	.4byte 0x00000000
.L_0200889c:
	.4byte IwramMulQ16
.L_020088a0:
	.4byte Func_02000628
.L_020088a4:
	.4byte 0xfffa0000
.L_020088a8:
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	str r4, [sp, #12]
	bl Func_02001600
	adds r0, r7, #0
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020088c0,"ax",%progbits
	.global Func_020008c0
	.thumb_func
Func_020008c0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200895c
	sub sp, #56
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_0200894e
	add r6, sp, #16
	movs r3, #3
	str r3, [r6]
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r6, #8]
	str r3, [r6, #12]
	movs r3, #14
	str r3, [r6, #4]
	bl Random16Far
	mov r2, r10
	lsls r3, r0, #3
	ldr r2, [r2, #8]
	adds r3, r3, r0
	lsrs r3, r3, #16
	subs r3, #4
	lsls r3, r3, #16
	mov r8, r2
	add r8, r3
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	lsrs r3, r3, #16
	movs r2, #32
	subs r2, r2, r3
	mov r3, r10
	ldr r5, [r3, #12]
	lsls r2, r2, #16
	adds r5, r5, r2
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsrs r0, r0, #16
	movs r2, #160
	lsls r2, r2, #11
	lsls r0, r0, #16
	adds r0, r0, r2
	movs r1, #10
	bl Engine_MathDivide
	mov r3, r10
	ldr r2, [r3, #16]
	movs r3, #176
	lsls r3, r3, #12
	str r0, [sp, #0]
	str r3, [sp, #8]
	mov r0, r8
	adds r1, r5, #0
	movs r3, #0
	str r7, [sp, #4]
	str r6, [sp, #12]
	bl Func_02001600
.L_0200894e:
	movs r0, #0
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200895c:
	.4byte Data_0300122c
	.section .text.x02008960,"ax",%progbits
	.global Func_02000960
	.thumb_func
Func_02000960:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #23
	bl Object_GetById
	adds r5, r0, #0
	ldr r2, [r5, #80]
	movs r1, #128
	mov r8, r2
	movs r2, #248
	movs r0, #24
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02002228
	movs r1, #128
	movs r2, #248
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02002228
	movs r1, #236
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02002228
	movs r1, #138
	movs r2, #128
	lsls r2, r2, #17
	movs r0, #10
	lsls r1, r1, #18
	bl Func_02002228
	movs r0, #24
	bl Object_GetById
	movs r3, #85
	movs r2, #0
	adds r3, r3, r5
	str r2, [r0, #24]
	strb r2, [r3]
	mov r11, r3
	ldr r3, [r5, #20]
	movs r0, #160
	str r3, [r5, #12]
	movs r3, #85
	adds r3, r3, r6
	strb r2, [r3]
	mov r9, r3
	ldr r3, [r6, #20]
	lsls r0, r0, #4
	str r3, [r6, #12]
	movs r3, #85
	adds r3, r3, r7
	strb r2, [r3]
	mov r10, r3
	ldr r3, [r7, #20]
	adds r0, #10
	str r3, [r7, #12]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008a3c
	ldr r3, [r6, #12]
	ldr r2, .L_02008ad4
	movs r0, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_02008ad8
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02008adc
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #11
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_02008a3c:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008a86
	ldr r3, [r7, #12]
	ldr r2, .L_02008ad4
	movs r0, #10
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02008ad8
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02008ae0
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	ldr r2, .L_02008ae4
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #12
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_02008a86:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ac6
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ac6
	ldr r3, [r6, #12]
	ldr r2, .L_02008ae8
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02008aec
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	mov r2, r10
	strb r3, [r2]
	mov r2, r11
	strb r3, [r2]
.L_02008ac6:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008ad4:
	.4byte 0x00066640
.L_02008ad8:
	.4byte 0x0001eb80
.L_02008adc:
	.4byte 0xfffd70c0
.L_02008ae0:
	.4byte 0x00028f40
.L_02008ae4:
	.4byte 0xfffff800
.L_02008ae8:
	.4byte 0x00199900
.L_02008aec:
	.4byte 0x001b8480
	.section .text.x02008af0,"ax",%progbits
	.global Func_02000af0
	.thumb_func
Func_02000af0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #8
	mov r8, r3
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r5, .L_02008bf4
	ldr r3, [r3, #40]
	movs r1, #0
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r4, r3, #16
	ldrh r3, [r5, r1]
	lsrs r2, r4, #16
	cmp r2, r3
	beq .L_02008b36
.L_02008b1c:
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	lsrs r2, r3, #16
	asrs r1, r3, #16
	cmp r2, #5
	bhi .L_02008b36
	lsls r3, r2, #1
	ldrh r3, [r5, r3]
	lsrs r2, r4, #16
	cmp r2, r3
	bne .L_02008b1c
.L_02008b36:
	lsls r3, r1, #16
	lsrs r2, r3, #16
	cmp r2, #6
	bne .L_02008b42
	movs r0, #0
	b .L_02008bea
.L_02008b42:
	ldr r6, .L_02008bf8
	lsls r2, r2, #2
	ldrsb r4, [r6, r2]
	adds r1, r4, #0
	cmp r4, #0
	bge .L_02008b50
	negs r1, r4
.L_02008b50:
	adds r3, r2, #2
	ldrsb r3, [r6, r3]
	cmp r3, #0
	bge .L_02008b5a
	negs r3, r3
.L_02008b5a:
	adds r3, r1, r3
	asrs r7, r3, #4
	adds r3, r2, #1
	ldrsb r1, [r6, r3]
	adds r5, r1, #0
	cmp r1, #0
	bge .L_02008b6a
	negs r5, r1
.L_02008b6a:
	adds r3, r2, #3
	ldrsb r2, [r6, r3]
	cmp r2, #0
	bge .L_02008b74
	negs r2, r2
.L_02008b74:
	adds r5, r5, r2
	mov r10, r5
	ldr r6, [r0, #8]
	mov r3, r10
	ldr r5, [r0, #16]
	asrs r3, r3, #4
	mov r10, r3
	lsls r3, r4, #16
	adds r6, r6, r3
	lsls r3, r1, #16
	adds r5, r5, r3
	movs r3, #164
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	asrs r6, r6, #20
	asrs r1, r3, #20
	movs r3, #166
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	lsls r2, r1, #16
	asrs r3, r3, #20
	lsls r3, r3, #16
	asrs r5, r5, #20
	lsrs r2, r2, #16
	lsrs r3, r3, #16
	adds r2, r6, r2
	adds r3, r5, r3
	str r2, [sp, #0]
	str r3, [sp, #4]
	adds r0, r6, #0
	adds r1, r5, #0
	adds r2, r7, #0
	mov r3, r10
	bl Func_020021b0
	movs r3, #255
	mov r2, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r8, r3
	movs r0, #0
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl Func_02000bfc
	mov r2, r10
	mov r3, r8
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #2
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl Func_02000bfc
	movs r0, #1
.L_02008bea:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008bf4:
	.4byte Data_020022d0
.L_02008bf8:
	.4byte Data_020022dc
	.section .text.x02008bfc,"ax",%progbits
	.global Func_02000bfc
	.thumb_func
Func_02000bfc:
	push {r5, r6, lr}
	adds r5, r3, #0
	ldr r3, [sp, #12]
	lsls r2, r2, #7
	mov r12, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r0, r0, #1
	lsls r3, r3, #3
	adds r3, r3, r0
	ldr r0, [r4, r3]
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r0, r0, r1
	movs r1, #0
	ldr r6, [sp, #16]
	cmp r1, r12
	bcs .L_02008c42
.L_02008c28:
	lsls r3, r1, #9
	movs r2, #0
	adds r3, r0, r3
	cmp r2, r5
	bcs .L_02008c3c
.L_02008c32:
	adds r2, #1
	strb r6, [r3, #2]
	adds r3, #4
	cmp r2, r5
	bcc .L_02008c32
.L_02008c3c:
	adds r1, #1
	cmp r1, r12
	bcc .L_02008c28
.L_02008c42:
	pop {r5, r6, pc}
	.section .text.x02008c44,"ax",%progbits
	.global Func_02000c44
	.thumb_func
Func_02000c44:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r5, r6, #0
	sub sp, #40
	adds r1, r6, #0
	adds r5, #12
	add r0, sp, #24
	adds r1, #16
	adds r2, r5, #0
	bl Func_02000dac
	adds r4, r0, #0
	cmp r4, #0
	bne .L_02008c6e
	b .L_02008d8e
.L_02008c6e:
	ldr r5, [r5]
	ldr r0, .L_02008da0
	str r5, [sp, #20]
	lsls r1, r5, #2
	ldrsb r2, [r0, r1]
	cmp r2, #0
	bge .L_02008c7e
	negs r2, r2
.L_02008c7e:
	adds r3, r1, #2
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_02008c88
	negs r3, r3
.L_02008c88:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #16]
	adds r3, r1, #1
	ldrsb r2, [r0, r3]
	cmp r2, #0
	bge .L_02008c98
	negs r2, r2
.L_02008c98:
	adds r3, r1, #3
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_02008ca2
	negs r3, r3
.L_02008ca2:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #12]
	ldr r3, [sp, #24]
	ldr r2, .L_02008da4
	ldr r1, .L_02008da8
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	mov r9, r1
	mov r2, r9
	ands r2, r3
	lsls r3, r3, #16
	mov r10, r3
	movs r3, #0
	str r3, [r6, #20]
	mov r11, r3
	adds r3, r4, #0
	adds r3, #34
	str r3, [sp, #8]
	ldr r1, [sp, #8]
	movs r3, #2
	strb r3, [r1]
	mov r9, r2
	ldr r3, [r4, #8]
	add r3, r9
	str r3, [r6]
	ldr r3, [r4, #16]
	add r3, r10
	str r3, [r6, #8]
	ldr r3, [r4, #12]
	str r3, [sp, #32]
.L_02008ce0:
	ldr r3, [sp, #20]
	ldr r2, .L_02008da0
	lsls r3, r3, #2
	str r3, [sp, #4]
	adds r3, #1
	ldrsb r2, [r2, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #16
	adds r3, r3, r2
	ldr r2, [sp, #12]
	movs r1, #0
	mov r8, r1
	str r3, [sp, #36]
	cmp r8, r2
	bge .L_02008d4e
.L_02008cfe:
	ldr r3, .L_02008da0
	ldr r1, [sp, #4]
	add r5, sp, #28
	ldrsb r2, [r3, r1]
	ldr r3, [r6]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [sp, #16]
	movs r7, #0
	cmp r7, r2
	bge .L_02008d38
.L_02008d16:
	adds r0, r4, #0
	add r1, sp, #28
	str r4, [sp, #0]
	bl Func_020021b8
	ldr r4, [sp, #0]
	cmp r0, #2
	beq .L_02008d60
	ldr r3, [r5]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r5]
	ldr r2, [sp, #16]
	adds r7, #1
	cmp r7, r2
	blt .L_02008d16
.L_02008d38:
	add r2, sp, #28
	ldr r3, [r2, #8]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r2, #8]
	ldr r3, [sp, #12]
	movs r2, #1
	add r8, r2
	cmp r8, r3
	blt .L_02008cfe
.L_02008d4e:
	ldr r3, [r6]
	movs r1, #1
	add r3, r9
	str r3, [r6]
	ldr r3, [r6, #8]
	add r11, r1
	add r3, r10
	str r3, [r6, #8]
	b .L_02008ce0
.L_02008d60:
	ldr r2, [sp, #8]
	movs r3, #0
	strb r3, [r2]
	mov r3, r11
	movs r0, #0
	cmp r3, #0
	beq .L_02008d90
	mov r1, r9
	ldr r3, [r4, #8]
	mov r2, r11
	muls r2, r1
	adds r3, r3, r2
	str r3, [r6]
	movs r0, #1
	ldr r3, [r4, #12]
	str r3, [r6, #4]
	mov r3, r10
	mov r2, r11
	muls r2, r3
	ldr r3, [r4, #16]
	adds r3, r3, r2
	str r3, [r6, #8]
	b .L_02008d90
.L_02008d8e:
	movs r0, #0
.L_02008d90:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008da0:
	.4byte Data_020022dc
.L_02008da4:
	.4byte Data_020022f4
.L_02008da8:
	.4byte 0xffff0000
	.section .text.x02008dac,"ax",%progbits
	.global Func_02000dac
	.thumb_func
Func_02000dac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r0, [sp, #8]
	str r1, [sp, #4]
	str r2, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	ldr r3, .L_02008eb8
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	adds r7, r0, #0
	ldrh r3, [r7, #6]
	ldr r1, [sp, #8]
	lsrs r3, r3, #12
	str r3, [r1]
	movs r2, #8
	adds r5, #52
	mov r11, r2
	mov lr, r5
.L_02008de8:
	mov r3, lr
	ldr r6, [r3]
	movs r5, #0
.L_02008dee:
	ldr r3, [r6, #80]
	ldr r2, .L_02008ebc
	ldr r3, [r3, #40]
	movs r0, #0
	ldrsh r1, [r3, r0]
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	cmp r1, r3
	bne .L_02008e92
	ldr r0, [sp, #8]
	movs r2, #10
	ldrsh r1, [r7, r2]
	ldr r3, [r0]
	ldr r2, .L_02008ec0
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	ldr r4, .L_02008ec4
	asrs r2, r3, #16
	adds r1, r1, r2
	asrs r1, r1, #4
	mov r9, r1
	movs r1, #18
	ldrsh r2, [r7, r1]
	lsls r3, r3, #16
	asrs r3, r3, #16
	adds r2, r2, r3
	asrs r2, r2, #4
	mov r8, r2
	movs r2, #10
	ldrsh r0, [r6, r2]
	lsls r2, r5, #2
	ldrsb r3, [r4, r2]
	adds r3, r0, r3
	asrs r3, r3, #4
	mov r10, r3
	movs r3, #18
	ldrsh r1, [r6, r3]
	adds r3, r2, #1
	ldrsb r3, [r4, r3]
	adds r3, r1, r3
	asrs r3, r3, #4
	mov r12, r3
	adds r3, r2, #2
	ldrsb r3, [r4, r3]
	adds r2, #3
	adds r0, r0, r3
	ldrsb r3, [r4, r2]
	asrs r0, r0, #4
	adds r1, r1, r3
	asrs r1, r1, #4
	cmp r10, r9
	bgt .L_02008e92
	cmp r9, r0
	bge .L_02008e92
	cmp r12, r8
	bgt .L_02008e92
	cmp r8, r1
	bge .L_02008e92
	ldr r0, [sp, #0]
	movs r3, #1
	ands r3, r5
	str r5, [r0]
	cmp r3, #0
	beq .L_02008e80
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r10, r3
	beq .L_02008e92
	ldr r2, [sp, #4]
	mov r1, r11
	str r1, [r2]
	adds r0, r6, #0
	b .L_02008ea8
.L_02008e80:
	ldr r3, [r7, #16]
	asrs r3, r3, #20
	cmp r12, r3
	beq .L_02008e92
	ldr r0, [sp, #4]
	mov r3, r11
	str r3, [r0]
	adds r0, r6, #0
	b .L_02008ea8
.L_02008e92:
	adds r5, #1
	cmp r5, #5
	bls .L_02008dee
	movs r2, #1
	add r11, r2
	movs r1, #4
	mov r3, r11
	add lr, r1
	cmp r3, #63
	bls .L_02008de8
	movs r0, #0
.L_02008ea8:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008eb8:
	.4byte gPartyState
.L_02008ebc:
	.4byte Data_020022d0
.L_02008ec0:
	.4byte Data_020022f4
.L_02008ec4:
	.4byte Data_020022dc
	.section .text.x02008ec8,"ax",%progbits
	.global Func_02000ec8
	.thumb_func
Func_02000ec8:
	sub sp, #16
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r0, [sp, #88]
	str r1, [sp, #92]
	str r2, [sp, #96]
	str r3, [sp, #100]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r0, #133
	str r3, [sp, #28]
	ldr r3, .L_02009168
	lsls r0, r0, #2
	adds r0, r0, r3
	mov r10, r0
	ldr r0, [r0]
	bl Object_GetById
	mov r8, r0
	ldr r0, [sp, #104]
	bl Object_GetById
	mov r3, r8
	ldr r3, [r3, #48]
	mov r4, r8
	str r3, [sp, #20]
	adds r6, r0, #0
	ldr r4, [r4, #52]
	mov r0, sp
	adds r0, #32
	str r0, [sp, #12]
	str r4, [sp, #16]
	ldr r2, [sp, #100]
	ldr r3, [r6, #8]
	movs r1, #0
	str r3, [r0]
	mov r9, r1
	ldr r3, [r6, #16]
	mov r1, sp
	adds r1, #44
	str r3, [r0, #8]
	ldr r5, .L_0200916c
	str r1, [sp, #8]
	lsls r7, r2, #2
	ldrsb r1, [r5, r7]
	ldr r3, [r6, #8]
	lsls r2, r1, #16
	adds r3, r3, r2
	ldr r2, [sp, #8]
	asrs r3, r3, #20
	str r3, [r2]
	mov lr, r3
	adds r3, r7, #1
	ldrsb r4, [r5, r3]
	ldr r3, [r6, #16]
	ldr r0, [sp, #8]
	lsls r2, r4, #16
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [r0, #8]
	adds r0, r1, #0
	mov r12, r3
	cmp r0, #0
	bge .L_02008f58
	negs r0, r0
.L_02008f58:
	adds r3, r7, #2
	ldrsb r1, [r5, r3]
	cmp r1, #0
	bge .L_02008f62
	negs r1, r1
.L_02008f62:
	adds r3, r0, r1
	asrs r3, r3, #4
	adds r1, r4, #0
	str r3, [sp, #24]
	cmp r1, #0
	bge .L_02008f70
	negs r1, r1
.L_02008f70:
	adds r3, r7, #3
	ldrsb r2, [r5, r3]
	cmp r2, #0
	bge .L_02008f7a
	negs r2, r2
.L_02008f7a:
	adds r3, r1, r2
	asrs r3, r3, #4
	str r3, [sp, #0]
	mov r11, r3
	movs r3, #0
	str r3, [sp, #4]
	mov r1, lr
	mov r2, r12
	ldr r3, [sp, #24]
	movs r0, #0
	bl Func_02000bfc
	mov r1, r10
	movs r2, #200
	ldr r0, [r1]
	lsls r2, r2, #5
	movs r1, #128
	lsls r1, r1, #8
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	mov r2, r10
	ldr r0, [r2]
	movs r1, #8
	bl Object_SetModeById
	movs r0, #15
	bl WaitFrames
	ldr r4, [sp, #12]
	ldr r1, [sp, #88]
	ldr r3, [r4]
	ldr r2, [sp, #96]
	subs r1, r1, r3
	ldr r3, [r4, #8]
	asrs r1, r1, #17
	subs r2, r2, r3
	mov r3, r10
	asrs r2, r2, #17
	ldr r0, [r3]
	bl ObjectMotion_OffsetPositionAndResetMotion
	mov r4, r10
	ldr r0, [r4]
	bl Object_GetById
	ldr r3, .L_02009170
	str r3, [r0, #108]
	movs r0, #4
	bl WaitFrames
	movs r1, #2
	adds r0, r6, #0
	bl Func_02002160
	movs r0, #239
	bl Func_020022c8
	movs r2, #200
	movs r1, #128
	lsls r2, r2, #5
	ldr r0, [sp, #104]
	lsls r1, r1, #8
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	adds r0, r6, #0
	ldr r1, [sp, #88]
	ldr r2, [sp, #92]
	ldr r3, [sp, #96]
	bl Func_02002190
	ldr r3, .L_02009168
	movs r0, #133
	lsls r0, r0, #2
	adds r5, r3, r0
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #152
	movs r2, #200
	lsls r1, r1, #7
	lsls r2, r2, #5
	ldr r0, [r5]
	adds r1, #204
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	ldr r2, .L_02009174
	mov r1, r9
	lsls r3, r1, #2
	ldr r2, [r2, r3]
	ldr r0, [r5]
	lsls r2, r2, #16
	asrs r1, r2, #31
	asrs r2, r2, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	ldr r3, [sp, #108]
	cmp r3, #0
	beq .L_02009050
	mov lr, r3
	.2byte 0xf800
.L_02009050:
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #1
	ldr r0, [r5]
	bl Object_SetModeById
	mov r3, r8
	movs r2, #0
	str r2, [r3, #108]
	ldr r4, [sp, #20]
	movs r5, #255
	str r4, [r3, #48]
	ldr r0, [sp, #16]
	str r0, [r3, #52]
	adds r0, r6, #0
	bl Func_02002198
	movs r0, #149
	lsls r0, r0, #1
	bl Func_020022c8
	movs r0, #213
	bl Func_020022c8
	ldr r2, [r6, #12]
	ldr r1, [sp, #88]
	ldr r3, [sp, #96]
	adds r0, r6, #0
	bl Object_SetPositionAndResetMotion
	adds r0, r6, #0
	movs r1, #1
	bl Func_02002160
	ldr r1, .L_0200916c
	ldr r0, [sp, #88]
	ldrsb r3, [r1, r7]
	adds r2, r7, #1
	lsls r3, r3, #16
	adds r0, r0, r3
	ldrsb r3, [r1, r2]
	mov r10, r1
	ldr r1, [sp, #96]
	lsls r3, r3, #16
	adds r1, r1, r3
	ldr r4, [sp, #28]
	asrs r0, r0, #20
	asrs r1, r1, #20
	str r0, [sp, #88]
	str r1, [sp, #96]
	mov r9, r2
	movs r2, #164
	lsls r2, r2, #1
	adds r3, r4, r2
	ldr r3, [r3]
	adds r2, #4
	asrs r3, r3, #20
	mov r8, r3
	adds r3, r4, r2
	ldr r6, [r3]
	mov r4, r8
	asrs r6, r6, #20
	adds r3, r4, r0
	adds r2, r6, r1
	str r3, [sp, #0]
	str r2, [sp, #4]
	mov r3, r11
	ldr r2, [sp, #24]
	bl Func_020021b0
	mov r0, r11
	ldr r1, [sp, #88]
	ldr r2, [sp, #96]
	str r0, [sp, #0]
	ldr r3, [sp, #24]
	movs r0, #0
	str r5, [sp, #4]
	bl Func_02000bfc
	mov r3, r11
	ldr r1, [sp, #88]
	ldr r2, [sp, #96]
	str r3, [sp, #0]
	movs r0, #2
	ldr r3, [sp, #24]
	str r5, [sp, #4]
	bl Func_02000bfc
	ldr r0, [sp, #12]
	mov r4, r10
	ldrsb r3, [r4, r7]
	ldr r1, [r0]
	ldr r2, [sp, #8]
	lsls r3, r3, #16
	adds r1, r1, r3
	asrs r1, r1, #20
	str r1, [r2]
	mov r3, r9
	ldrsb r2, [r4, r3]
	ldr r3, [r0, #8]
	ldr r4, [sp, #8]
	lsls r2, r2, #16
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [r4, #8]
	add r8, r1
	adds r6, r6, r3
	str r1, [sp, #0]
	str r3, [sp, #4]
	ldr r2, [sp, #24]
	mov r0, r8
	adds r1, r6, #0
	mov r3, r11
	bl Func_020021b0
	ldr r0, [sp, #8]
	mov r3, r11
	ldr r1, [r0]
	ldr r2, [r0, #8]
	movs r4, #0
	str r3, [sp, #0]
	movs r0, #2
	ldr r3, [sp, #24]
	str r4, [sp, #4]
	bl Func_02000bfc
	bl Func_020022a0
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r3}
	add sp, #16
	bx r3
	.2byte 0x0000
.L_02009168:
	.4byte gPartyState
.L_0200916c:
	.4byte Data_020022dc
.L_02009170:
	.4byte Func_02001178
.L_02009174:
	.4byte Data_020022f4
	.section .text.x02009178,"ax",%progbits
	.global Func_02001178
	.thumb_func
Func_02001178:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldrh r3, [r5, #6]
	movs r2, #12
	lsrs r1, r3, #12
	adds r3, r1, #2
	ands r3, r2
	lsls r1, r3, #12
	ldr r3, [r5, #8]
	sub sp, #12
	mov r6, sp
	str r3, [r6]
	ldr r3, [r5, #12]
	movs r0, #128
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	lsls r0, r0, #13
	adds r2, r6, #0
	str r3, [r6, #8]
	bl Vector_AddPolarOffsetFar
	adds r0, r6, #0
	movs r1, #1
	bl Func_020022a8
	cmp r0, #0
	beq .L_020091d4
	movs r4, #0
.L_020091b0:
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldr r2, .L_020091fc
	lsls r3, r4, #1
	ldrh r3, [r2, r3]
	cmp r1, r3
	beq .L_020091f8
	adds r4, #1
	cmp r4, #5
	bls .L_020091b0
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
.L_020091d4:
	ldr r3, [r5, #8]
	adds r0, r5, #0
	str r3, [r6]
	ldr r3, [r5, #12]
	adds r1, r6, #0
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	str r3, [r6, #8]
	bl Func_020021b8
	cmp r0, #0
	ble .L_020091f8
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
.L_020091f8:
	add sp, #12
	pop {r5, r6, pc}
.L_020091fc:
	.4byte Data_020022d0
	.section .text.x02009200,"ax",%progbits
	.global Func_02001200
	.thumb_func
Func_02001200:
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
	ldr r3, .L_02009394
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_02009398
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_0200939c
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_020093a0
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_020093a4
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_02009252
	b .L_02009386
.L_02009252:
	ldr r2, .L_020093a8
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_02009260
	b .L_02009376
.L_02009260:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_02009268
	b .L_02009376
.L_02009268:
	mov r1, r10
	subs r0, r3, r1
	ldr r2, [sp, #0]
	ldr r3, [r5, #12]
	movs r1, #128
	subs r3, r3, r2
	ldr r2, [r5, #16]
	lsls r1, r1, #12
	adds r3, r3, r1
	mov r1, r8
	subs r2, r2, r1
	ldr r1, [sp, #0]
	subs r2, r2, r1
	subs r4, r2, r3
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r3, #58
	mov r11, r3
	ldr r3, .L_020093ac
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_020092de
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #7
	movs r1, #167
	adds r4, r2, #0
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #16
	cmp r3, r1
	bhi .L_02009376
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_02009376
	cmp r4, #239
	bgt .L_02009376
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	mov r3, r12
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_020093b0
	b .L_0200931a
.L_020092de:
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #0
	movs r1, #175
	adds r4, r2, #0
	adds r3, #23
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #64
	cmp r3, r1
	bhi .L_02009376
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_02009376
	cmp r4, #175
	bgt .L_02009376
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	movs r3, #0
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_020093b4
.L_0200931a:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_020093b8
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_02009358
	adds r0, r5, #0
	bl Func_020022b0
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	lsls r0, r0, #2
	ands r3, r2
	orrs r3, r0
	strb r3, [r6, #9]
	b .L_0200936c
.L_02009358:
	movs r3, #3
	ands r3, r2
	movs r0, #13
	ldrb r2, [r6, #9]
	negs r0, r0
	adds r1, r0, #0
	lsls r3, r3, #2
	ands r2, r1
	orrs r2, r3
	strb r2, [r6, #9]
.L_0200936c:
	adds r0, r6, #0
	mov r1, r11
	bl Func_02002130
	adds r6, #12
.L_02009376:
	ldr r3, .L_020093a4
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_02009386
	b .L_02009252
.L_02009386:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009394:
	.4byte 0xffff0000
.L_02009398:
	.4byte gOverlayArea + 0x2af0
.L_0200939c:
	.4byte ResourceTableEntries
.L_020093a0:
	.4byte gOverlayArea + 0x2b34
.L_020093a4:
	.4byte gOverlayArea + 0x2af2
.L_020093a8:
	.4byte gOverlayArea + 0x2af4
.L_020093ac:
	.4byte gOverlayArea + 0x2bf4
.L_020093b0:
	.4byte 0x40002000
.L_020093b4:
	.4byte 0xc000a000
.L_020093b8:
	.4byte gOverlayArea + 0x2bf6
	.section .text.x020093bc,"ax",%progbits
	.global Func_020013bc
	.thumb_func
Func_020013bc:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_0200941c
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_02009420
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_02009424
	bl Func_02002118
	ldr r5, .L_02009428
	bl Resource_FindFreeEntry
	movs r1, #192
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200942c
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_02009430
	ldr r2, .L_02009414
	strh r2, [r3]
	ldr r3, .L_02009434
	strh r2, [r3]
	ldr r2, .L_02009438
	ldr r3, .L_02009418
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009414:
	.4byte 0x00000000
.L_02009418:
	.4byte 0xffffffff
.L_0200941c:
	.4byte IwramClearWords
.L_02009420:
	.4byte gOverlayArea + 0x2af4
.L_02009424:
	.4byte Data_02002334
.L_02009428:
	.4byte gOverlayArea + 0x2af0
.L_0200942c:
	.4byte Func_02001200
.L_02009430:
	.4byte gOverlayArea + 0x2af2
.L_02009434:
	.4byte gOverlayArea + 0x2bf4
.L_02009438:
	.4byte gOverlayArea + 0x2bf6
	.section .text.x0200943c,"ax",%progbits
	.global Func_0200143c
	.thumb_func
Func_0200143c:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_0200949c
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_020094a0
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_020094a4
	bl Func_02002118
	ldr r5, .L_020094a8
	bl Resource_FindFreeEntry
	movs r1, #128
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_020094ac
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_020094b0
	ldr r2, .L_02009494
	strh r2, [r3]
	ldr r3, .L_020094b4
	strh r2, [r3]
	ldr r2, .L_020094b8
	ldr r3, .L_02009498
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009494:
	.4byte 0x00000000
.L_02009498:
	.4byte 0xffffffff
.L_0200949c:
	.4byte IwramClearWords
.L_020094a0:
	.4byte gOverlayArea + 0x2af4
.L_020094a4:
	.4byte Data_02002496 + 0x1
.L_020094a8:
	.4byte gOverlayArea + 0x2af0
.L_020094ac:
	.4byte Func_02001200
.L_020094b0:
	.4byte gOverlayArea + 0x2af2
.L_020094b4:
	.4byte gOverlayArea + 0x2bf4
.L_020094b8:
	.4byte gOverlayArea + 0x2bf6
	.section .text.x020094bc,"ax",%progbits
	.global Func_020014bc
	.thumb_func
Func_020014bc:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_02009520
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_02009524
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_02009528
	bl Func_02002118
	ldr r5, .L_0200952c
	bl Resource_FindFreeEntry
	movs r1, #128
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009530
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_02009534
	ldr r3, .L_02009514
	strh r3, [r2]
	ldr r2, .L_02009538
	ldr r3, .L_02009518
	strh r3, [r2]
	ldr r2, .L_0200953c
	ldr r3, .L_0200951c
	strh r3, [r2]
	b .L_02009540
.L_02009514:
	.4byte 0x00000000
.L_02009518:
	.4byte 0x00000001
.L_0200951c:
	.4byte 0xffffffff
.L_02009520:
	.4byte IwramClearWords
.L_02009524:
	.4byte gOverlayArea + 0x2af4
.L_02009528:
	.4byte Data_020026c6
.L_0200952c:
	.4byte gOverlayArea + 0x2af0
.L_02009530:
	.4byte Func_02001200
.L_02009534:
	.4byte gOverlayArea + 0x2af2
.L_02009538:
	.4byte gOverlayArea + 0x2bf4
.L_0200953c:
	.4byte gOverlayArea + 0x2bf6
.L_02009540:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02009544,"ax",%progbits
	.global Func_02001544
	.thumb_func
Func_02001544:
	push {r5, lr}
	adds r5, r1, #0
	bl Object_GetById
	adds r4, r0, #0
	cmp r4, #0
	beq .L_0200956a
	adds r3, r4, #0
	adds r3, #100
	strh r5, [r3]
	ldr r1, .L_0200956c
	ldr r0, .L_02009570
	ldrh r2, [r1]
	movs r5, #0
	ldrsh r3, [r1, r5]
	adds r2, #1
	lsls r3, r3, #2
	str r4, [r0, r3]
	strh r2, [r1]
.L_0200956a:
	pop {r5, pc}
.L_0200956c:
	.4byte gOverlayArea + 0x2af2
.L_02009570:
	.4byte gOverlayArea + 0x2af4
	.section .text.x02009574,"ax",%progbits
	.global Func_02001574
	.thumb_func
Func_02001574:
	ldr r3, .L_0200957c
	strh r0, [r3]
	bx lr
	.2byte 0x0000
.L_0200957c:
	.4byte gOverlayArea + 0x2bf6
	.section .text.x020095c6,"ax",%progbits
	.2byte 0x0000
	.section .text.x020095c8,"ax",%progbits
	.global Func_020015c8
	.thumb_func
Func_020015c8:
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
	.section .text.x02009600,"ax",%progbits
	.global Func_02001600
	.thumb_func
Func_02001600:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_020097b8
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
	beq .L_02009648
	cmp r7, #0
	beq .L_02009648
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_02009650
.L_02009648:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_02009650:
	mov r3, r10
	bl Func_02002178
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200965e
	b .L_020097aa
.L_0200965e:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02002160
	ldr r2, .L_020097bc
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02002170
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_020097c0
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
	bl Object_SetSpritePriority
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_020097c4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_020097aa
	cmp r7, #0
	beq .L_020097aa
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_020096e0
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_020096e0:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009700
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_02009700:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_02009714
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02009714:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200975a
	ldr r3, .L_020097bc
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_02009742
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_02009754
.L_02009742:
	ldr r2, .L_020097c4
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_020097c4
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_02009754:
	bl Engine_MathDivide
	str r0, [r6, #52]
.L_0200975a:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009776
	adds r0, r6, #0
	movs r1, #1
	bl Func_02002160
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02002170
.L_02009776:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009788
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_02009788:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200979a
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200979a:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020097aa
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_020097aa:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020097b8:
	.4byte gPartyState
.L_020097bc:
	.4byte Data_02002aa0
.L_020097c0:
	.4byte Func_020015c8
.L_020097c4:
	.4byte 0xffff0000
	.section .text.x020097c8,"ax",%progbits
	.global Func_020017c8
	.thumb_func
Func_020017c8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r4, .L_020098e0
	movs r1, #1
	movs r0, #12
	ldrsh r3, [r4, r0]
	negs r1, r1
	sub sp, #4
	cmp r3, r1
	beq .L_020098d4
	lsls r3, r3, #3
	adds r3, r3, r4
	adds r3, #32
	mov r8, r3
	ldr r3, .L_020098e4
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
	bne .L_02009814
	ldr r3, [r0, #16]
	movs r5, #2
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	cmp r3, r2
	beq .L_0200981c
.L_02009814:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r4, #12]
.L_0200981c:
	movs r0, #12
	ldrsh r3, [r4, r0]
	movs r2, #1
	negs r2, r2
	ldr r1, .L_020098e8
	cmp r3, r2
	beq .L_020098d4
	movs r5, #14
	ldrsh r3, [r4, r5]
	cmp r3, #0
	beq .L_020098d4
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
	ldr r2, .L_020098ec
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
	bhi .L_020098d4
	movs r0, #15
	negs r0, r0
	cmp r2, r0
	blt .L_020098d4
	cmp r2, #239
	bgt .L_020098d4
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
	ldr r3, .L_020098f0
	adds r0, r4, #0
	orrs r2, r3
	movs r3, #128
	str r2, [r4, #24]
	lsls r3, r3, #3
	mov r2, r10
	orrs r2, r3
	str r2, [r4, #28]
	adds r0, #20
	bl Func_02002130
.L_020098d4:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020098e0:
	.4byte gOverlayArea + 0x2bf8
.L_020098e4:
	.4byte gPartyState
.L_020098e8:
	.4byte 0xffff0000
.L_020098ec:
	.4byte ResourceTableEntries
.L_020098f0:
	.4byte 0x80008800
	.section .text.x020098f4,"ax",%progbits
	.global Func_020018f4
	.thumb_func
Func_020018f4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_02009b24
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
	ldr r3, .L_02009b28
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
	bge .L_02009a78
.L_020099a8:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_02009a6c
.L_020099bc:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_02009a5c
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_02009a5c
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_02009a5c
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
	bne .L_02009a10
	cmp r5, r10
	bne .L_02009a4e
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_02009a4e
.L_02009a10:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009a4e
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
	bl Func_020021a8
.L_02009a4e:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_02009a5c:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_020099bc
.L_02009a6c:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_020099a8
.L_02009a78:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009ad0
	ldr r3, .L_02009b2c
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
	bge .L_02009ad0
.L_02009aaa:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_02009ac0
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_02009ac0
	mov r0, r8
	strh r2, [r0, #12]
.L_02009ac0:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_02009aaa
.L_02009ad0:
	movs r0, #128
	lsls r0, r0, #1
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_02009ade:
	ldr r3, .L_02009b30
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_02009ade
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
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009b34
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
.L_02009b24:
	.4byte gOverlayArea + 0x2bf8
.L_02009b28:
	.4byte IwramClearWords
.L_02009b2c:
	.4byte gPartyState
.L_02009b30:
	.4byte 0x11111111
.L_02009b34:
	.4byte Func_020017c8
	.section .text.x02009b38,"ax",%progbits
	.global Func_02001b38
	.thumb_func
Func_02001b38:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r1, .L_02009bb8
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
	bl Func_02002268
	ldr r2, .L_02009bbc
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009bb8:
	.4byte gPartyState
.L_02009bbc:
	.4byte 0xfff80000
	.section .text.x02009bc0,"ax",%progbits
	.global Func_02001bc0
	.thumb_func
Func_02001bc0:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, .L_02009c2c
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
	bl Func_02001b38
	movs r0, #161
	bl Func_020022c8
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
	bl Func_020021a8
	movs r0, #12
	bl Battle_WaitMode0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02009c2c:
	.4byte gPartyState
	.section .text.x02009c30,"ax",%progbits
	.global Func_02001c30
	.thumb_func
Func_02001c30:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_02009ce0
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
	bl Func_02001b38
	movs r0, #229
	bl Func_020022c8
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
	bl Func_020021a8
	movs r0, #12
	bl Battle_WaitMode0
	movs r3, #128
	ldr r2, .L_02009cd8
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
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
	ldr r2, .L_02009cdc
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_02009ce4
	.2byte 0x0000
.L_02009cd8:
	.4byte 0x00000000
.L_02009cdc:
	.4byte 0x00008000
.L_02009ce0:
	.4byte gPartyState
.L_02009ce4:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
.L_02009cf8:
	cmp r7, #5
	bne .L_02009d02
	movs r0, #204
	bl Func_020022c8
.L_02009d02:
	ldr r3, [r6, #24]
	ldr r1, .L_02009d60
	ldr r2, .L_02009d64
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_02009d68
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_02009cf8
	ldr r3, .L_02009d6c
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
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02009d60:
	.4byte 0xfffffc00
.L_02009d64:
	.4byte 0xfffffd00
.L_02009d68:
	.4byte 0xffff6667
.L_02009d6c:
	.4byte gPartyState
	.section .text.x02009d70,"ax",%progbits
	.global Func_02001d70
	.thumb_func
Func_02001d70:
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
	bge .L_02009da0
	adds r3, #15
.L_02009da0:
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
	.section .text.x02009dc8,"ax",%progbits
	.global Func_02001dc8
	.thumb_func
Func_02001dc8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009f4c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_020021f0
	movs r0, #0
	bl Func_02002290
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02002188
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
	bl Func_020022c8
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_02009f50
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_02009e62:
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
	ldr r3, .L_02009f54
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_02009f58
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
	ldr r4, .L_02009f5c
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_02001600
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_02009e62
	movs r0, #188
	bl Func_020022c8
	ldr r5, .L_02009f4c
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02002250
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_020021c8
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_020021c8
	bl Func_020021d0
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02002250
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
	bl Func_020021f8
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02009f4c:
	.4byte gPartyState
.L_02009f50:
	.4byte Func_02001d70
.L_02009f54:
	.4byte 0xffffa000
.L_02009f58:
	.4byte 0xffffd000
.L_02009f5c:
	.4byte 0x01090001
	.section .text.x02009f60,"ax",%progbits
	.global Func_02001f60
	.thumb_func
Func_02001f60:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a008
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200a00c
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
	bge .L_02009ffc
.L_02009f94:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_02009ff0
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_02009ff0
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009fc4
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_02001bc0
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_02009ffc
.L_02009fc4:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_02009ffc
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_02001c30
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
	b .L_02009ffe
.L_02009ff0:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_02009f94
.L_02009ffc:
	movs r0, #0
.L_02009ffe:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a008:
	.4byte gPartyState
.L_0200a00c:
	.4byte gOverlayArea + 0x2bf8
	.section .text.x0200a010,"ax",%progbits
	.global Func_02002010
	.thumb_func
Func_02002010:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r3, .L_0200a0c0
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r9, r0
	ldr r0, [r3]
	mov r11, r1
	bl Object_GetById
	movs r3, #192
	ldr r5, .L_0200a0c4
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
	bne .L_0200a05e
	cmp r0, #0
	beq .L_0200a0b2
.L_0200a05e:
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
	bl Func_02002188
	bl Func_02001dc8
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #0]
	str r2, [r3]
.L_0200a0b2:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a0c0:
	.4byte gPartyState
.L_0200a0c4:
	.4byte gOverlayArea + 0x2bf8
	.section .rodata.x0200a2d0,"a",%progbits
	.global Data_020022d0
Data_020022d0:
	.4byte 0x01030102
	.4byte 0x01260125
	.4byte 0x014b014c
	.global Data_020022dc
Data_020022dc:
	.4byte Sound_Wave29 + 0x134c
	.4byte 0x2008e0f8
	.4byte 0x0020f0e0
	.4byte 0x2008e0f8
	.4byte Sound_Wave29 + 0x134c
	.4byte 0x2008e0f8
	.global Data_020022f4
Data_020022f4:
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
	.global Data_02002334
Data_02002334:
	.4byte 0x06345d01
	.4byte Runtime_ReciprocalTable + 0x1259
	.4byte 0x2f010026
	.4byte 0x5f1f7000
	.4byte 0x667b0906
	.4byte 0x01040901
	.4byte 0x0e287800
	.4byte 0x56053b3f
	.4byte 0x5f6f080e
	.4byte 0x08100800
	.4byte 0xa1076601
	.4byte 0x001d0800
	.4byte Text_MessageContexts + 0x10b38
	.4byte 0x660015bb
	.4byte 0x0e020016
	.4byte 0x66177000
	.4byte 0x3b02080d
	.4byte Data_02001024 + 0xbb
	.4byte 0x66128010
	.4byte 0x01037a01
	.4byte 0x04277910
	.4byte 0xfb44013d
	.4byte Data_0200752c + 0x2d4
	.4byte 0x20590414
	.4byte 0x57052700
	.4byte 0x50002066
	.4byte 0x20ff0016
	.4byte 0x00162a00
	.4byte 0x0016002e
	.4byte 0x162a0020
	.4byte 0x4e002000
	.4byte 0x7f320040
	.4byte 0x04aa023b
	.4byte 0x02500006
	.4byte 0xe7169207
	.4byte 0x0010e103
	.4byte 0x0301ff0d
	.4byte 0x0c01e709
	.4byte 0x5906dd03
	.4byte 0x210a6118
	.4byte 0xfd340030
	.4byte 0x0403a425
	.4byte 0x5f290630
	.4byte 0x02490100
	.4byte 0xf8063b58
	.4byte 0x3b5704af
	.4byte 0x163bb406
	.4byte Resource_DecodeHalfwordLzCode + 0x12
	.4byte 0x283ea00f
	.4byte 0x070820ff
	.4byte 0x2e050010
	.4byte 0x057807f8
	.4byte 0x0a980488
	.4byte 0xdd660ba8
	.4byte 0x07080030
	.4byte 0x38205f22
	.4byte 0x33481706
	.4byte 0x00603b58
	.4byte 0x783ffe78
	.4byte Text_MessageContexts + 0x15ce8
	.4byte 0x1f017900
	.4byte 0x0a782700
	.4byte 0x2aff5f11
	.4byte 0x701f07a8
	.4byte 0xef273000
	.4byte 0x00505704
	.4byte 0x4c002037
	.4byte 0xf7000040
	.4byte 0x0040002a
	.4byte 0x1100206e
	.4byte 0x403b0046
	.4byte 0x00200000
	.4byte 0x2a00402c
	.4byte 0x244819f7
	.4byte 0x2d004000
	.4byte 0x043b0803
	.4byte 0x05830722
	.4byte 0x1817b705
	.4byte 0x0028003b
	.4byte 0x66015920
	.4byte 0x00072260
	.4byte 0x6805041a
	.4byte 0x3f1b01ff
	.4byte 0x06150458
	.4byte 0x40182a2c
	.4byte 0x8f000900
	.4byte 0xe0130302
	.4byte 0x0030cd0c
	.2byte 0x0002
	.global Data_02002496
Data_02002496:
	.2byte 0x0000
	.4byte 0xac862b05
	.4byte 0xaf643138
	.4byte 0x426905df
	.4byte 0xabd8ac4e
	.4byte 0xbe810f20
	.4byte 0x7015130e
	.4byte 0x58aaf4f3
	.4byte 0x1c57fed1
	.4byte 0xba72f5fa
	.4byte 0x15f3c78f
	.4byte 0x2c4bddfc
	.4byte 0x8c405c2e
	.4byte 0x5a901050
	.4byte 0x669c3d02
	.4byte 0x813f657c
	.4byte 0xc48af4a0
	.4byte 0x70f27d1e
	.4byte 0xdfcf8f1f
	.4byte 0x0be667d7
	.4byte 0x54be3c30
	.4byte 0xa442f8f4
	.4byte 0xeb17cfc7
	.4byte 0x058022f4
	.4byte 0xc3a1d0c0
	.4byte 0xf05c0a2b
	.4byte 0xbcc4f23e
	.4byte 0xe8ae7760
	.4byte 0x38745730
	.4byte 0xe6259c57
	.4byte 0x0e73008e
	.4byte 0x075f2408
	.4byte 0x1297a050
	.4byte 0x8f028687
	.4byte 0xebc03222
	.4byte 0x3e706883
	.4byte 0x8034e810
	.4byte 0x161c479c
	.4byte 0x9c090038
	.4byte 0x0a75ee0f
	.4byte 0xbf6039f8
	.4byte 0xf613af4e
	.4byte Tileset_Set112TilesA + 0x21e
	.4byte 0x3df8973f
	.4byte 0xf81f363e
	.4byte 0x63e27cd8
	.4byte 0x9df381f3
	.4byte 0xf7e793f3
	.4byte 0xd0e1c9fa
	.4byte 0xf258ce1d
	.4byte 0x3873c780
	.4byte 0xb93be3cc
	.4byte 0x2f3be3cc
	.4byte 0x6118fcc1
	.4byte 0x980e30ca
	.4byte 0x7cef9c33
	.4byte 0x29f1be0c
	.4byte 0xc7c786f0
	.4byte 0x5edecbe6
	.4byte 0x9239f837
	.4byte 0xebc2b9d8
	.4byte 0xd2f88ad8
	.4byte 0xa9bcc731
	.4byte 0x71e99cc7
	.4byte 0x5f547988
	.4byte 0x27049ac1
	.4byte 0xd7ae264f
	.4byte 0xc532a694
	.4byte 0x12fa7ae5
	.4byte 0xc43ef833
	.4byte 0x7d1e54f6
	.4byte 0x97fdb2f9
	.4byte 0xc6a7c3dd
	.4byte 0x3415957d
	.4byte 0x1f1897ad
	.4byte 0xac141210
	.4byte 0x047b4ae7
	.4byte 0x9eb43c03
	.4byte 0xcd180907
	.4byte 0x000079ea
	.4byte 0xf6450423
	.4byte 0xa7d80d99
	.4byte 0x78059b49
	.4byte 0x467a0580
	.4byte 0xb85c205b
	.4byte 0xd050008c
	.4byte 0x500b31ab
	.4byte 0x7833d893
	.4byte 0x8307f302
	.4byte 0x3a9c0502
	.4byte 0x3831102c
	.4byte 0x306a301c
	.4byte 0x3831101d
	.4byte 0x3026301c
	.4byte 0x38f2201a
	.4byte 0x0f2f2e80
	.4byte 0xc0861624
	.4byte 0x1f1ec23c
	.4byte 0x04f95397
	.4byte 0xc067c07c
	.4byte 0xc701f244
	.4byte 0x644c7e42
	.4byte 0x28ae605c
	.4byte 0xa2b8f289
	.4byte Battle_PurpleCaveBackdrop + 0x3607
	.4byte 0xb7fbd5ce
	.4byte 0x8db92233
	.4byte 0xa5310573
	.4byte 0x8e792a73
	.4byte 0x80804174
	.4byte 0x783318f3
	.4byte 0x3905033e
	.4byte 0x7c0bd78f
	.4byte 0x71e9ce5c
	.4byte 0xebc2b80d
	.4byte 0xfe12e605
	.4byte 0x385d04be
	.4byte 0xc06a9047
	.4byte 0x83a09e03
	.4byte 0x1efc23d7
	.4byte 0xf02b869f
	.4byte 0x3002f9f9
	.4byte 0x3f0df1e0
	.4byte 0x793f904f
	.4byte 0xc78f423c
	.4byte 0x3e067c3b
	.4byte 0x3e29ebe7
	.4byte 0xf7f39f16
	.4byte 0x33967f4d
	.4byte 0x780c9067
	.4byte 0x6ce3c57c
	.4byte 0x37ce61c6
	.4byte 0x37cc0e8c
	.4byte 0xcfc6d8fc
	.4byte 0xfcf83ef7
	.4byte 0xe07f9f03
	.4byte 0xfe7c0ff3
	.4byte 0xf03fcf81
	.4byte 0xff3e07f9
	.4byte 0xf816c7c0
	.2byte 0x0000
	.global Data_020026c6
Data_020026c6:
	.2byte 0x0100
	.4byte 0xcf81fe7c
	.4byte 0x07f9f03f
	.4byte 0x6080ff3e
	.4byte 0xead9f7b4
	.4byte 0x1e7cf8d3
	.4byte 0xca9f3dc1
	.4byte 0x8bf7f3e3
	.4byte 0x5b2fc50f
	.4byte 0xf00e1c7d
	.4byte 0xe7c5f8b1
	.4byte 0x4a1f1bef
	.4byte 0x8bf0f3e0
	.4byte 0x3e3dc58f
	.4byte 0xa1f0df7f
	.4byte 0x0a7dbf05
	.4byte 0xf8d3e16e
	.4byte 0x829f7b46
	.4byte 0xe7b8e75e
	.4byte 0x828df1f3
	.4byte 0x5ad8295e
	.4byte 0x3e060bd0
	.4byte 0x49a8e07d
	.4byte 0x6264f270
	.4byte 0x32a6829c
	.4byte 0x127b65c5
	.4byte 0xf6fcf033
	.4byte 0x54f6c43c
	.4byte 0xb2f97d1e
	.4byte 0xc3dd97fd
	.4byte 0x957dc6a7
	.4byte 0x8914e015
	.4byte 0x012101f1
	.4byte 0x166b2b8f
	.4byte 0xf00c11e4
	.4byte 0x241e7ad0
	.4byte 0xe7ab3460
	.4byte 0x108c0001
	.4byte 0x3667d914
	.4byte 0xde0f9f60
	.4byte 0x1b4b2a37
	.4byte 0x2c23c02c
	.4byte 0x02da33d0
	.4byte 0x0465c6e1
	.4byte 0x8d5e8282
	.4byte 0xc49a8459
	.4byte 0x9813c19e
	.4byte 0x2854187f
	.4byte 0x8161d4e0
	.4byte 0x80e1c388
	.4byte 0x80e98751
	.4byte 0x80e1c388
	.4byte 0x00d18131
	.4byte 0x7401c791
	.4byte 0xb1207979
	.4byte 0x7c37dd8f
	.4byte 0x00000000
.L_0200a7a8:
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
.L_0200a7e4:
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
.L_0200a820:
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
	.4byte 0x00000105
	.4byte 0x00102105
	.4byte 0x00201105
	.4byte 0x00304105
	.4byte 0x00403105
	.4byte 0x00506105
	.4byte 0x00605105
	.4byte 0x00707107
	.4byte 0x00808103
	.4byte 0x0090a105
	.4byte 0x00a09105
	.4byte 0x00b0c105
	.4byte 0x00c0b105
	.4byte 0x00d0d105
	.4byte 0x000001ff
	.global gScenePlacements
gScenePlacements:
	.4byte 0xffff0154
	.4byte 0x00000001
	.4byte 0x00cb0000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00022000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00026000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff014b
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00024000
	.4byte 0xffff014b
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00024000
	.4byte 0xffff014c
	.4byte 0x00000001
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff014c
	.4byte 0x00000001
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff014c
	.4byte 0x00000001
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneEvents
gSceneEvents:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
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
	.4byte 0x00000021
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000021
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000031
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000202
	.4byte 0xffff001e
	.4byte Func_02000068
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte Func_02000054
	.4byte 0x00000002
	.4byte 0xffff0022
	.4byte Func_02000054
	.4byte 0x00000002
	.4byte 0xffff0023
	.4byte Func_02000054
	.4byte 0x10008c15
	.4byte 0xffff000a
	.4byte Data_02000090 + 0x1
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte Func_02000094
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002a84
Data_02002a84:
	.4byte 0xffffffff
	.global Data_02002a88
Data_02002a88:
	.4byte 0x00000001
	.global Data_02002a8c
Data_02002a8c:
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.global Data_02002aa0
Data_02002aa0:
	.4byte .L_0200a7a8
	.4byte .L_0200a7e4
	.4byte .L_0200a820
