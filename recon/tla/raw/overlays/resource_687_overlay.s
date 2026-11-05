.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080ce,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815a,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200854c,"ax",%progbits
	.global Func_0200054c
	.thumb_func
Func_0200054c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #8
	ldr r5, [r3, #108]
	sub sp, #88
	bl Actor_GetById
	movs r1, #170
	lsls r1, r1, #1
	adds r5, r5, r1
	movs r2, #0
	ldrsh r3, [r5, r2]
	mov r9, r0
	mov r6, r9
	subs r3, #30
	add r1, sp, #76
	adds r6, #8
	cmp r3, #7
	bhi .L_020085e8
	ldr r2, .L_020087fc
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_02008588:
	.4byte .L_020085a8
	.4byte .L_020085c6
	.4byte .L_020085ac
	.4byte .L_020085ce
	.4byte .L_020085b6
	.4byte .L_020085d8
	.4byte .L_020085be
	.4byte .L_020085de
.L_020085a8:
	movs r3, #0
	b .L_020085e0
.L_020085ac:
	movs r3, #1
	negs r3, r3
	str r3, [r1]
	movs r3, #0
	b .L_020085e6
.L_020085b6:
	movs r3, #0
	str r3, [r1]
	movs r3, #1
	b .L_020085e6
.L_020085be:
	movs r3, #1
	str r3, [r1]
	movs r3, #0
	b .L_020085e6
.L_020085c6:
	movs r3, #1
	negs r3, r3
	str r3, [r1]
	b .L_020085e6
.L_020085ce:
	movs r3, #1
	negs r3, r3
	str r3, [r1]
	movs r3, #1
	b .L_020085e6
.L_020085d8:
	movs r3, #1
	str r3, [r1]
	b .L_020085e6
.L_020085de:
	movs r3, #1
.L_020085e0:
	str r3, [r1]
	movs r3, #1
	negs r3, r3
.L_020085e6:
	str r3, [r1, #8]
.L_020085e8:
	mov r1, r9
	add r5, sp, #76
	ldr r3, [r1, #8]
	ldr r2, [r5]
	asrs r3, r3, #21
	adds r3, r3, r2
	movs r1, #128
	lsls r1, r1, #13
	lsls r3, r3, #21
	adds r3, r3, r1
	str r3, [r5]
	mov r2, r9
	ldr r3, [r2, #16]
	ldr r2, [r5, #8]
	asrs r3, r3, #21
	adds r3, r3, r2
	lsls r3, r3, #21
	adds r3, r3, r1
	str r3, [r5, #8]
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_020003d0
	cmp r0, #46
	ble .L_02008648
	ldr r2, [r6]
	ldr r3, [r5]
	cmp r2, r3
	bge .L_02008628
	movs r1, #128
	lsls r1, r1, #8
	b .L_0200862a
.L_02008628:
	ldr r1, .L_02008800
.L_0200862a:
	adds r3, r2, r1
	str r3, [r6]
	mov r2, r9
	ldr r0, [r2, #16]
	ldr r3, [r5, #8]
	cmp r0, r3
	bge .L_02008640
	movs r5, #128
	lsls r5, r5, #8
	adds r3, r0, r5
	b .L_02008646
.L_02008640:
	ldr r1, .L_02008800
	mov r2, r9
	adds r3, r0, r1
.L_02008646:
	str r3, [r2, #16]
.L_02008648:
	add r3, sp, #76
	ldr r1, [r3]
	ldr r2, [r3, #8]
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #1
	mov r11, r3
	bl Func_0200034c
	asrs r0, r0, #8
	cmp r0, #40
	bne .L_0200866c
	ldr r0, .L_02008804
	bl Math_Cosine
	str r0, [sp, #16]
	ldr r0, .L_02008804
	b .L_0200867a
.L_0200866c:
	movs r0, #128
	lsls r0, r0, #2
	bl Math_Cosine
	str r0, [sp, #16]
	movs r0, #128
	lsls r0, r0, #2
.L_0200867a:
	bl Math_Sine
	adds r7, r0, #0
	mov r1, r9
	mov r2, r11
	ldr r3, [r2]
	ldr r5, [r1, #8]
	subs r5, r5, r3
	ldr r3, [r1, #16]
	adds r0, r5, #0
	mov r10, r3
	ldr r3, [r2, #8]
	ldr r2, .L_02008808
	mov r1, r10
	subs r1, r1, r3
	mov r8, r2
	mov r10, r1
	ldr r1, [sp, #16]
	mov lr, r8
	.2byte 0xf800
	adds r1, r7, #0
	adds r6, r0, #0
	mov r0, r10
	mov lr, r8
	.2byte 0xf800
	adds r1, r7, #0
	subs r6, r6, r0
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r0, #0
	ldr r1, [sp, #16]
	mov r0, r10
	mov lr, r8
	.2byte 0xf800
	mov r1, r11
	ldr r3, [r1]
	adds r5, r5, r0
	adds r6, r6, r3
	ldr r3, [r1, #8]
	ldr r7, .L_0200880c
	adds r5, r5, r3
	mov r2, r9
	str r5, [r2, #16]
	str r6, [r2, #8]
	ldr r2, [r7]
	movs r3, #3
	mov r10, r3
	mov r5, r10
	ands r5, r2
	mov r10, r5
	cmp r5, #0
	beq .L_020086e6
	b .L_020087ee
.L_020086e6:
	movs r3, #10
	ands r3, r2
	cmp r3, #0
	bne .L_020086f8
	movs r0, #171
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002054
.L_020086f8:
	add r1, sp, #20
	movs r3, #2
	str r3, [r1]
	movs r3, #7
	str r3, [r1, #4]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r1, #8]
	str r3, [r1, #12]
	mov r2, r9
	ldrh r0, [r2, #6]
	mov r8, r1
	bl Math_Cosine
	mov r3, r9
	adds r5, r0, #0
	ldrh r0, [r3, #6]
	bl Math_Sine
	lsls r3, r5, #2
	lsls r2, r0, #2
	adds r3, r3, r5
	adds r2, r2, r0
	lsls r3, r3, #1
	lsls r2, r2, #1
	add r6, sp, #60
	negs r0, r3
	negs r4, r2
	str r3, [r6]
	str r2, [r6, #4]
	str r0, [r6, #8]
	str r4, [r6, #12]
	mov r5, r9
	ldr r1, [r5, #8]
	adds r3, r3, r1
	str r3, [r6]
	movs r1, #128
	ldr r3, [r5, #16]
	lsls r1, r1, #8
	adds r2, r2, r3
	str r2, [r6, #4]
	mov r11, r1
	ldr r3, [r5, #8]
	adds r0, r0, r3
	str r0, [r6, #8]
	ldr r3, [r5, #16]
	adds r4, r4, r3
	str r4, [r6, #12]
	ldr r3, [r5, #16]
	cmp r2, r3
	beq .L_0200876a
	ldr r3, .L_02008810
	adds r2, r2, r3
	adds r3, r4, r3
	str r2, [r6, #4]
	str r3, [r6, #12]
.L_0200876a:
	ldr r7, [r7]
	movs r3, #4
	mov r2, r9
	ands r7, r3
	ldr r1, [r2, #12]
	ldr r0, [r6]
	ldr r2, [r6, #4]
	cmp r7, #0
	beq .L_020087b8
	movs r3, #192
	mov r5, r10
	lsls r3, r3, #10
	adds r0, r0, r3
	str r5, [sp, #0]
	str r5, [sp, #4]
	mov r3, r8
	movs r5, #176
	lsls r5, r5, #12
	str r3, [sp, #12]
	mov r3, r11
	str r5, [sp, #8]
	bl Func_0200015c
	ldr r0, [r6, #8]
	ldr r1, .L_02008814
	mov r3, r10
	mov r2, r9
	adds r0, r0, r1
	ldr r1, [r2, #12]
	ldr r2, [r6, #12]
	str r3, [sp, #0]
	str r3, [sp, #4]
	str r5, [sp, #8]
	ldr r3, .L_02008800
	mov r5, r8
	str r5, [sp, #12]
	bl Func_0200015c
	b .L_020087ee
.L_020087b8:
	ldr r3, .L_02008814
	movs r5, #176
	adds r0, r0, r3
	mov r3, r8
	lsls r5, r5, #12
	str r3, [sp, #12]
	ldr r3, .L_02008800
	str r7, [sp, #0]
	str r7, [sp, #4]
	str r5, [sp, #8]
	bl Func_0200015c
	ldr r0, [r6, #8]
	movs r1, #192
	lsls r1, r1, #10
	mov r2, r9
	mov r3, r8
	adds r0, r0, r1
	ldr r1, [r2, #12]
	ldr r2, [r6, #12]
	str r3, [sp, #12]
	mov r3, r11
	str r7, [sp, #0]
	str r7, [sp, #4]
	str r5, [sp, #8]
	bl Func_0200015c
.L_020087ee:
	add sp, #88
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020087fc:
	.4byte .L_02008588
.L_02008800:
	.4byte 0xffff8000
.L_02008804:
	.4byte 0xfffffe00
.L_02008808:
	.4byte IwramMulQ16
.L_0200880c:
	.4byte gFrameCount
.L_02008810:
	.4byte 0xfff40000
.L_02008814:
	.4byte 0xfffd0000
	.section .text.x02008da0,"ax",%progbits
	.global Func_02000da0
	.thumb_func
Func_02000da0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_02008e24
	bl Task_RemoveFar
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02002054
	ldr r2, .L_02008e28
	movs r3, #1
	str r3, [r2]
.L_02008dcc:
	movs r0, #10
	bl wait
	ldr r3, .L_02008e2c
	movs r4, #160
	lsls r4, r4, #19
	movs r0, #0
	movs r2, #1
	adds r4, #98
	mov lr, r0
	mov r9, r2
	mov r11, r3
	mov r10, r0
	mov r8, r4
.L_02008de8:
	mov r0, r8
	ldrh r1, [r0]
	mov r3, r10
	mov r0, r11
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008e20
	movs r4, #31
	lsls r2, r2, #16
	lsls r0, r1, #16
	adds r5, r4, #0
	ands r5, r1
	lsrs r6, r2, #21
	lsrs r1, r0, #21
	lsrs r7, r2, #26
	lsrs r0, r0, #26
	lsrs r2, r2, #16
	ands r1, r3
	ands r0, r3
	ands r6, r3
	ands r7, r3
	ands r2, r4
	adds r3, r5, #0
	cmp r3, r2
	bcs .L_02008e30
	adds r5, r3, #1
	movs r3, #1
	mov lr, r3
	b .L_02008e30
.L_02008e20:
	.4byte 0x0000001f
.L_02008e24:
	.4byte Func_02000c50
.L_02008e28:
	.4byte gData_020022cc
.L_02008e2c:
	.4byte gData_020026c0
.L_02008e30:
	lsls r3, r5, #16
	lsrs r3, r3, #16
	cmp r3, r2
	bls .L_02008e48
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r3, r4
	lsls r3, r3, #16
	movs r2, #1
	asrs r5, r3, #16
	mov lr, r2
.L_02008e48:
	lsls r3, r1, #16
	lsrs r3, r3, #16
	mov r12, r6
	cmp r3, r12
	bcs .L_02008e5c
	adds r3, #1
	lsls r3, r3, #16
	asrs r1, r3, #16
	movs r3, #1
	mov lr, r3
.L_02008e5c:
	lsls r3, r1, #16
	lsrs r3, r3, #16
	cmp r3, r12
	bls .L_02008e74
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r3, r4
	lsls r3, r3, #16
	movs r2, #1
	asrs r1, r3, #16
	mov lr, r2
.L_02008e74:
	lsls r3, r0, #16
	lsrs r3, r3, #16
	mov r12, r7
	cmp r3, r12
	bcs .L_02008e88
	adds r3, #1
	lsls r3, r3, #16
	asrs r0, r3, #16
	movs r3, #1
	mov lr, r3
.L_02008e88:
	lsls r3, r0, #16
	lsrs r3, r3, #16
	cmp r3, r12
	bls .L_02008ea0
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r3, r4
	lsls r3, r3, #16
	movs r2, #1
	asrs r0, r3, #16
	mov lr, r2
.L_02008ea0:
	lsls r3, r0, #16
	lsls r2, r1, #16
	lsrs r2, r2, #11
	lsrs r3, r3, #6
	orrs r3, r2
	lsls r2, r5, #16
	lsrs r2, r2, #16
	movs r0, #1
	orrs r2, r3
	add r9, r0
	mov r3, r8
	movs r4, #2
	strh r2, [r3]
	mov r2, r9
	add r8, r4
	add r10, r4
	cmp r2, #15
	bne .L_02008de8
	mov r3, lr
	cmp r3, #0
	beq .L_02008ecc
	b .L_02008dcc
.L_02008ecc:
	ldr r2, .L_02008eec
	movs r3, #1
	movs r0, #128
	str r3, [r2]
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	bl Func_02001fb4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008eec:
	.4byte gData_020022cc
	.section .text.x02009108,"ax",%progbits
	.global Func_02001108
	.thumb_func
Func_02001108:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #100
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #221
	bl GameFlag_SetBit
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #8
	bl Actor_SetSpeedParameters
	movs r0, #8
	bl Actor_GetById
	movs r3, #192
	lsls r3, r3, #8
	movs r1, #176
	movs r2, #132
	strh r3, [r0, #6]
	lsls r2, r2, #1
	movs r0, #8
	lsls r1, r1, #1
	bl Actor_ResetAndSetPositionInMode2
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02001fec
	movs r0, #176
	movs r1, #1
	movs r2, #196
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #17
	bl Camera_MoveWithinMapBounds
	bl Func_02001ffc
	movs r0, #8
	bl Actor_WaitForMoveById
	movs r0, #78
	bl Func_02002054
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02002054
	ldr r3, .L_020091bc
	mov r0, sp
	adds r0, #98
	strh r3, [r0]
	movs r1, #160
	movs r3, #128
	movs r2, #129
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, #98
	adds r2, #14
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #6
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #49
	movs r1, #70
	movs r2, #19
	movs r3, #73
	bl Func_02001f74
	b .L_020091c0
.L_020091bc:
	.4byte 0x00005ccb
.L_020091c0:
	movs r0, #10
	bl wait
	ldr r3, .L_02009218
	movs r4, #160
	lsls r4, r4, #19
	movs r0, #0
	movs r2, #1
	adds r4, #98
	mov lr, r0
	mov r8, r2
	mov r11, r3
	mov r9, r0
	mov r10, r4
.L_020091dc:
	mov r0, r10
	ldrh r1, [r0]
	mov r3, r9
	mov r0, r11
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009214
	movs r4, #31
	lsls r2, r2, #16
	lsls r0, r1, #16
	adds r5, r4, #0
	ands r5, r1
	lsrs r6, r2, #21
	lsrs r1, r0, #21
	lsrs r7, r2, #26
	lsrs r0, r0, #26
	lsrs r2, r2, #16
	ands r1, r3
	ands r0, r3
	ands r6, r3
	ands r7, r3
	ands r2, r4
	adds r3, r5, #0
	cmp r3, r2
	bcs .L_0200921c
	adds r5, r3, #1
	movs r3, #1
	mov lr, r3
	b .L_0200921c
.L_02009214:
	.4byte 0x0000001f
.L_02009218:
	.4byte gData_020026c0
.L_0200921c:
	lsls r3, r5, #16
	lsrs r3, r3, #16
	cmp r3, r2
	bls .L_02009234
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r3, r4
	lsls r3, r3, #16
	movs r2, #1
	asrs r5, r3, #16
	mov lr, r2
.L_02009234:
	lsls r3, r1, #16
	lsrs r3, r3, #16
	mov r12, r6
	cmp r3, r12
	bcs .L_02009248
	adds r3, #1
	lsls r3, r3, #16
	asrs r1, r3, #16
	movs r3, #1
	mov lr, r3
.L_02009248:
	lsls r3, r1, #16
	lsrs r3, r3, #16
	cmp r3, r12
	bls .L_02009260
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r3, r4
	lsls r3, r3, #16
	movs r2, #1
	asrs r1, r3, #16
	mov lr, r2
.L_02009260:
	lsls r3, r0, #16
	lsrs r3, r3, #16
	mov r12, r7
	cmp r3, r12
	bcs .L_02009274
	adds r3, #1
	lsls r3, r3, #16
	asrs r0, r3, #16
	movs r3, #1
	mov lr, r3
.L_02009274:
	lsls r3, r0, #16
	lsrs r3, r3, #16
	cmp r3, r12
	bls .L_0200928c
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r3, r4
	lsls r3, r3, #16
	movs r2, #1
	asrs r0, r3, #16
	mov lr, r2
.L_0200928c:
	lsls r3, r0, #16
	lsls r2, r1, #16
	lsrs r2, r2, #11
	lsrs r3, r3, #6
	orrs r3, r2
	lsls r2, r5, #16
	lsrs r2, r2, #16
	movs r0, #1
	orrs r2, r3
	add r8, r0
	mov r3, r10
	movs r4, #2
	strh r2, [r3]
	mov r2, r8
	add r10, r4
	add r9, r4
	cmp r2, #15
	bne .L_020091dc
	mov r3, lr
	cmp r3, #0
	beq .L_020092b8
	b .L_020091c0
.L_020092b8:
	movs r0, #30
	bl FieldEvent_Wait
	movs r4, #0
	mov r8, r4
.L_020092c2:
	movs r3, #63
	mov r0, r8
	ands r3, r0
	cmp r3, #0
	bne .L_020092d2
	movs r0, #164
	bl Func_02002054
.L_020092d2:
	mov r2, r8
	cmp r2, #119
	bhi .L_020092e2
	movs r3, #15
	ands r3, r2
	cmp r3, #0
	beq .L_020092fe
	b .L_02009344
.L_020092e2:
	mov r3, r8
	cmp r3, #239
	bhi .L_020092f4
	movs r3, #7
	mov r4, r8
	ands r3, r4
	cmp r3, #0
	beq .L_020092fe
	b .L_02009344
.L_020092f4:
	movs r3, #1
	mov r0, r8
	ands r3, r0
	cmp r3, #0
	bne .L_02009344
.L_020092fe:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	movs r6, #31
	add r1, sp, #56
	movs r3, #7
	str r3, [r1, #4]
	ands r5, r6
	movs r3, #168
	lsls r3, r3, #17
	lsls r5, r5, #16
	adds r2, r0, #0
	adds r5, r5, r3
	movs r3, #0
	str r3, [sp, #0]
	str r3, [sp, #4]
	ands r2, r6
	movs r4, #176
	movs r3, #128
	lsls r4, r4, #16
	lsls r3, r3, #9
	lsls r2, r2, #16
	str r3, [sp, #8]
	str r1, [sp, #12]
	adds r0, r5, #0
	adds r2, r2, r4
	movs r1, #0
	movs r3, #0
	bl Func_0200015c
	movs r0, #1
	bl wait
.L_02009344:
	movs r0, #1
	movs r2, #180
	add r8, r0
	lsls r2, r2, #1
	cmp r8, r2
	bne .L_020092c2
	movs r3, #0
	mov r8, r3
.L_02009354:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	movs r3, #192
	add r1, sp, #16
	lsls r3, r3, #9
	str r3, [r1, #8]
	str r3, [r1, #12]
	movs r3, #7
	str r3, [r1, #4]
	movs r3, #1
	str r3, [r1]
	movs r3, #128
	movs r6, #31
	lsls r3, r3, #9
	str r3, [sp, #0]
	adds r2, r0, #0
	movs r3, #0
	ands r5, r6
	movs r4, #168
	lsls r4, r4, #17
	str r3, [sp, #4]
	ands r2, r6
	lsls r5, r5, #16
	movs r0, #176
	movs r3, #176
	adds r5, r5, r4
	lsls r0, r0, #16
	lsls r3, r3, #12
	lsls r2, r2, #16
	adds r2, r2, r0
	str r3, [sp, #8]
	str r1, [sp, #12]
	movs r3, #0
	adds r0, r5, #0
	movs r1, #0
	bl Func_0200015c
	movs r0, #1
	bl wait
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #16
	bne .L_02009354
	movs r1, #174
	movs r3, #184
	movs r2, #0
	lsls r1, r1, #17
	lsls r3, r3, #16
	movs r0, #159
	bl Import_ActorCreate
	ldr r3, .L_02009410
	adds r5, r0, #0
	str r3, [r5, #108]
	movs r0, #60
	bl FieldEvent_Wait
	adds r0, r5, #0
	movs r1, #1
	bl Func_02001f4c
	movs r0, #40
	bl FieldEvent_Wait
	ldr r3, .L_02009414
	movs r4, #166
	lsls r4, r4, #1
	adds r4, #255
	adds r3, r3, r4
	movs r2, #2
	strb r2, [r3]
	ldr r0, .L_02009418
	movs r1, #4
	bl Party_SetFields1eeAnd1f0
	movs r0, #12
	movs r1, #6
	bl Func_0200200c
	bl Func_02001fb4
	add sp, #100
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009410:
	.4byte Func_020010a8
.L_02009414:
	.4byte gMainRoutineWork
.L_02009418:
	.4byte 0x000000c6
	.section .bss,"aw",%nobits
	.space 0x00000002
	.global Data_02002580
Data_02002580:
	.space 0x00000140
	.global gData_020026c0
gData_020026c0:
