.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008080,"ax",%progbits
	.global Func_02000080
	.thumb_func
Func_02000080:
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
	.section .text.x020080b8,"ax",%progbits
	.global Func_020000b8
	.thumb_func
Func_020000b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_02008270
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
	beq .L_02008100
	cmp r7, #0
	beq .L_02008100
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_02008108
.L_02008100:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_02008108:
	mov r3, r10
	bl Func_02002f98
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02008116
	b .L_02008262
.L_02008116:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02002f80
	ldr r2, .L_02008274
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02002f90
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_02008278
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
	ldr r3, .L_0200827c
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02008262
	cmp r7, #0
	beq .L_02008262
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02008198
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_02008198:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020081b8
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_020081b8:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_020081cc
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_020081cc:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02008212
	ldr r3, .L_02008274
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_020081fa
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200820c
.L_020081fa:
	ldr r2, .L_0200827c
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200827c
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200820c:
	bl __divsi3
	str r0, [r6, #52]
.L_02008212:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200822e
	adds r0, r6, #0
	movs r1, #1
	bl Func_02002f80
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02002f90
.L_0200822e:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008240
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_02008240:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008252
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_02008252:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008262
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_02008262:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008270:
	.4byte gPartyState
.L_02008274:
	.4byte Data_02003334
.L_02008278:
	.4byte Func_02000080
.L_0200827c:
	.4byte 0xffff0000
	.section .text.x02008280,"ax",%progbits
	.global Func_02000280
	.thumb_func
Func_02000280:
	push {lr}
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #8
	movs r1, #52
	bl Func_020030f0
	pop {pc}
	.2byte 0x0000
	.section .text.x02008298,"ax",%progbits
	.global Func_02000298
	.thumb_func
Func_02000298:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r5, #60
.L_0200829e:
	cmp r5, #0
	beq .L_020082b0
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #40]
	subs r5, #1
	cmp r3, #0
	bne .L_0200829e
.L_020082b0:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020082b4,"ax",%progbits
	.global Func_020002b4
	.thumb_func
Func_020002b4:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_020082c4
	movs r0, #0
	b .L_020082ea
.L_020082c4:
	cmp r0, #2
	bhi .L_020082d8
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_020082da
.L_020082d8:
	ldr r4, .L_020082ec
.L_020082da:
	lsls r3, r2, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	ldrb r0, [r4, #2]
	ldrb r3, [r4, #3]
	lsls r0, r0, #8
	orrs r0, r3
.L_020082ea:
	pop {pc}
.L_020082ec:
	.4byte gMapCellBuffer
	.section .text.x020082f0,"ax",%progbits
	.global Func_020002f0
	.thumb_func
Func_020002f0:
	push {r5, lr}
	adds r5, r1, #0
	adds r1, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_02008304
	movs r0, #0
	b .L_02008330
.L_02008304:
	cmp r0, #2
	bhi .L_02008318
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_0200831a
.L_02008318:
	ldr r4, .L_02008334
.L_0200831a:
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
.L_02008330:
	pop {r5, pc}
	.2byte 0x0000
.L_02008334:
	.4byte gMapCellBuffer
	.section .text.x02008338,"ax",%progbits
	.global Func_02000338
	.thumb_func
Func_02000338:
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
	mov r9, r3
	ldr r3, [r5, #16]
	mov r1, r9
	asrs r3, r3, #20
	mov r10, r3
	mov r2, r10
	bl Func_020002b4
	mov r1, r9
	mov r2, r10
	mov r8, r0
	movs r0, #2
	bl Func_020002b4
	movs r2, #34
	adds r2, r2, r5
	adds r6, r0, #0
	mov r11, r2
	ldrb r0, [r2]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Map_GetTerrainHeight
	adds r3, r5, #0
	adds r3, #100
	asrs r7, r0, #19
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083b0
	ldr r3, .L_02008400
	mov r2, r8
	ands r6, r3
	movs r3, #129
	negs r3, r3
	ands r2, r3
	ldr r3, [r5, #20]
	mov r8, r2
	asrs r3, r3, #19
	cmp r3, r7
	beq .L_020083a6
	subs r7, #4
.L_020083a6:
	adds r0, r5, #0
	movs r1, #3
	bl Object_SetSpritePriority
	b .L_020083ca
.L_020083b0:
	movs r3, #255
	ands r6, r3
	lsls r3, r3, #8
	mov r2, r8
	orrs r6, r3
	movs r3, #128
	orrs r2, r3
	adds r0, r5, #0
	movs r1, #2
	mov r8, r2
	adds r7, #4
	bl Object_SetSpritePriority
.L_020083ca:
	mov r1, r9
	mov r2, r10
	mov r3, r8
	movs r0, #0
	bl Func_020002f0
	mov r1, r9
	mov r2, r10
	adds r3, r6, #0
	movs r0, #2
	bl Func_020002f0
	mov r3, r9
	mov r2, r10
	lsls r0, r3, #20
	mov r3, r11
	lsls r1, r2, #20
	ldrb r2, [r3]
	adds r3, r7, #0
	bl Func_02003030
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008400:
	.4byte 0xffff00ff
	.section .text.x02008404,"ax",%progbits
	.global Func_02000404
	.thumb_func
Func_02000404:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_02008478
	adds r7, r0, #0
.L_02008418:
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
	adds r5, r5, r0
	adds r1, r5, #0
	adds r0, r6, #0
	bl Func_02002f80
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
	bl Func_02000338
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_02008418
.L_02008478:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008488,"ax",%progbits
	.global Func_02000488
	.thumb_func
Func_02000488:
	push {lr}
	ldr r3, .L_020084ac
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020084b0
	cmp r2, r3
	bne .L_020084a0
	ldr r0, .L_020084b4
	b .L_020084aa
.L_020084a0:
	ldr r3, .L_020084b8
	movs r0, #0
	cmp r2, r3
	bne .L_020084aa
	ldr r0, .L_020084bc
.L_020084aa:
	pop {pc}
.L_020084ac:
	.4byte gPartyState
.L_020084b0:
	.4byte 0x000000d3
.L_020084b4:
	.4byte Data_020033d0
.L_020084b8:
	.4byte 0x000000d4
.L_020084bc:
	.4byte Data_020033f0
	.section .text.x020084c8,"ax",%progbits
	.global Func_020004c8
	.thumb_func
Func_020004c8:
	push {lr}
	ldr r3, .L_02008514
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008518
	cmp r2, r3
	beq .L_0200850e
	ldr r3, .L_0200851c
	cmp r2, r3
	bne .L_020084e6
	ldr r0, .L_02008520
	b .L_02008510
.L_020084e6:
	ldr r3, .L_02008524
	cmp r2, r3
	bne .L_020084f0
	ldr r0, .L_02008528
	b .L_02008510
.L_020084f0:
	ldr r3, .L_0200852c
	cmp r2, r3
	bne .L_020084fa
	ldr r0, .L_02008530
	b .L_02008510
.L_020084fa:
	ldr r3, .L_02008534
	cmp r2, r3
	bne .L_02008504
	ldr r0, .L_02008538
	b .L_02008510
.L_02008504:
	ldr r3, .L_0200853c
	cmp r2, r3
	bne .L_0200850e
	ldr r0, .L_02008540
	b .L_02008510
.L_0200850e:
	ldr r0, .L_02008544
.L_02008510:
	pop {pc}
	.2byte 0x0000
.L_02008514:
	.4byte gPartyState
.L_02008518:
	.4byte 0x000000cf
.L_0200851c:
	.4byte 0x000000d0
.L_02008520:
	.4byte Data_02003648
.L_02008524:
	.4byte 0x000000d1
.L_02008528:
	.4byte Data_02003660
.L_0200852c:
	.4byte 0x000000d2
.L_02008530:
	.4byte Data_02003690
.L_02008534:
	.4byte 0x000000d3
.L_02008538:
	.4byte Data_02003708
.L_0200853c:
	.4byte 0x000000d4
.L_02008540:
	.4byte Data_020037c8
.L_02008544:
	.4byte Data_02003618
	.section .text.x02008548,"ax",%progbits
	.global Func_02000548
	.thumb_func
Func_02000548:
	push {lr}
	sub sp, #8
	bl Func_02003048
	movs r3, #24
	movs r2, #46
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #19
	movs r1, #55
	movs r2, #7
	movs r3, #8
	bl Func_02002fe8
	movs r3, #7
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #102
	movs r2, #24
	movs r3, #110
	movs r0, #24
	bl Func_02002fc0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #66
	bl GameFlag_SetBit
	bl Func_02003050
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x0200858c,"ax",%progbits
	.global Func_0200058c
	.thumb_func
Func_0200058c:
	push {lr}
	sub sp, #8
	bl Func_02003048
	movs r3, #32
	movs r2, #46
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #37
	movs r1, #55
	movs r2, #7
	movs r3, #8
	bl Func_02002fe8
	movs r3, #7
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #102
	movs r2, #32
	movs r3, #110
	movs r0, #32
	bl Func_02002fc0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #67
	bl GameFlag_SetBit
	bl Func_02003050
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x020085d0,"ax",%progbits
	.global Func_020005d0
	.thumb_func
Func_020005d0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	sub sp, #8
	bl Func_02003048
	movs r0, #0
	bl Func_02003148
	movs r2, #170
	lsls r2, r2, #1
	adds r7, r5, r2
	movs r2, #0
	ldrsh r3, [r7, r2]
	movs r6, #37
	cmp r3, #3
	bne .L_020085fa
	movs r6, #25
.L_020085fa:
	movs r0, #158
	bl Func_020031a8
	movs r3, #1
	str r3, [sp, #0]
	mov r8, r3
	adds r2, r6, #0
	movs r5, #2
	movs r1, #38
	movs r3, #49
	movs r0, #30
	str r5, [sp, #4]
	bl Func_02002fc0
	movs r0, #10
	bl Battle_WaitMode0
	mov r2, r8
	str r2, [sp, #0]
	movs r1, #38
	adds r2, r6, #0
	movs r3, #49
	movs r0, #32
	str r5, [sp, #4]
	bl Func_02002fc0
	movs r0, #10
	bl Battle_WaitMode0
	ldr r6, .L_020086d0
	movs r3, #133
	lsls r3, r3, #2
	adds r6, r6, r3
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r6]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r0, [r6]
	bl Object_GetById
	ldr r5, [r0, #8]
	mov r2, r8
	ldr r0, [r6]
	asrs r5, r5, #19
	orrs r5, r2
	bl Object_GetById
	movs r3, #18
	ldrsh r2, [r0, r3]
	lsls r5, r5, #3
	mov r3, r8
	orrs r2, r3
	adds r1, r5, #0
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #123
	bl Func_020031a8
	ldr r0, [r6]
	bl Object_GetById
	ldr r5, [r0, #8]
	ldr r0, [r6]
	bl Object_GetById
	movs r3, #18
	ldrsh r2, [r0, r3]
	asrs r5, r5, #19
	mov r3, r8
	orrs r2, r3
	lsls r5, r5, #3
	subs r2, #16
	adds r1, r5, #0
	ldr r0, [r6]
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #5
	bl Battle_WaitMode0
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	ldrsh r0, [r7, r2]
	bl Func_020030e8
	bl Func_02003050
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020086d0:
	.4byte gPartyState
	.section .text.x020086d4,"ax",%progbits
	.global Func_020006d4
	.thumb_func
Func_020006d4:
	ldr r3, [r0, #8]
	ldr r2, [r0, #68]
	ldr r1, [r0, #72]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r3, [r0, #12]
	ldr r2, [r0, #76]
	adds r3, r3, r1
	str r3, [r0, #12]
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
	ldr r3, .L_02008704
	adds r1, r1, r3
	str r1, [r0, #72]
	bx lr
.L_02008704:
	.4byte 0xffffb334
	.section .text.x02008708,"ax",%progbits
	.global Func_02000708
	.thumb_func
Func_02000708:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #60
	add r7, sp, #20
	movs r3, #1
	str r1, [sp, #16]
	str r3, [r7]
	movs r3, #24
	adds r3, #255
	strh r3, [r7, #24]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r7, #20]
	str r3, [r7, #16]
	ldr r3, .L_0200878c
	str r2, [r7, #12]
	str r2, [r7, #8]
	str r3, [r7, #36]
	ldr r3, [sp, #16]
	movs r2, #0
	mov r11, r0
	mov r9, r2
	cmp r3, #0
	beq .L_020087cc
.L_02008744:
	bl Random16Far
	ldr r3, .L_02008788
	ands r0, r3
	lsls r0, r0, #12
	strh r0, [r7, #32]
	bl Random16Far
	mov r8, r0
	movs r2, #15
	mov r3, r8
	ands r3, r2
	mov r10, r2
	mov r8, r3
	subs r2, #23
	add r8, r2
	mov r3, r8
	lsls r3, r3, #14
	mov r8, r3
	bl Random16Far
	mov r2, r10
	adds r5, r0, #0
	ands r5, r2
	bl Random16Far
	mov r2, r11
	ldr r6, [r2]
	movs r3, #31
	ands r3, r0
	lsls r3, r3, #16
	subs r6, r6, r3
	b .L_02008790
	.2byte 0x0000
.L_02008788:
	.4byte 0x0000000f
.L_0200878c:
	.4byte Func_020006d4
.L_02008790:
	movs r3, #240
	lsls r3, r3, #12
	adds r6, r6, r3
	bl Random16Far
	mov r3, r11
	mov r2, r10
	ldr r1, [r3, #4]
	ands r0, r2
	ldr r2, [r3, #8]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #175
	lsls r0, r0, #16
	lsls r3, r3, #17
	adds r5, #8
	adds r1, r1, r0
	str r3, [sp, #8]
	lsls r5, r5, #14
	mov r3, r8
	adds r0, r6, #0
	str r5, [sp, #0]
	str r7, [sp, #12]
	bl Func_020000b8
	ldr r3, [sp, #16]
	movs r2, #1
	add r9, r2
	cmp r9, r3
	bne .L_02008744
.L_020087cc:
	add sp, #60
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020087dc,"ax",%progbits
	.global Func_020007dc
	.thumb_func
Func_020007dc:
	push {lr}
	sub sp, #20
	cmp r0, #2
	beq .L_020087ee
	cmp r0, #2
	ble .L_02008838
	cmp r0, #3
	beq .L_0200880c
	b .L_02008838
.L_020087ee:
	ldr r3, .L_0200883c
	add r0, sp, #8
	str r3, [r0]
	movs r3, #0
	str r3, [r0, #4]
	movs r2, #204
	movs r3, #188
	lsls r3, r3, #17
	lsls r2, r2, #8
	str r3, [r0, #8]
	adds r2, #204
	movs r1, #1
	bl Func_02000708
	b .L_02008838
.L_0200880c:
	movs r3, #4
	str r0, [sp, #0]
	str r3, [sp, #4]
	movs r0, #31
	movs r1, #85
	movs r2, #20
	movs r3, #85
	bl Func_02002fc0
	ldr r3, .L_0200883c
	add r0, sp, #8
	str r3, [r0]
	movs r3, #0
	str r3, [r0, #4]
	movs r3, #188
	lsls r3, r3, #17
	movs r2, #192
	str r3, [r0, #8]
	lsls r2, r2, #9
	movs r1, #8
	bl Func_02000708
.L_02008838:
	add sp, #20
	pop {pc}
.L_0200883c:
	.4byte 0x015b0000
	.section .text.x02008840,"ax",%progbits
	.global Func_02000840
	.thumb_func
Func_02000840:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_020088a4
	movs r6, #7
	ldr r7, [r3]
	sub sp, #56
	ands r7, r6
	mov r8, r0
	cmp r7, #0
	bne .L_0200889a
	bl Random16Far
	movs r5, #15
	ands r5, r0
	bl Random16Far
	movs r3, #209
	lsls r3, r3, #1
	ands r0, r6
	adds r3, #255
	add r6, sp, #16
	strh r3, [r6, #24]
	mov r3, r8
	ldr r4, [r3, #8]
	ldr r1, [r3, #12]
	ldr r2, [r3, #16]
	movs r3, #128
	lsls r3, r3, #8
	subs r5, #8
	lsls r5, r5, #16
	subs r0, #8
	str r3, [sp, #0]
	movs r3, #128
	lsls r0, r0, #16
	lsls r3, r3, #13
	adds r4, r4, r5
	adds r1, r1, r0
	str r3, [sp, #8]
	adds r0, r4, #0
	movs r3, #0
	str r7, [sp, #4]
	str r6, [sp, #12]
	bl Func_020000b8
.L_0200889a:
	add sp, #56
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020088a4:
	.4byte gFrameCount
	.section .text.x020088a8,"ax",%progbits
	.global Func_020008a8
	.thumb_func
Func_020008a8:
	push {r5, lr}
	movs r0, #8
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02003048
	movs r0, #0
	bl Func_02003148
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #68
	bl GameFlag_SetBit
	movs r3, #20
	movs r2, #22
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #22
	movs r0, #11
	movs r2, #3
	movs r3, #2
	bl Func_02002fe8
	movs r3, #128
	lsls r3, r3, #8
	adds r2, r5, #0
	str r3, [r5, #72]
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	adds r0, r5, #0
	bl Func_02000298
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02003088
	movs r1, #172
	movs r2, #186
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #68
	bl Func_02003088
	movs r0, #68
	bl Object_GetById
	ldr r3, .L_0200891c
	str r3, [r0, #108]
	bl Func_02003050
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_0200891c:
	.4byte Func_02000840
	.section .text.x02008920,"ax",%progbits
	.global Func_02000920
	.thumb_func
Func_02000920:
	push {lr}
	bl Func_02003048
	movs r0, #0
	bl Func_02003148
	ldr r0, .L_02008958
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #73
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008952
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #173
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
.L_02008952:
	bl Func_02003050
	pop {pc}
.L_02008958:
	.4byte 0x00001a95
	.section .text.x0200895c,"ax",%progbits
	.global Func_0200095c
	.thumb_func
Func_0200095c:
	push {lr}
	bl Func_0200189c
	pop {pc}
	.section .text.x02008964,"ax",%progbits
	.global Func_02000964
	.thumb_func
Func_02000964:
	push {r5, lr}
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02003048
	movs r0, #0
	bl Func_02003148
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02003110
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02003108
	movs r0, #2
	bl Func_02003118
	movs r0, #132
	bl Func_020031a8
	movs r0, #160
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02003000
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r1, r1
	negs r0, r0
	bl Func_02003000
	movs r0, #2
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02003108
	movs r0, #2
	bl Func_02003118
	adds r2, r5, #0
	movs r3, #0
	adds r2, #90
	strb r3, [r2]
	movs r1, #212
	movs r2, #134
	str r3, [r5, #108]
	lsls r2, r2, #2
	movs r0, #8
	lsls r1, r1, #1
	bl ObjectMotion_ResetAndSetPosition
	ldr r1, .L_02008a40
	adds r0, r5, #0
	bl Func_02002f90
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	ldr r3, .L_02008a44
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r5, #200
	ldr r1, [r3]
	lsls r5, r5, #6
	movs r0, #8
	bl Object_LinkObjectAndSetCallback
	adds r3, r5, #0
	movs r1, #21
	movs r2, #33
	movs r0, #0
	bl Func_020002f0
	adds r3, r5, #0
	movs r1, #22
	movs r2, #33
	movs r0, #0
	bl Func_020002f0
	movs r3, #255
	lsls r3, r3, #8
	movs r1, #26
	movs r2, #33
	movs r0, #0
	bl Func_020002f0
	bl Func_02003050
	pop {r5, pc}
.L_02008a40:
	.4byte Data_02003370
.L_02008a44:
	.4byte gPartyState
	.section .text.x02008a48,"ax",%progbits
	.global Func_02000a48
	.thumb_func
Func_02000a48:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	adds r6, r1, #0
	mov r10, r0
	movs r1, #26
	ldrsh r0, [r3, r1]
	sub sp, #56
	mov r9, r3
	bl Object_GetById
	adds r7, r0, #0
	cmp r6, #7
	bgt .L_02008adc
	movs r3, #24
	add r5, sp, #16
	adds r3, #255
	strh r3, [r5, #24]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #12]
	str r3, [r5, #8]
	movs r3, #128
	movs r2, #0
	lsls r3, r3, #8
	mov r8, r2
	str r2, [r5]
	str r3, [r5, #20]
	str r3, [r5, #16]
	bl Random16Far
	ldr r3, .L_02008ad0
	ands r0, r3
	lsls r0, r0, #12
	strh r0, [r5, #32]
	bl Random16Far
	ldr r1, [r7, #12]
	lsls r2, r6, #1
	adds r2, r2, r6
	lsls r2, r2, #16
	movs r4, #128
	subs r1, r1, r2
	lsls r4, r4, #13
	movs r3, #15
	adds r1, r1, r4
	ands r3, r0
	mov r4, r8
	ldr r0, [r7, #8]
	ldr r2, [r7, #16]
	subs r3, #8
	str r4, [sp, #0]
	str r4, [sp, #4]
	movs r4, #180
	lsls r3, r3, #13
	lsls r4, r4, #15
	str r4, [sp, #8]
	str r5, [sp, #12]
	bl Func_020000b8
	b .L_02008ad4
	.2byte 0x0000
.L_02008ad0:
	.4byte 0x0000000f
.L_02008ad4:
	ldr r3, [r7, #28]
	ldr r1, .L_02008b30
	adds r3, r3, r1
	str r3, [r7, #28]
.L_02008adc:
	mov r2, r10
	cmp r2, #1
	bne .L_02008b22
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #28]
	adds r3, r7, #0
	adds r3, #100
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl GameFlag_SetBit
	mov r2, r9
	movs r1, #26
	ldrsh r0, [r2, r1]
	bl Object_GetById
	bl Func_02000338
	ldr r2, [r7, #8]
	ldr r3, [r7, #16]
	asrs r2, r2, #20
	mov r4, r10
	asrs r3, r3, #20
	adds r2, #64
	movs r0, #67
	movs r1, #23
	str r4, [sp, #0]
	str r4, [sp, #4]
	bl Func_02002fc0
	movs r3, #0
	str r3, [r7, #16]
	str r3, [r7, #12]
	str r3, [r7, #8]
.L_02008b22:
	add sp, #56
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008b30:
	.4byte 0xffffe100
	.section .text.x02008b34,"ax",%progbits
	.global Func_02000b34
	.thumb_func
Func_02000b34:
	push {lr}
	bl Func_02000a48
	pop {pc}
	.section .text.x02008b3c,"ax",%progbits
	.global Func_02000b3c
	.thumb_func
Func_02000b3c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r5, .L_02008bf8
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003048
	movs r0, #0
	bl Func_02003148
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r1, r1
	negs r0, r0
	bl Motion_CamBounds
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #0
	mov r8, r3
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #22
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_020030c8
	ldr r0, [r5]
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r6, #6]
	movs r1, #5
	ldr r0, [r5]
	bl Object_SetModeById
	ldr r0, [r5]
	movs r1, #24
	bl Object_SetActionById
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #152
	movs r1, #228
	movs r3, #12
	lsls r1, r1, #18
	movs r2, #0
	negs r3, r3
	lsls r0, r0, #17
	bl Func_02003030
	ldr r0, [r5]
	bl Object_GetById
	mov r3, r8
	str r3, [r0, #68]
	movs r0, #11
	bl Func_020030e8
	bl Func_02003050
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02008bf8:
	.4byte gPartyState
	.section .text.x02008bfc,"ax",%progbits
	.global Func_02000bfc
	.thumb_func
Func_02000bfc:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	ldr r3, .L_02008c40
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008c44
	cmp r2, r3
	bne .L_02008c4c
	ldr r0, .L_02008c48
	bl Func_02003170
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #188
	adds r3, r5, r2
	ldr r0, [r3]
	ldr r1, .L_02008c3c
	ldr r2, [r0, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	lsls r2, r2, #8
	ands r3, r1
	orrs r2, r3
	adds r0, #100
	strh r2, [r0]
	b .L_02008c58
.L_02008c3c:
	.4byte 0x000000ff
.L_02008c40:
	.4byte gPartyState
.L_02008c44:
	.4byte 0x000000d3
.L_02008c48:
	.4byte Data_02003264
.L_02008c4c:
	ldr r3, .L_02008c5c
	cmp r2, r3
	bne .L_02008c58
	ldr r0, .L_02008c60
	bl Func_02003170
.L_02008c58:
	pop {r5, pc}
	.2byte 0x0000
.L_02008c5c:
	.4byte 0x000000d4
.L_02008c60:
	.4byte Data_0200326e
	.section .text.x02008c64,"ax",%progbits
	.global Func_02000c64
	.thumb_func
Func_02000c64:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #188
	lsls r1, r1, #16
	adds r3, r3, r2
	asrs r1, r1, #16
	ldr r5, [r3]
	mov r8, r1
	bl Func_02003048
	movs r0, #0
	bl Func_02003148
	adds r3, r5, #0
	adds r3, #100
	ldrh r1, [r3]
	ldr r2, [r5, #8]
	lsls r3, r1, #16
	asrs r2, r2, #20
	asrs r3, r3, #24
	subs r7, r2, r3
	ldr r2, [r5, #16]
	movs r3, #255
	ands r3, r1
	asrs r2, r2, #20
	subs r6, r2, r3
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #48]
	b .L_02008cd8
.L_02008cac:
	ldr r3, [r5, #8]
	ldr r0, [r5, #16]
	lsls r1, r7, #20
	adds r1, r1, r3
	lsls r3, r6, #20
	ldr r2, [r5, #12]
	adds r3, r3, r0
	adds r0, r5, #0
	bl Func_02002fb0
	mov r3, r8
	lsls r0, r3, #16
	lsrs r0, r0, #16
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	adds r0, r5, #0
	bl Func_02002fb8
	movs r0, #1
	bl WaitFrames
.L_02008cd8:
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	adds r1, r1, r7
	adds r2, r2, r6
	movs r0, #1
	bl Func_020002b4
	asrs r0, r0, #8
	cmp r0, #50
	bne .L_02008d08
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	adds r1, r1, r7
	adds r2, r2, r6
	movs r0, #2
	bl Func_020002b4
	asrs r0, r0, #8
	cmp r0, #255
	bne .L_02008cac
.L_02008d08:
	ldr r3, [r5, #8]
	movs r2, #1
	asrs r3, r3, #19
	orrs r3, r2
	lsls r3, r3, #19
	str r3, [r5, #8]
	ldr r3, [r5, #16]
	asrs r3, r3, #19
	orrs r3, r2
	lsls r3, r3, #19
	str r3, [r5, #16]
	bl Func_02003178
	bl Func_02003050
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x02008d2c,"ax",%progbits
	.global Func_02000d2c
	.thumb_func
Func_02000d2c:
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
	ldr r6, [r3]
	bl Func_02003048
	movs r0, #0
	bl Func_02003148
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	movs r0, #2
	bl Map_GetTerrainHeight
	ldr r3, [r6, #12]
	cmp r0, r3
	beq .L_02008d88
	movs r2, #34
	adds r2, r2, r6
	movs r3, #2
	adds r7, r6, #0
	strb r3, [r2]
	adds r7, #85
	movs r3, #3
	strb r3, [r7]
	adds r0, r6, #0
	mov r8, r2
	bl Func_02000298
	movs r0, #188
	bl Func_020031a8
	adds r0, r6, #0
	bl Func_02000298
	movs r5, #0
	mov r3, r8
	strb r5, [r7]
	strb r5, [r3]
.L_02008d88:
	bl Func_02003178
	ldr r3, .L_02008ddc
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008de0
	cmp r2, r3
	bne .L_02008dd0
	ldr r3, [r6, #8]
	asrs r3, r3, #19
	cmp r3, #27
	bne .L_02008db0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #71
	bl GameFlag_SetBit
.L_02008db0:
	ldr r3, [r6, #8]
	asrs r3, r3, #19
	cmp r3, #25
	bne .L_02008dd0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #71
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008dd0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #70
	bl GameFlag_SetBit
.L_02008dd0:
	bl Func_02003050
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008ddc:
	.4byte gPartyState
.L_02008de0:
	.4byte 0x000000d3
	.section .text.x02008de4,"ax",%progbits
	.global Func_02000de4
	.thumb_func
Func_02000de4:
	push {r5, lr}
	sub sp, #8
	bl Func_02002fc8
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl GameFlag_ClearBit
	ldr r5, .L_02008e74
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #6
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_02008e70
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #39
	movs r1, #100
	movs r2, #53
	movs r3, #100
	bl Func_02002fc0
	movs r3, #53
	movs r2, #37
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #43
	movs r1, #41
	movs r2, #3
	movs r3, #3
	bl Func_02002fe8
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #38
	bne .L_02008e70
	ldr r0, [r5]
	bl Object_GetById
	movs r2, #6
	ldrsh r3, [r0, r2]
	ldr r0, [r5]
	cmp r3, #0
	bge .L_02008e64
	movs r1, #218
	movs r2, #150
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02003088
	b .L_02008e70
.L_02008e64:
	movs r1, #218
	movs r2, #158
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02003088
.L_02008e70:
	add sp, #8
	pop {r5, pc}
.L_02008e74:
	.4byte gPartyState
	.section .text.x02008e78,"ax",%progbits
	.global Func_02000e78
	.thumb_func
Func_02000e78:
	push {r5, lr}
	sub sp, #8
	bl Func_02002fd0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl GameFlag_SetBit
	ldr r5, .L_02008efc
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #6
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_02008ef8
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #43
	movs r1, #100
	movs r2, #53
	movs r3, #100
	bl Func_02002fc0
	movs r3, #53
	movs r2, #37
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #43
	movs r1, #37
	movs r2, #3
	movs r3, #3
	bl Func_02002fe8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #38
	ble .L_02008eea
	movs r3, #248
	lsls r3, r3, #5
	movs r0, #0
	movs r1, #54
	movs r2, #37
	bl Func_020002f0
	b .L_02008ef8
.L_02008eea:
	movs r3, #248
	lsls r3, r3, #5
	movs r0, #0
	movs r1, #54
	movs r2, #39
	bl Func_020002f0
.L_02008ef8:
	add sp, #8
	pop {r5, pc}
.L_02008efc:
	.4byte gPartyState
	.section .text.x02008f00,"ax",%progbits
	.global Func_02000f00
	.thumb_func
Func_02000f00:
	push {lr}
	movs r0, #0
	bl Func_02003148
	pop {pc}
	.2byte 0x0000
	.section .text.x02008f0c,"ax",%progbits
	.global Func_02000f0c
	.thumb_func
Func_02000f0c:
	push {r5, lr}
	sub sp, #20
	cmp r0, #2
	beq .L_02008f6a
	cmp r0, #2
	bgt .L_02008f1e
	cmp r0, #1
	beq .L_02008f24
	b .L_02008fa6
.L_02008f1e:
	cmp r0, #3
	beq .L_02008f8a
	b .L_02008fa6
.L_02008f24:
	movs r3, #4
	str r3, [sp, #4]
	movs r5, #3
	movs r0, #18
	movs r1, #97
	movs r2, #6
	movs r3, #97
	str r5, [sp, #0]
	bl Func_02002fc0
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #82
	movs r1, #34
	movs r2, #70
	movs r3, #34
	str r5, [sp, #0]
	bl Func_02002fc0
	movs r3, #7
	movs r2, #35
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #7
	movs r1, #36
	movs r2, #1
	movs r3, #1
	bl Func_02002fe8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #75
	bl GameFlag_SetBit
	b .L_02008fa6
.L_02008f6a:
	movs r3, #240
	add r0, sp, #8
	lsls r3, r3, #15
	str r3, [r0]
	movs r3, #0
	str r3, [r0, #4]
	movs r2, #204
	movs r3, #142
	lsls r3, r3, #18
	lsls r2, r2, #8
	str r3, [r0, #8]
	adds r2, #204
	movs r1, #1
	bl Func_02000708
	b .L_02008fa6
.L_02008f8a:
	movs r3, #240
	add r0, sp, #8
	lsls r3, r3, #15
	str r3, [r0]
	movs r3, #0
	str r3, [r0, #4]
	movs r3, #142
	lsls r3, r3, #18
	movs r2, #192
	str r3, [r0, #8]
	lsls r2, r2, #9
	movs r1, #8
	bl Func_02000708
.L_02008fa6:
	add sp, #20
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008fac,"ax",%progbits
	.global Func_02000fac
	.thumb_func
Func_02000fac:
	push {lr}
	movs r0, #0
	bl Func_02003148
	pop {pc}
	.2byte 0x0000
	.section .text.x02008fb8,"ax",%progbits
	.global Func_02000fb8
	.thumb_func
Func_02000fb8:
	push {r5, r6, lr}
	adds r6, r1, #0
	sub sp, #8
	adds r5, r0, #0
	cmp r6, #0
	bne .L_02008fce
	bl Func_02002e00
	ldr r0, .L_02009040
	bl Scheduler_RemoveCallbackFar
.L_02008fce:
	cmp r5, #1
	bne .L_02009002
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #13
	movs r1, #26
	movs r2, #13
	movs r3, #16
	bl Func_02002fc0
	movs r3, #13
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #13
	movs r1, #26
	movs r2, #3
	movs r3, #3
	bl Func_02002fe8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #72
	bl GameFlag_SetBit
.L_02009002:
	cmp r5, #2
	bne .L_02009018
	movs r0, #232
	movs r1, #128
	movs r2, #140
	lsls r0, r0, #16
	lsls r1, r1, #13
	lsls r2, r2, #17
	movs r3, #2
	bl Func_02002d44
.L_02009018:
	cmp r5, #3
	bne .L_0200902e
	movs r0, #232
	movs r1, #128
	movs r2, #140
	lsls r0, r0, #16
	lsls r1, r1, #13
	lsls r2, r2, #17
	movs r3, #30
	bl Func_02002d44
.L_0200902e:
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	cmp r6, r3
	bne .L_0200903c
	bl Func_02002ebc
.L_0200903c:
	add sp, #8
	pop {r5, r6, pc}
.L_02009040:
	.4byte Func_0200137c
	.section .text.x02009044,"ax",%progbits
	.global Func_02001044
	.thumb_func
Func_02001044:
	push {lr}
	ldr r3, .L_02009098
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200909c
	cmp r2, r3
	bne .L_0200905c
	ldr r0, .L_020090a0
	b .L_02009096
.L_0200905c:
	ldr r3, .L_020090a4
	cmp r2, r3
	beq .L_02009094
	ldr r3, .L_020090a8
	cmp r2, r3
	bne .L_0200906c
	ldr r0, .L_020090ac
	b .L_02009096
.L_0200906c:
	ldr r3, .L_020090b0
	cmp r2, r3
	bne .L_02009076
	ldr r0, .L_020090b4
	b .L_02009096
.L_02009076:
	ldr r3, .L_020090b8
	cmp r2, r3
	bne .L_02009080
	ldr r0, .L_020090bc
	b .L_02009096
.L_02009080:
	ldr r3, .L_020090c0
	cmp r2, r3
	bne .L_0200908a
	ldr r0, .L_020090c4
	b .L_02009096
.L_0200908a:
	ldr r3, .L_020090c8
	cmp r2, r3
	bne .L_02009094
	ldr r0, .L_020090cc
	b .L_02009096
.L_02009094:
	ldr r0, .L_020090d0
.L_02009096:
	pop {pc}
.L_02009098:
	.4byte gPartyState
.L_0200909c:
	.4byte 0x000000ce
.L_020090a0:
	.4byte Data_020037f8
.L_020090a4:
	.4byte 0x000000cf
.L_020090a8:
	.4byte 0x000000d0
.L_020090ac:
	.4byte Data_0200393c
.L_020090b0:
	.4byte 0x000000d1
.L_020090b4:
	.4byte Data_020039e4
.L_020090b8:
	.4byte 0x000000d2
.L_020090bc:
	.4byte Data_02003a5c
.L_020090c0:
	.4byte 0x000000d3
.L_020090c4:
	.4byte Data_02003b28
.L_020090c8:
	.4byte 0x000000d4
.L_020090cc:
	.4byte Data_02003c90
.L_020090d0:
	.4byte Data_0200381c
	.section .text.x020090d4,"ax",%progbits
	.global Func_020010d4
	.thumb_func
Func_020010d4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009120
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #80]
	ldr r2, [r6, #16]
	mov r8, r3
	ldr r3, [r5, #76]
	ldr r7, [r5, #80]
	cmp r2, r3
	bgt .L_02009124
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #12
	movs r3, #0
	ldrh r2, [r1]
	str r3, [r5, #16]
	str r3, [r5, #8]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #252
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_0200911c
	ldrh r3, [r1]
	orrs r3, r2
	strh r3, [r1]
	b .L_02009182
.L_0200911c:
	.4byte 0x00000001
.L_02009120:
	.4byte gPartyState
.L_02009124:
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #12
	ldrh r2, [r1]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #252
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_02009154
	ldrh r3, [r1]
	movs r0, #128
	orrs r3, r2
	lsls r0, r0, #2
	strh r3, [r1]
	adds r0, #18
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009158
	movs r3, #0
	str r3, [r5, #16]
	str r3, [r5, #8]
	b .L_02009182
.L_02009154:
	.4byte 0x00000002
.L_02009158:
	ldr r3, [r6, #8]
	ldr r1, [r7, #40]
	str r3, [r5, #8]
	ldr r3, [r6, #12]
	str r3, [r5, #12]
	ldr r3, [r5, #76]
	ldr r2, [r6, #16]
	subs r2, r2, r3
	subs r3, r3, r2
	str r3, [r5, #16]
	ldrh r3, [r6, #6]
	mvns r3, r3
	strh r3, [r5, #6]
	mov r3, r8
	ldr r2, [r3, #40]
	ldr r3, [r2, #16]
	str r3, [r1, #16]
	ldrh r3, [r2, #2]
	strh r3, [r1, #2]
	ldrb r3, [r2, #20]
	strb r3, [r1, #20]
.L_02009182:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x02009188,"ax",%progbits
	.global Func_02001188
	.thumb_func
Func_02001188:
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
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_020091b8
	adds r3, #15
.L_020091b8:
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
	.section .text.x020091e0,"ax",%progbits
	.global Func_020011e0
	.thumb_func
Func_020011e0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009368
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02003048
	movs r0, #0
	bl Func_02003148
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02002fa8
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
	bl Func_020031a8
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200936c
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200927a:
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
	ldr r3, .L_02009370
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_02009374
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
	ldr r4, .L_02009378
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_020000b8
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200927a
	movs r0, #188
	bl Func_020031a8
	ldr r5, .L_02009368
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_020030c8
	ldr r0, [r5]
	movs r1, #22
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02003000
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02003000
	bl Func_02003008
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_020030c8
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
	bl Func_02003050
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009368:
	.4byte gPartyState
.L_0200936c:
	.4byte Func_02001188
.L_02009370:
	.4byte 0xffffa000
.L_02009374:
	.4byte 0xffffd000
.L_02009378:
	.4byte 0x01090001
	.section .text.x0200937c,"ax",%progbits
	.global Func_0200137c
	.thumb_func
Func_0200137c:
	push {lr}
	ldr r3, .L_020093b4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	asrs r3, r3, #19
	cmp r3, #0
	beq .L_020093a2
	movs r0, #0
	movs r1, #14
	movs r2, #17
	movs r3, #0
	bl Func_020002f0
	b .L_020093b0
.L_020093a2:
	movs r3, #200
	lsls r3, r3, #6
	movs r0, #0
	movs r1, #14
	movs r2, #17
	bl Func_020002f0
.L_020093b0:
	pop {pc}
	.2byte 0x0000
.L_020093b4:
	.4byte gPartyState
	.section .text.x020093b8,"ax",%progbits
	.global Func_020013b8
	.thumb_func
Func_020013b8:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	ldr r1, .L_0200973c
	str r2, [r3]
	subs r2, #36
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009740
	sub sp, #8
	cmp r2, r3
	bne .L_020093e0
	b .L_020097c4
.L_020093e0:
	ldr r3, .L_02009744
	cmp r2, r3
	beq .L_020093e8
	b .L_0200950a
.L_020093e8:
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #1
	bge .L_020093f8
	b .L_020097c4
.L_020093f8:
	cmp r3, #4
	ble .L_02009402
	cmp r3, #11
	beq .L_0200942a
	b .L_020097c4
.L_02009402:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #66
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009414
	bl Func_02000548
.L_02009414:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #67
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009424
	b .L_020097c4
.L_02009424:
	bl Func_0200058c
	b .L_020097c4
.L_0200942a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #69
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020094c6
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r5, #0
	adds r3, #89
	strb r6, [r3]
	subs r3, #4
	strb r6, [r3]
	movs r3, #128
	lsls r3, r3, #12
	movs r1, #186
	adds r0, r5, #0
	str r3, [r5, #20]
	str r3, [r5, #12]
	adds r1, #255
	bl Func_02003020
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #68
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200947a
	b .L_020097c4
.L_0200947a:
	movs r3, #3
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #31
	movs r1, #85
	movs r2, #20
	movs r3, #85
	bl Func_02002fc0
	movs r3, #20
	movs r2, #22
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #2
	movs r0, #11
	movs r1, #22
	movs r2, #3
	bl Func_02002fe8
	movs r1, #172
	movs r2, #186
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #68
	bl Func_02003088
	movs r0, #68
	bl Object_GetById
	ldr r3, .L_02009748
	movs r1, #0
	str r3, [r0, #108]
	movs r2, #0
	movs r0, #8
	bl Func_02003088
	b .L_020097c4
.L_020094c6:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #68
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020094d6
	b .L_020097c4
.L_020094d6:
	movs r3, #3
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #31
	movs r1, #85
	movs r2, #20
	movs r3, #85
	bl Func_02002fc0
	movs r3, #20
	movs r2, #22
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #11
	movs r1, #22
	movs r2, #3
	movs r3, #2
	bl Func_02002fe8
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02003088
	b .L_020097c4
.L_0200950a:
	ldr r3, .L_0200974c
	cmp r2, r3
	bne .L_020095ba
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #7
	ble .L_02009520
	b .L_020097c4
.L_02009520:
	cmp r3, #6
	bge .L_02009526
	b .L_020097c4
.L_02009526:
	movs r0, #100
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009594
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009568
	movs r1, #172
	movs r2, #134
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02003088
	movs r3, #204
	lsls r3, r3, #6
	movs r1, #21
	movs r2, #33
	movs r0, #0
	bl Func_020002f0
	movs r3, #255
	lsls r3, r3, #8
	movs r0, #0
	movs r1, #22
	movs r2, #33
	bl Func_020002f0
	b .L_02009584
.L_02009568:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009584
	movs r3, #255
	lsls r3, r3, #8
	movs r0, #0
	movs r1, #26
	movs r2, #33
	bl Func_020002f0
.L_02009584:
	ldr r3, .L_0200973c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #8
	bl Object_LinkObjectAndSetCallback
.L_02009594:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020095a2
	b .L_020097c4
.L_020095a2:
	ldr r3, .L_0200973c
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #7
	beq .L_020095b4
	b .L_020097c4
.L_020095b4:
	bl Func_020011e0
	b .L_020097c4
.L_020095ba:
	ldr r3, .L_02009750
	cmp r2, r3
	bne .L_020095c8
	ldr r0, .L_02009754
	bl Func_02000404
	b .L_020097c4
.L_020095c8:
	ldr r3, .L_02009758
	cmp r2, r3
	beq .L_020095d0
	b .L_02009770
.L_020095d0:
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #9
	bls .L_020095e2
	b .L_020097c4
.L_020095e2:
	ldr r2, .L_0200975c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_020095ec:
	.4byte .L_02009656
	.4byte .L_02009656
	.4byte .L_02009614
	.4byte .L_020096a4
	.4byte .L_020096a4
	.4byte .L_020096fe
	.4byte .L_020096fe
	.4byte .L_020097c4
	.4byte .L_020097c4
	.4byte .L_02009728
.L_02009614:
	movs r0, #143
	movs r1, #236
	movs r3, #200
	lsls r0, r0, #1
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #16
	bl Func_02002f98
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200962e
	b .L_020097c4
.L_0200962e:
	movs r1, #1
	bl Object_SetSpritePriority
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
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
	b .L_020097c4
.L_02009656:
	ldr r0, .L_02009760
	bl Func_02000404
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #71
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009694
	movs r1, #216
	movs r2, #136
	movs r0, #13
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02003088
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #70
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009694
	movs r1, #200
	movs r2, #136
	movs r0, #12
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02003088
.L_02009694:
	ldr r0, .L_02009764
	bl Func_02003168
	movs r0, #12
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	b .L_020097c4
.L_020096a4:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #75
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020096ec
	movs r3, #4
	str r3, [sp, #4]
	movs r5, #3
	movs r0, #18
	movs r1, #97
	movs r2, #6
	movs r3, #97
	str r5, [sp, #0]
	bl Func_02002fc0
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #82
	movs r1, #34
	movs r2, #70
	movs r3, #34
	str r5, [sp, #0]
	bl Func_02002fc0
	movs r3, #7
	movs r2, #35
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #7
	movs r1, #36
	movs r2, #1
	movs r3, #1
	bl Func_02002fe8
.L_020096ec:
	movs r1, #0
	movs r2, #0
	movs r3, #0
	movs r0, #4
	ldr r5, .L_02009768
	bl Func_02002f98
	movs r3, #144
	b .L_0200970e
.L_020096fe:
	movs r1, #0
	movs r2, #0
	movs r3, #0
	movs r0, #4
	ldr r5, .L_02009768
	bl Func_02002f98
	movs r3, #156
.L_0200970e:
	lsls r3, r3, #18
	str r3, [r0, #76]
	ldr r3, .L_0200976c
	adds r2, r0, #0
	str r3, [r0, #108]
	adds r2, #85
	movs r3, #0
	str r0, [r5]
	strb r3, [r2]
	movs r1, #3
	bl Object_SetSpritePriority
	b .L_020097c4
.L_02009728:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020097c4
	bl Func_020011e0
	b .L_020097c4
	.2byte 0x0000
.L_0200973c:
	.4byte gPartyState
.L_02009740:
	.4byte 0x000000ce
.L_02009744:
	.4byte 0x000000cf
.L_02009748:
	.4byte Func_02000840
.L_0200974c:
	.4byte 0x000000d1
.L_02009750:
	.4byte 0x000000d2
.L_02009754:
	.4byte Data_02003340
.L_02009758:
	.4byte 0x000000d3
.L_0200975c:
	.4byte .L_020095ec
.L_02009760:
	.4byte Data_0200335a
.L_02009764:
	.4byte Data_02003264
.L_02009768:
	.4byte Data_02003ccc
.L_0200976c:
	.4byte Func_020010d4
.L_02009770:
	ldr r3, .L_020097cc
	cmp r2, r3
	bne .L_020097c4
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #72
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020097b4
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #13
	movs r1, #26
	movs r2, #13
	movs r3, #16
	bl Func_02002fc0
	movs r3, #13
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #13
	movs r1, #26
	movs r2, #3
	movs r3, #3
	bl Func_02002fe8
	b .L_020097be
.L_020097b4:
	movs r1, #144
	ldr r0, .L_020097d0
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
.L_020097be:
	ldr r0, .L_020097d4
	bl Func_02003168
.L_020097c4:
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020097cc:
	.4byte 0x000000d4
.L_020097d0:
	.4byte Func_0200137c
.L_020097d4:
	.4byte Data_0200326e
	.section .text.x020097dc,"ax",%progbits
	.global Func_020017dc
	.thumb_func
Func_020017dc:
	push {r5, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	movs r0, #0
	ldrsh r1, [r2, r0]
	ldrh r3, [r2]
	cmp r1, #0
	beq .L_020097f4
	subs r3, #1
	strh r3, [r2]
	b .L_0200985a
.L_020097f4:
	adds r3, r5, #0
	adds r3, #90
	movs r0, #131
	strb r1, [r3]
	lsls r0, r0, #1
	bl GameFlag_Test
	movs r3, #1
	negs r3, r3
	cmp r0, #0
	bne .L_0200981a
	ldr r3, .L_0200985c
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_02009860
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
.L_0200981a:
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_0200982c
	adds r0, r5, #0
	movs r1, #9
	bl Func_02002f80
	b .L_0200985a
.L_0200982c:
	ldrh r1, [r5, #6]
	movs r2, #128
	subs r3, r3, r1
	lsls r3, r3, #16
	asrs r3, r3, #16
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_0200983e
	adds r3, r2, #0
.L_0200983e:
	ldr r2, .L_02009864
	cmp r3, r2
	bge .L_02009846
	adds r3, r2, #0
.L_02009846:
	adds r3, r1, r3
	adds r0, r5, #0
	movs r1, #2
	strh r3, [r5, #6]
	bl Func_02002f80
	adds r0, r5, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
.L_0200985a:
	pop {r5, pc}
.L_0200985c:
	.4byte gInput
.L_02009860:
	.4byte Data_02003272
.L_02009864:
	.4byte 0xfffff000
	.section .text.x02009868,"ax",%progbits
	.global Func_02001868
	.thumb_func
Func_02001868:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #162
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02009898
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Func_02003100
	bl Func_02003140
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
.L_02009898:
	pop {pc}
	.2byte 0x0000
	.section .text.x0200989c,"ax",%progbits
	.global Func_0200189c
	.thumb_func
Func_0200189c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200995c
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	adds r7, r0, #0
.L_020098bc:
	bl Func_02001868
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_02009960
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_02002fe0
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_02009964
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_02009974
	ldr r3, .L_02009968
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200996c
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_02009970
	cmp r3, r2
	bne .L_020099a0
	b .L_02009b36
.L_0200995c:
	.4byte gPartyState
.L_02009960:
	.4byte 0xfff00000
.L_02009964:
	.4byte IwramMulQ16
.L_02009968:
	.4byte gInput
.L_0200996c:
	.4byte Data_020032b2
.L_02009970:
	.4byte 0xffff0000
.L_02009974:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_0200999c
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_020099a0
.L_0200999c:
	.4byte 0xffffc000
.L_020099a0:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_02002fe0
	mov r11, r0
	cmp r0, #255
	beq .L_02009a22
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_02009a22
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r3, [sp, #8]
	ldr r2, [r7, #12]
	bl Func_02002fb0
	adds r0, r7, #0
	movs r1, #2
	bl Func_02002f80
	adds r0, r7, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	adds r0, r7, #0
	bl Func_02002fb8
	ldr r3, .L_02009b44
	str r3, [r7, #108]
	b .L_02009acc
.L_02009a22:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_02009b18
.L_02009a36:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_02009aec
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_02009a64:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02009a8e
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02009a8e
	cmp r5, r7
	beq .L_02009a8e
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02003028
	cmp r0, #0
	bge .L_02009aec
.L_02009a8e:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_02009a64
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	ldr r3, [r6, #8]
	ldr r1, [r6]
	ldr r2, [r6, #4]
	bl Func_02002fb0
	adds r0, r7, #0
	bl Func_02002fb8
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_02009b12
.L_02009acc:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_02002fe0
	mov r11, r0
	cmp r0, #255
	bne .L_02009a36
.L_02009aec:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_02002fb0
	adds r0, r7, #0
	bl Func_02002fb8
	movs r0, #2
	bl WaitFrames
	b .L_020098bc
.L_02009b12:
	movs r0, #10
	bl WaitFrames
.L_02009b18:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02002f80
.L_02009b36:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009b44:
	.4byte Func_020017dc
	.section .text.x02009b48,"ax",%progbits
	.global Func_02001b48
	.thumb_func
Func_02001b48:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009ba8
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_02009bac
	ldr r3, .L_02009ba4
	adds r7, r0, #0
	strh r3, [r2]
.L_02009b6e:
	bl Func_02001868
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_02009bb0
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	b .L_02009bb4
.L_02009ba4:
	.4byte 0x00000000
.L_02009ba8:
	.4byte gPartyState
.L_02009bac:
	.4byte Data_02003cd0
.L_02009bb0:
	.4byte 0xfff00000
.L_02009bb4:
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_02002fe0
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_02009c20
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_02009c30
	ldr r3, .L_02009c24
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_02009c28
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_02009c2c
	cmp r3, r2
	bne .L_02009c5c
	b .L_02009e26
.L_02009c20:
	.4byte IwramMulQ16
.L_02009c24:
	.4byte gInput
.L_02009c28:
	.4byte Data_020032b2
.L_02009c2c:
	.4byte 0xffff0000
.L_02009c30:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_02009c58
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_02009c5c
.L_02009c58:
	.4byte 0xffffc000
.L_02009c5c:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_02002fe0
	mov r11, r0
	cmp r0, #255
	beq .L_02009cd6
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_02009cd6
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r2, [r7, #12]
	ldr r3, [sp, #8]
	bl Func_02002fb0
	adds r0, r7, #0
	movs r1, #2
	bl Func_02002f80
	adds r0, r7, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	movs r5, #0
	b .L_02009cfe
.L_02009cd6:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_02009e26
.L_02009cea:
	ldr r3, .L_02009e54
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_02009cf6
	b .L_02009e26
.L_02009cf6:
	movs r0, #1
	bl WaitFrames
	adds r5, #1
.L_02009cfe:
	cmp r5, #179
	bgt .L_02009d0c
	adds r0, r7, #0
	bl Func_02003018
	cmp r0, #0
	beq .L_02009cea
.L_02009d0c:
	ldr r3, .L_02009e58
	str r3, [r7, #108]
	b .L_02009dda
.L_02009d12:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_02009dfa
	ldr r3, .L_02009e54
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02009e26
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_02009d4a:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02009d74
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02009d74
	cmp r5, r7
	beq .L_02009d74
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02003028
	cmp r0, #0
	bge .L_02009dfa
.L_02009d74:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_02009d4a
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	movs r5, #0
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_02002fb0
	b .L_02009db2
.L_02009daa:
	movs r0, #1
	bl WaitFrames
	adds r5, #1
.L_02009db2:
	cmp r5, #179
	bgt .L_02009dca
	adds r0, r7, #0
	bl Func_02003018
	cmp r0, #0
	bne .L_02009dca
	ldr r3, .L_02009e54
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_02009daa
.L_02009dca:
	ldr r3, .L_02009e54
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02009e26
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_02009e20
.L_02009dda:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_02002fe0
	mov r11, r0
	cmp r0, #255
	bne .L_02009d12
.L_02009dfa:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_02002fb0
	adds r0, r7, #0
	bl Func_02002fb8
	movs r0, #2
	bl WaitFrames
	b .L_02009b6e
.L_02009e20:
	movs r0, #10
	bl WaitFrames
.L_02009e26:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02002f80
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009e54:
	.4byte Data_02003cd0
.L_02009e58:
	.4byte Func_020017dc
	.section .text.x02009e5c,"ax",%progbits
	.global Func_02001e5c
	.thumb_func
Func_02001e5c:
	ldr r3, .L_02009e64
	strh r0, [r3]
	bx lr
	.2byte 0x0000
.L_02009e64:
	.4byte Data_02003cd0
	.section .text.x02009e68,"ax",%progbits
	.global Func_02001e68
	.thumb_func
Func_02001e68:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009ed4
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #32
	bl ObjectTable_Get
	adds r5, r0, #0
	ldrh r3, [r5, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_02009ed0
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	str r3, [sp, #16]
.L_02009e9a:
	bl Func_02001868
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #64]
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	ldr r2, .L_02009ed8
	ldr r3, [r5, #8]
	movs r1, #128
	lsls r1, r1, #12
	ands r3, r2
	mov r9, r1
	add r6, sp, #20
	add r3, r9
	str r3, [r6]
	mov r8, r3
	b .L_02009edc
	.2byte 0x0000
.L_02009ed0:
	.4byte 0xffffc000
.L_02009ed4:
	.4byte gPartyState
.L_02009ed8:
	.4byte 0xfff00000
.L_02009edc:
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	ands r3, r2
	adds r7, r3, r1
	mov r2, r8
	str r7, [r6, #8]
	str r2, [sp, #8]
	str r7, [sp, #4]
	movs r3, #34
	adds r3, r3, r5
	ldrb r0, [r3]
	adds r1, r2, #0
	adds r2, r7, #0
	mov r11, r3
	bl Func_02002fe0
	str r0, [sp, #12]
	movs r0, #128
	ldr r1, [sp, #16]
	lsls r0, r0, #13
	adds r2, r6, #0
	bl Vector_AddPolarOffsetFar
	mov r1, r11
	ldrb r0, [r1]
	ldr r2, [r6, #8]
	ldr r1, [r6]
	bl Func_02002fe0
	mov r10, r0
	cmp r0, #255
	beq .L_02009f70
	mov r2, r11
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	subs r0, r0, r3
	cmp r0, r9
	bgt .L_02009f70
	ldr r3, [sp, #8]
	ldr r2, .L_02009f68
	str r3, [r6]
	ldr r1, [sp, #4]
	movs r3, #128
	lsls r3, r3, #10
	str r1, [r6, #8]
	str r3, [r5, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r5, #52]
	adds r3, r5, #0
	adds r3, #100
	strh r2, [r3]
	adds r0, r5, #0
	movs r1, #2
	bl Func_02002f80
	adds r0, r5, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	ldr r3, .L_02009f6c
	str r3, [r5, #108]
	b .L_0200a01a
	.2byte 0x0000
.L_02009f68:
	.4byte 0x00000000
.L_02009f6c:
	.4byte Func_020017dc
.L_02009f70:
	add r1, sp, #16
	ldrh r1, [r1]
	movs r3, #0
	mov r2, r8
	strh r1, [r5, #6]
	str r3, [r5, #36]
	str r3, [r5, #44]
	str r2, [r5, #8]
	str r7, [r5, #16]
	b .L_0200a066
.L_02009f84:
	mov r3, r11
	ldrb r0, [r3]
	ldr r1, [r7]
	ldr r2, [r7, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	movs r1, #128
	subs r0, r0, r3
	lsls r1, r1, #12
	cmp r0, r1
	bgt .L_0200a03a
	ldrh r3, [r5, #32]
	movs r2, #0
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #20]
	movs r3, #89
	adds r3, r3, r6
	mov r9, r2
	mov r8, r3
.L_02009fb2:
	ldr r3, [r6]
	cmp r3, #0
	beq .L_02009fdc
	mov r1, r8
	ldrb r2, [r1]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02009fdc
	cmp r6, r5
	beq .L_02009fdc
	ldrh r3, [r6, #32]
	adds r0, r6, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #20
	bl Func_02003028
	cmp r0, #0
	bge .L_0200a03a
.L_02009fdc:
	movs r2, #1
	add r9, r2
	movs r3, #128
	mov r1, r9
	add r8, r3
	adds r6, #128
	cmp r1, #63
	ble .L_02009fb2
	ldr r2, [r7]
	adds r0, r5, #0
	str r2, [sp, #8]
	ldr r3, [r7, #8]
	str r3, [sp, #4]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r5, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl Func_02002fb0
	adds r0, r5, #0
	bl Func_02002fb8
	ldr r1, [sp, #12]
	cmp r10, r1
	bne .L_0200a060
.L_0200a01a:
	movs r0, #128
	ldr r1, [sp, #16]
	add r2, sp, #20
	lsls r0, r0, #13
	bl Vector_AddPolarOffsetFar
	mov r2, r11
	add r7, sp, #20
	ldrb r0, [r2]
	ldr r1, [r7]
	ldr r2, [r7, #8]
	bl Func_02002fe0
	mov r10, r0
	cmp r0, #255
	bne .L_02009f84
.L_0200a03a:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #52]
	ldr r2, [r5, #12]
	adds r0, r5, #0
	ldr r1, [sp, #8]
	ldr r3, [sp, #4]
	bl Func_02002fb0
	adds r0, r5, #0
	bl Func_02002fb8
	movs r0, #2
	bl WaitFrames
	b .L_02009e9a
.L_0200a060:
	movs r0, #10
	bl WaitFrames
.L_0200a066:
	movs r3, #0
	str r3, [r5, #108]
	adds r1, r5, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #52]
	adds r0, r5, #0
	movs r1, #1
	bl Func_02002f80
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a094,"ax",%progbits
	.global Func_02002094
	.thumb_func
Func_02002094:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	sub sp, #4
	cmp r3, r2
	beq .L_0200a0f8
	adds r7, r0, #0
.L_0200a0aa:
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
	bl Func_02002184
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200a0aa
.L_0200a0f8:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x0200a100,"ax",%progbits
	.global Func_02002100
	.thumb_func
Func_02002100:
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
	b .L_0200a16c
.L_0200a11c:
	ldrh r3, [r5]
	movs r1, #26
	ldrsh r7, [r2, r1]
	cmp r7, r3
	bne .L_0200a168
	adds r0, r7, #0
	bl Object_GetById
	adds r5, #2
	ldrh r2, [r5]
	mov r3, r10
	adds r6, r0, #0
	mov r8, r2
	ldrh r5, [r5, #2]
	cmp r3, #7
	bgt .L_0200a144
	ldr r3, [r6, #28]
	ldr r1, .L_0200a180
	adds r3, r3, r1
	str r3, [r6, #28]
.L_0200a144:
	mov r2, r9
	cmp r2, #1
	bne .L_0200a176
	adds r0, r5, #0
	bl GameFlag_SetBit
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	adds r0, r7, #0
	mov r1, r8
	adds r2, r5, #0
	bl Func_02002184
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	b .L_0200a176
.L_0200a168:
	adds r5, #6
	movs r1, #255
.L_0200a16c:
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	bne .L_0200a11c
.L_0200a176:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200a180:
	.4byte 0xffffe100
	.section .text.x0200a184,"ax",%progbits
	.global Func_02002184
	.thumb_func
Func_02002184:
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
	bl Func_02003160
	lsls r0, r0, #2
	adds r5, r5, r0
	mov r0, r8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a1f8
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #0
	strb r3, [r5, #2]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	cmp r6, #1
	beq .L_0200a1e4
	cmp r6, #1
	bcc .L_0200a1da
	cmp r6, #2
	beq .L_0200a1ee
	b .L_0200a226
.L_0200a1da:
	adds r0, r7, #0
	movs r1, #2
	bl Func_02002f80
	b .L_0200a226
.L_0200a1e4:
	adds r0, r7, #0
	movs r1, #4
	bl Func_02002f80
	b .L_0200a226
.L_0200a1ee:
	adds r0, r7, #0
	movs r1, #6
	bl Func_02002f80
	b .L_0200a226
.L_0200a1f8:
	movs r3, #255
	strb r3, [r5, #2]
	cmp r6, #1
	beq .L_0200a214
	cmp r6, #1
	bcc .L_0200a20a
	cmp r6, #2
	beq .L_0200a21e
	b .L_0200a226
.L_0200a20a:
	adds r0, r7, #0
	movs r1, #1
	bl Func_02002f80
	b .L_0200a226
.L_0200a214:
	adds r0, r7, #0
	movs r1, #3
	bl Func_02002f80
	b .L_0200a226
.L_0200a21e:
	adds r0, r7, #0
	movs r1, #5
	bl Func_02002f80
.L_0200a226:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x0200a22c,"ax",%progbits
	.global Func_0200222c
	.thumb_func
Func_0200222c:
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
	beq .L_0200a312
.L_0200a252:
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
	ldr r2, .L_0200a348
	adds r3, r5, r2
	ldr r2, .L_0200a34c
	asrs r3, r3, #2
	adds r6, r3, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a2a0
	ldr r0, [sp, #12]
	movs r1, #0
	movs r2, #0
	bl Func_02003088
	b .L_0200a300
.L_0200a2a0:
	adds r0, r7, #0
	bl Func_02003160
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r5, r5, r3
	str r5, [sp, #4]
	mov r2, r9
	ldrb r0, [r2]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	bl Func_02003010
	mov r3, r9
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	mov r10, r0
	ldrb r0, [r3]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	ldr r0, [sp, #12]
	bl Func_020030c0
	ldr r2, [sp, #4]
	movs r3, #128
	asrs r5, r5, #19
	strb r3, [r2, #3]
	adds r5, #4
	mov r3, r9
	adds r2, r5, #0
	ldrb r0, [r3]
	mov r1, r10
	bl Func_02003180
	add r8, r6
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a300
	adds r0, r7, #0
	movs r1, #0
	bl Func_02002f80
.L_0200a300:
	movs r3, #4
	add r11, r3
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200a252
.L_0200a312:
	ldr r3, .L_0200a350
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
	bge .L_0200a33a
	str r0, [r5, #20]
	str r0, [r5, #12]
.L_0200a33a:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a348:
	.4byte 0xfdff0000
.L_0200a34c:
	.4byte gMapShapeGrid
.L_0200a350:
	.4byte gPartyState
	.section .text.x0200a354,"ax",%progbits
	.global Func_02002354
	.thumb_func
Func_02002354:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #12
	adds r5, r0, #0
	bl Func_020030f8
	cmp r0, #0
	beq .L_0200a36a
	b .L_0200a4da
.L_0200a36a:
	ldr r3, .L_0200a4e4
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
	bl Func_02003188
	mov r8, r0
	cmp r0, #0
	bne .L_0200a396
	b .L_0200a4da
.L_0200a396:
	b .L_0200a4cc
.L_0200a398:
	ldrh r7, [r5]
	adds r0, r7, #0
	bl Object_GetById
	cmp r0, r8
	beq .L_0200a3a8
	adds r5, #4
	b .L_0200a4cc
.L_0200a3a8:
	ldrh r5, [r5, #2]
	bl Func_02003048
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a416
	movs r0, #125
	bl Func_020031a8
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	movs r1, #0
	mov r0, r8
	bl Func_02002f80
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
	bl Func_020029d0
	adds r0, r5, #0
	bl GameFlag_SetBit
	b .L_0200a4c6
.L_0200a416:
	adds r5, #1
	mov r10, r5
	mov r0, r10
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a4c6
	adds r6, #85
	strb r0, [r6]
	movs r0, #185
	bl Func_020031a8
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02003000
	movs r0, #0
	bl Func_020029d0
	movs r5, #2
	movs r0, #8
	mov r7, r8
	bl WaitFrames
	negs r5, r5
	mov r0, r8
	movs r1, #2
	adds r7, #34
	bl Func_02002f80
	ldrb r1, [r7]
	adds r0, r5, #0
	bl Func_02002ca8
	movs r0, #1
	bl Func_020029d0
	movs r0, #16
	bl WaitFrames
	ldrb r1, [r7]
	adds r0, r5, #0
	bl Func_02002ca8
	movs r0, #4
	bl WaitFrames
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02003000
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
	bl Func_020031a8
	bl Func_02002b04
	movs r0, #20
	bl WaitFrames
	mov r0, r10
	bl GameFlag_SetBit
.L_0200a4c6:
	bl Func_02003050
	b .L_0200a4da
.L_0200a4cc:
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_0200a4da
	b .L_0200a398
.L_0200a4da:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a4e4:
	.4byte gPartyState
	.section .text.x0200a4e8,"ax",%progbits
	.global Func_020024e8
	.thumb_func
Func_020024e8:
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
	beq .L_0200a5c2
.L_0200a506:
	ldrh r3, [r5]
	cmp r3, r6
	beq .L_0200a510
	adds r5, #4
	b .L_0200a5b6
.L_0200a510:
	ldrh r5, [r5, #2]
	bl Func_02003048
	adds r3, r5, #1
	mov r8, r3
	mov r0, r8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a5b0
	movs r0, #185
	bl Func_020031a8
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02003000
	movs r0, #0
	bl Func_020029d0
	movs r0, #8
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #2
	bl Func_02002f80
	adds r3, r7, #0
	adds r3, #34
	movs r0, #4
	ldrb r1, [r3]
	adds r2, r6, #0
	negs r0, r0
	bl Func_02002c1c
	movs r0, #1
	bl Func_020029d0
	movs r0, #16
	bl WaitFrames
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02003000
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
	bl Func_020031a8
	bl Func_02002b04
	movs r0, #20
	bl WaitFrames
	adds r0, r5, #0
	bl GameFlag_SetBit
	mov r0, r8
	bl GameFlag_SetBit
.L_0200a5b0:
	bl Func_02003050
	b .L_0200a5c2
.L_0200a5b6:
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200a506
.L_0200a5c2:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x0200a5c8,"ax",%progbits
	.global Func_020025c8
	.thumb_func
Func_020025c8:
	push {r5, lr}
	adds r5, r0, #0
	bl Object_GetById
	movs r3, #3
	adds r0, #92
	strb r3, [r0]
	adds r0, r5, #0
	bl Func_020030c0
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200a5e0,"ax",%progbits
	.global Func_020025e0
	.thumb_func
Func_020025e0:
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
	ldr r5, .L_0200a73c
	str r3, [sp, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	mov r11, r0
	ldr r1, [r5]
	movs r0, #8
	bl Func_02003090
	ldr r1, [r5]
	movs r0, #9
	bl Func_02003090
	ldr r1, [r5]
	movs r0, #10
	bl Func_02003090
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	bl Func_020025c8
	movs r0, #9
	bl Func_020025c8
	movs r0, #10
	bl Func_020025c8
	movs r1, #0
	movs r0, #9
	bl Object_SetModeById
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02003088
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02003088
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_02003088
	movs r0, #1
	bl WaitFrames
	b .L_0200a71a
.L_0200a666:
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
	ldr r2, .L_0200a740
	adds r3, r6, r2
	ldr r2, .L_0200a744
	asrs r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	adds r2, r5, #0
	adds r2, #92
	movs r3, #3
	strb r3, [r2]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a6bc
	mov r0, r9
	movs r1, #0
	movs r2, #0
	bl Func_02003088
	b .L_0200a716
.L_0200a6bc:
	adds r0, r5, #0
	bl Func_02003160
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r6, r6, r3
	str r6, [sp, #4]
	add r8, r10
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r7]
	bl Func_02003010
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	mov r10, r0
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	mov r0, r9
	bl Func_020030c0
	ldr r6, [sp, #4]
	asrs r5, r5, #19
	movs r3, #128
	adds r5, #4
	adds r2, r5, #0
	strb r3, [r6, #3]
	ldrb r0, [r7]
	mov r1, r10
	bl Func_02003180
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a716
	mov r0, r9
	movs r1, #9
	bl ObjectVisual_CopyAttributes
.L_0200a716:
	movs r3, #4
	add r11, r3
.L_0200a71a:
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200a666
	movs r0, #10
	bl WaitFrames
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a73c:
	.4byte gPartyState
.L_0200a740:
	.4byte 0xfdff0000
.L_0200a744:
	.4byte gMapShapeGrid
	.section .text.x0200a748,"ax",%progbits
	.global Func_02002748
	.thumb_func
Func_02002748:
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
	.section .text.x0200a764,"ax",%progbits
	.global Func_02002764
	.thumb_func
Func_02002764:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #12
	adds r6, r0, #0
	bl Func_020030f8
	cmp r0, #0
	beq .L_0200a77c
	b .L_0200a93e
.L_0200a77c:
	ldr r3, .L_0200a94c
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
	bl Func_02003188
	mov r10, r0
	cmp r0, #0
	bne .L_0200a7b0
	b .L_0200a93e
.L_0200a7b0:
	b .L_0200a930
.L_0200a7b2:
	ldrh r3, [r6]
	mov r8, r3
	mov r0, r8
	bl Object_GetById
	cmp r0, r10
	beq .L_0200a7c4
	adds r6, #4
	b .L_0200a930
.L_0200a7c4:
	ldrh r6, [r6, #2]
	bl Func_02003048
	adds r0, r6, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a85e
	adds r0, r7, #0
	movs r1, #1
	bl Func_02002f80
	mov r1, r10
	adds r0, r7, #0
	bl Func_02002748
	movs r0, #1
	bl WaitFrames
	movs r0, #125
	bl Func_020031a8
	movs r0, #8
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #0
	bl Func_02002f80
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
	bl Func_020029d0
	mov r0, r10
	adds r1, r7, #0
	bl Func_02002748
	movs r0, #1
	bl WaitFrames
	adds r0, r6, #0
	bl GameFlag_SetBit
	b .L_0200a92a
.L_0200a85e:
	adds r6, #1
	mov r9, r6
	mov r0, r9
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200a92a
	adds r0, r7, #0
	movs r1, #0
	bl Func_02002f80
	mov r1, r10
	adds r0, r7, #0
	bl Func_02002748
	adds r5, #85
	movs r0, #1
	bl WaitFrames
	strb r6, [r5]
	movs r0, #185
	bl Func_020031a8
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02003000
	movs r0, #0
	bl Func_020029d0
	mov r8, r5
	movs r0, #8
	movs r6, #2
	mov r5, r10
	bl WaitFrames
	negs r6, r6
	adds r0, r7, #0
	movs r1, #2
	adds r5, #34
	bl Func_02002f80
	ldrb r1, [r5]
	adds r0, r6, #0
	bl Func_02002ca8
	movs r0, #1
	bl Func_020029d0
	movs r0, #16
	bl WaitFrames
	ldrb r1, [r5]
	adds r0, r6, #0
	bl Func_02002ca8
	movs r0, #4
	bl WaitFrames
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02003000
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
	bl Func_020031a8
	bl Func_02002b04
	movs r0, #20
	bl WaitFrames
	mov r0, r9
	bl GameFlag_SetBit
.L_0200a92a:
	bl Func_02003050
	b .L_0200a93e
.L_0200a930:
	movs r2, #255
	ldrh r3, [r6]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_0200a93e
	b .L_0200a7b2
.L_0200a93e:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a94c:
	.4byte gPartyState
	.section .text.x0200a950,"ax",%progbits
	.global Func_02002950
	.thumb_func
Func_02002950:
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
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0200a980
	adds r3, #15
.L_0200a980:
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
	ldr r3, .L_0200a9cc
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
.L_0200a9cc:
	.4byte gPartyState
	.section .text.x0200a9d0,"ax",%progbits
	.global Func_020029d0
	.thumb_func
Func_020029d0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0200aadc
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
.L_0200a9f2:
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
	ldr r0, .L_0200aae0
	adds r1, r1, r3
	ldr r3, .L_0200aae4
	adds r1, r1, r0
	movs r0, #30
	adds r2, r2, r3
	adds r0, #255
	ldr r3, [r7, #16]
	bl Func_02002f98
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200aac6
	mov r1, r9
	ldr r0, [r6, #80]
	bl Func_02003150
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
	bl Func_02002f80
	adds r0, r6, #0
	ldr r1, .L_0200aae8
	bl Func_02002f90
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
	beq .L_0200aa90
	mov r3, r10
	lsls r5, r3, #13
	adds r0, r5, #0
	bl Math_Cosine
	ldr r3, .L_0200aaec
	ldr r1, .L_0200aaf0
	mov lr, r3
	.2byte 0xf800
	str r0, [r6, #68]
	adds r0, r5, #0
	bl Math_Sine
	b .L_0200aa94
.L_0200aa90:
	mov r0, r8
	str r0, [r6, #68]
.L_0200aa94:
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
	ldr r3, .L_0200aaf4
	lsls r0, r0, #9
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r6, #0
	adds r3, #100
	strh r0, [r3]
	ldr r3, .L_0200aaf8
	str r3, [r6, #48]
	ldr r3, .L_0200aafc
	str r3, [r6, #52]
	ldr r3, .L_0200ab00
	str r3, [r6, #108]
.L_0200aac6:
	movs r0, #1
	add r10, r0
	mov r2, r10
	cmp r2, #7
	bls .L_0200a9f2
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200aadc:
	.4byte gPartyState
.L_0200aae0:
	.4byte 0xfff80000
.L_0200aae4:
	.4byte 0xfffe0000
.L_0200aae8:
	.4byte Data_020032d4
.L_0200aaec:
	.4byte IwramMulQ16
.L_0200aaf0:
	.4byte 0x00013333
.L_0200aaf4:
	.4byte 0xffffff00
.L_0200aaf8:
	.4byte 0xfffff800
.L_0200aafc:
	.4byte 0xfffffa00
.L_0200ab00:
	.4byte Func_02002950
	.section .text.x0200ab04,"ax",%progbits
	.global Func_02002b04
	.thumb_func
Func_02002b04:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0200ac04
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200abf8
	movs r3, #0
	mov r9, r3
	mov r10, r3
.L_0200ab28:
	movs r0, #30
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, #255
	bl Func_02002f98
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200abee
	mov r1, r9
	ldr r0, [r7, #80]
	bl Func_02003150
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
	bl Func_02002f80
	ldr r1, .L_0200ac08
	adds r0, r7, #0
	bl Func_02002f90
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
	ldr r3, .L_0200ac0c
	adds r2, r2, r3
	str r2, [r7, #68]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r7, #76]
	adds r3, r3, r0
	ldr r4, .L_0200ac10
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r2, r2, r3
	adds r2, r2, r4
	str r2, [r7, #76]
	bl Random16Far
	ldr r2, .L_0200ac14
	lsls r0, r0, #12
	lsrs r0, r0, #16
	adds r3, r7, #0
	adds r0, r0, r2
	adds r3, #100
	strh r0, [r3]
	mov r3, r8
	str r3, [r7, #48]
	str r3, [r7, #52]
	ldr r3, .L_0200ac18
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
.L_0200abee:
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200ab28
.L_0200abf8:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ac04:
	.4byte gPartyState
.L_0200ac08:
	.4byte Data_02003304
.L_0200ac0c:
	.4byte 0xffffa000
.L_0200ac10:
	.4byte 0xffffd000
.L_0200ac14:
	.4byte 0xfffff800
.L_0200ac18:
	.4byte Func_02002950
	.section .text.x0200ac1c,"ax",%progbits
	.global Func_02002c1c
	.thumb_func
Func_02002c1c:
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
	ldr r2, .L_0200ac9c
	adds r7, r0, #0
	ldr r1, .L_0200aca0
	adds r3, r3, r2
	adds r5, r7, #0
	asrs r3, r3, #2
	adds r5, #34
	adds r6, r3, r1
	ldr r2, [r7, #16]
	ldr r1, [r7, #8]
	ldrb r0, [r5]
	bl Func_02003010
	ldr r2, [r7, #16]
	mov r8, r0
	ldr r1, [r7, #8]
	ldrb r0, [r5]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #8]
	asrs r2, r0, #19
	add r2, r10
	cmp r3, #0
	bge .L_0200ac76
	ldr r1, .L_0200aca4
	adds r3, r3, r1
.L_0200ac76:
	ldr r0, [r7, #16]
	asrs r1, r3, #20
	cmp r0, #0
	bge .L_0200ac82
	ldr r3, .L_0200aca4
	adds r0, r0, r3
.L_0200ac82:
	asrs r3, r0, #20
	lsls r3, r3, #7
	adds r3, r1, r3
	ldrb r0, [r5]
	mov r1, r8
	adds r6, r6, r3
	bl Func_02003180
	strb r0, [r6]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200ac9c:
	.4byte 0xfdff0000
.L_0200aca0:
	.4byte gMapShapeGrid
.L_0200aca4:
	.4byte 0x000fffff
	.section .text.x0200aca8,"ax",%progbits
	.global Func_02002ca8
	.thumb_func
Func_02002ca8:
	push {lr}
	ldr r3, .L_0200acbc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r2, [r3]
	bl Func_02002c1c
	pop {pc}
	.2byte 0x0000
.L_0200acbc:
	.4byte gPartyState
	.section .text.x0200acc0,"ax",%progbits
	.global Func_02002cc0
	.thumb_func
Func_02002cc0:
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
.L_0200acde:
	ldr r3, [r5, #24]
	cmp r3, #19
	bhi .L_0200ad2c
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
	ldr r3, .L_0200ad20
	ldr r2, .L_0200ad24
	ands r1, r3
	ldrh r3, [r6, #8]
	adds r0, r6, #0
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #8]
	adds r1, r5, #0
	bl Func_02003198
	adds r0, r5, #0
	movs r1, #63
	ldr r2, .L_0200ad28
	bl Func_020031a0
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	b .L_0200ad2c
.L_0200ad20:
	.4byte 0x000003ff
.L_0200ad24:
	.4byte 0xfffffc00
.L_0200ad28:
	.4byte 0xffff8000
.L_0200ad2c:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r2, r8
	adds r6, #40
	adds r5, #28
	cmp r2, #0
	bge .L_0200acde
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200ad44,"ax",%progbits
	.global Func_02002d44
	.thumb_func
Func_02002d44:
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
	ble .L_0200adf0
	adds r7, r2, #0
.L_0200ad6c:
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
	bne .L_0200ad6c
.L_0200adf0:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200ae00,"ax",%progbits
	.global Func_02002e00
	.thumb_func
Func_02002e00:
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
	ldr r0, .L_0200aeb4
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Func_02002f40
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
.L_0200ae58:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	bl Func_02003190
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
	bge .L_0200ae58
	movs r1, #176
	lsls r1, r1, #5
	adds r2, r6, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200aeb8
	bl Scheduler_AddOrUpdateCallback
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200aeb4:
	.4byte 0x000001f0
.L_0200aeb8:
	.4byte Func_02002cc0
	.section .text.x0200aebc,"ax",%progbits
	.global Func_02002ebc
	.thumb_func
Func_02002ebc:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r0, .L_0200aee4
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
.L_0200aee4:
	.4byte Func_02002cc0
	.section .rodata.x0200b1b0,"a",%progbits
.L_0200b1b0:
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
.L_0200b1ec:
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
.L_0200b228:
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
	.global Data_02003264
Data_02003264:
	.4byte 0x000c000b
	.4byte 0x000e000d
	.2byte 0xffff
	.global Data_0200326e
Data_0200326e:
	.2byte 0x0008
	.2byte 0xffff
	.global Data_02003272
Data_02003272:
	.2byte 0xffff
	.4byte 0x80000000
	.4byte 0xc000ffff
	.4byte 0xa000e000
	.4byte 0x4000c000
	.4byte 0x60002000
	.4byte 0xffff4000
	.4byte 0x80000000
	.4byte 0xffffffff
	.4byte 0x80000000
	.4byte 0xc000ffff
	.4byte 0xffffffff
	.4byte 0x4000c000
	.4byte 0xffffffff
	.4byte 0xffff4000
	.4byte 0x80000000
	.2byte 0xffff
	.global Data_020032b2
Data_020032b2:
	.2byte 0xffff
	.4byte 0x80000000
	.4byte 0xc000ffff
	.4byte 0x80000000
	.4byte 0x4000c000
	.4byte 0x80000000
	.4byte 0xffff4000
	.4byte 0x80000000
	.4byte 0x0000ffff
	.global Data_020032d4
Data_020032d4:
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
	.global Data_02003304
Data_02003304:
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
	.global Data_02003334
Data_02003334:
	.4byte .L_0200b1b0
	.4byte .L_0200b1ec
	.4byte .L_0200b228
	.global Data_02003340
Data_02003340:
	.4byte 0x00070008
	.4byte 0x00090200
	.4byte 0x02010007
	.4byte 0x0007000a
	.4byte 0x000b0202
	.4byte 0x02030007
	.2byte 0xffff
	.global Data_0200335a
Data_0200335a:
	.2byte 0x0008
	.4byte 0x02000007
	.4byte 0x00070009
	.4byte 0x000a0201
	.4byte 0x02020007
	.4byte 0x0000ffff
	.global Data_02003370
Data_02003370:
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00001000
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x80010000
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
	.global Data_020033d0
Data_020033d0:
	.4byte 0x00500330
	.4byte 0x03400210
	.4byte 0x02200060
	.4byte 0x0006ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020033f0
Data_020033f0:
	.4byte 0x000f00b0
	.4byte 0x00c000c0
	.4byte 0x00d00021
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x000000ce
	.4byte 0x1012e002
	.4byte 0xffffffff
	.4byte 0x102010cf
	.4byte 0xffffffff
	.4byte 0x000000cf
	.4byte 0x101020ce
	.4byte 0xffffffff
	.4byte 0x1020b0cf
	.4byte 0xffffffff
	.4byte 0x103070cf
	.4byte 0xffffffff
	.4byte 0x104080cf
	.4byte 0xffffffff
	.4byte 0x105050d3
	.4byte 0xffffffff
	.4byte 0x106100cf
	.4byte 0xffffffff
	.4byte 0x107030cf
	.4byte 0xffffffff
	.4byte 0x108040cf
	.4byte 0xffffffff
	.4byte 0x109120cf
	.4byte 0xffffffff
	.4byte 0x10a090d0
	.4byte 0xffffffff
	.4byte 0x10b020cf
	.4byte 0xffffffff
	.4byte 0x10c040d3
	.4byte 0xffffffff
	.4byte 0x10d0f0cf
	.4byte 0xffffffff
	.4byte 0x10e110cf
	.4byte 0xffffffff
	.4byte 0x10f0d0cf
	.4byte 0xffffffff
	.4byte 0x110060cf
	.4byte 0xffffffff
	.4byte 0x1110e0cf
	.4byte 0xffffffff
	.4byte 0x112090cf
	.4byte 0xffffffff
	.4byte 0x000000d0
	.4byte 0x101030d0
	.4byte 0xffffffff
	.4byte 0x102070d0
	.4byte 0xffffffff
	.4byte 0x103010d0
	.4byte 0xffffffff
	.4byte 0x1040a0d0
	.4byte 0xffffffff
	.4byte 0x105020d1
	.4byte 0xffffffff
	.4byte 0x1060b0d0
	.4byte 0xffffffff
	.4byte 0x107020d0
	.4byte 0xffffffff
	.4byte 0x1080c0d0
	.4byte 0xffffffff
	.4byte 0x1090a0cf
	.4byte 0xffffffff
	.4byte 0x10a040d0
	.4byte 0xffffffff
	.4byte 0x10b060d0
	.4byte 0xffffffff
	.4byte 0x10c080d0
	.4byte 0xffffffff
	.4byte 0x000000d1
	.4byte 0x101020d2
	.4byte 0xffffffff
	.4byte 0x102050d0
	.4byte 0xffffffff
	.4byte 0x103060d2
	.4byte 0xffffffff
	.4byte 0x104050d2
	.4byte 0xffffffff
	.4byte 0x105060d1
	.4byte 0xffffffff
	.4byte 0x106050d1
	.4byte 0xffffffff
	.4byte 0x000000d2
	.4byte 0x101030d2
	.4byte 0xffffffff
	.4byte 0x102010d1
	.4byte 0xffffffff
	.4byte 0x103010d2
	.4byte 0xffffffff
	.4byte 0x104080d2
	.4byte 0xffffffff
	.4byte 0x105040d1
	.4byte 0xffffffff
	.4byte 0x106030d1
	.4byte 0xffffffff
	.4byte 0x107010d3
	.4byte 0xffffffff
	.4byte 0x108040d2
	.4byte 0xffffffff
	.4byte 0x109020d3
	.4byte 0xffffffff
	.4byte 0x10a090d3
	.4byte 0xffffffff
	.4byte 0x10b070d1
	.4byte 0xffffffff
	.4byte 0x000000d3
	.4byte 0x101070d2
	.4byte 0xffffffff
	.4byte 0x102090d2
	.4byte 0xffffffff
	.4byte 0x103020d4
	.4byte 0xffffffff
	.4byte 0x1040c0cf
	.4byte 0xffffffff
	.4byte 0x105050cf
	.4byte 0xffffffff
	.4byte 0x106010d4
	.4byte 0xffffffff
	.4byte 0x107080d3
	.4byte 0xffffffff
	.4byte 0x108070d3
	.4byte 0xffffffff
	.4byte 0x1090a0d2
	.4byte 0xffffffff
	.4byte 0x10a0a0d3
	.4byte 0xffffffff
	.4byte 0x000000d4
	.4byte 0x101060d3
	.4byte 0xffffffff
	.4byte 0x102030d3
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02003618
Data_02003618:
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003648
Data_02003648:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003660
Data_02003660:
	.4byte 0x006400f5
	.4byte 0x00000007
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003690
Data_02003690:
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003708
Data_02003708:
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020037c8
Data_020037c8:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020037f8
Data_020037f8:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200381c
Data_0200381c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_020005d0
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_020005d0
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
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
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
	.4byte 0x00000003
	.4byte 0xffff0034
	.4byte Func_02000920
	.4byte 0x50008a05
	.4byte 0x09420032
	.4byte Func_02000548
	.4byte 0x50008a05
	.4byte 0x09430033
	.4byte Func_0200058c
	.4byte 0x50009705
	.4byte 0x09440034
	.4byte Func_020007dc
	.4byte 0x00009705
	.4byte 0x09440034
	.4byte Func_020008a8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200393c
Data_0200393c:
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
	.4byte 0x00000021
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
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte Func_0200095c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020039e4
Data_020039e4:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte Func_0200095c
	.4byte 0x00000002
	.4byte 0x02000033
	.4byte Func_02000964
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000280
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003a5c
Data_02003a5c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte Func_02000b3c
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte Func_0200095c
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte Func_02000b34
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte Func_02000b34
	.4byte 0x50008615
	.4byte 0x0202000a
	.4byte Func_02000b34
	.4byte 0x50008615
	.4byte 0x0203000b
	.4byte Func_02000b34
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003b28
Data_02003b28:
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
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff001e
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte Func_02000f00
	.4byte 0x50008615
	.4byte 0x02000008
	.4byte Func_02000b34
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte Func_02000b34
	.4byte 0x50008615
	.4byte 0x0202000a
	.4byte Func_02000b34
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000bfc
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000d2c
	.4byte 0x80008c15
	.4byte 0xffff000b
	.4byte Func_02000fac
	.4byte 0x10008c15
	.4byte 0xffff000b
	.4byte Func_02000bfc
	.4byte 0x50008c15
	.4byte 0xffff000b
	.4byte Func_02000c64
	.4byte 0x10008c15
	.4byte 0x0946000c
	.4byte Func_02000bfc
	.4byte 0x00008c15
	.4byte 0x0946000c
	.4byte Func_02000d2c
	.4byte 0x10008c15
	.4byte 0x0947000d
	.4byte Func_02000bfc
	.4byte 0x00008c15
	.4byte 0x0947000d
	.4byte Func_02000d2c
	.4byte 0x80008c15
	.4byte 0xffff000e
	.4byte Func_02000fac
	.4byte 0x10008c15
	.4byte 0xffff000e
	.4byte Func_02000bfc
	.4byte 0x50008c15
	.4byte 0xffff000e
	.4byte Func_02000c64
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000e78
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000de4
	.4byte 0x80009705
	.4byte 0xffff0034
	.4byte Func_02000fac
	.4byte 0x50009705
	.4byte 0x094b0034
	.4byte Func_02000f0c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003c90
Data_02003c90:
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte Func_02000bfc
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte Func_02000d2c
	.4byte 0x50009705
	.4byte 0x09480032
	.4byte Func_02000fb8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.global Data_02003ccc
Data_02003ccc:
	.space 0x00000004
	.global Data_02003cd0
Data_02003cd0:
