.syntax unified
	.thumb
	.global Func_080ec4d4
	.thumb_func
Func_080ec4d4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #84
	bl EventRuntime_GetControlledOwner
	ldr r0, .L_080ec794
	ldr r1, .L_080ec794
	adds r0, #32
	str r0, [sp, #52]
	ldr r2, .L_080ec798
	ldrh r3, [r1]
	movs r4, #0
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	movs r2, #1
	lsrs r3, r3, #5
	negs r2, r2
	str r3, [sp, #48]
	movs r3, #64
	str r2, [sp, #40]
	str r3, [sp, #36]
	str r4, [sp, #32]
	str r4, [sp, #28]
	bl Func_080ed804
	ldr r5, .L_080ec794
	str r0, [sp, #8]
	movs r0, #198
	lsls r0, r0, #3
	adds r3, r5, r0
	ldr r3, [r3]
	ldr r0, .L_080ec79c
	movs r1, #192
	ands r3, r0
	str r3, [sp, #4]
	lsls r1, r1, #3
	adds r1, #52
	adds r3, r5, r1
	ldr r3, [r3]
	ldr r1, .L_080ec7a0
	ands r3, r0
	str r3, [sp, #0]
	ldr r3, .L_080ec7a4
	movs r2, #31
	ldr r3, [r3]
	lsrs r3, r3, #1
	ands r3, r2
	ldrb r3, [r1, r3]
	str r3, [sp, #44]
	ldr r3, .L_080ec7a8
	ldr r2, [r3]
	cmp r2, #0
	beq .L_080ec630
	ldr r3, [r5, #4]
	ldr r4, .L_080ec7ac
	subs r0, r2, r3
	mov r11, r2
	ldr r6, [r4]
	cmp r0, #0
	bge .L_080ec560
	movs r5, #255
	lsls r5, r5, #8
	adds r5, #255
	adds r0, r0, r5
.L_080ec560:
	asrs r0, r0, #16
	mov r10, r0
	ldr r0, .L_080ec794
	ldr r3, [r0, #8]
	subs r0, r6, r3
	cmp r0, #0
	bge .L_080ec576
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r0, r0, r1
.L_080ec576:
	asrs r0, r0, #16
	mov r8, r0
	mov r2, r10
	mov r4, r8
	mov r3, r8
	muls r3, r4
	mov r0, r10
	muls r0, r2
	adds r0, r0, r3
	ldr r3, .L_080ec7b0
	mov lr, r3
	.2byte 0xf800
	ldr r5, .L_080ec794
	lsls r7, r0, #16
	ldr r3, [r5, #4]
	mov r0, r11
	subs r0, r0, r3
	ldr r3, [r5, #8]
	movs r1, #128
	subs r3, r6, r3
	lsls r1, r1, #15
	mov r10, r0
	mov r8, r3
	cmp r7, r1
	bge .L_080ec5c4
	ldr r7, .L_080ec7b4
	mov r1, r10
	mov lr, r7
	.2byte 0xf800
	mov r1, r8
	adds r5, r0, #0
	mov r0, r8
	mov lr, r7
	.2byte 0xf800
	adds r5, r5, r0
	adds r0, r5, #0
	bl Func_080149e0
	adds r7, r0, #0
.L_080ec5c4:
	adds r1, r7, #0
	cmp r7, #0
	bge .L_080ec5cc
	adds r1, r7, #3
.L_080ec5cc:
	asrs r1, r1, #2
	movs r3, #128
	mov r9, r1
	lsls r3, r3, #12
	cmp r9, r3
	ble .L_080ec5da
	mov r9, r3
.L_080ec5da:
	movs r2, #128
	lsls r2, r2, #8
	cmp r7, r2
	bge .L_080ec5f6
	ldr r4, .L_080ec794
	mov r3, r11
	str r3, [r4, #4]
	str r6, [r4, #8]
	ldr r5, [sp, #28]
	ldr r0, .L_080ec7a8
	ldr r1, .L_080ec7ac
	str r5, [r0]
	str r5, [r1]
	b .L_080ec6b2
.L_080ec5f6:
	cmp r7, r9
	ble .L_080ec620
	ldr r2, .L_080ec7b8
	mov r1, r10
	mov r11, r2
	adds r0, r7, #0
	mov lr, r11
	.2byte 0xf800
	ldr r5, .L_080ec7b4
	mov r1, r9
	mov lr, r5
	.2byte 0xf800
	mov r1, r8
	mov r10, r0
	adds r0, r7, #0
	mov lr, r11
	.2byte 0xf800
	mov r1, r9
	mov lr, r5
	.2byte 0xf800
	mov r8, r0
.L_080ec620:
	ldr r4, .L_080ec794
	ldr r3, [r4, #4]
	add r3, r10
	str r3, [r4, #4]
	ldr r3, [r4, #8]
	add r3, r8
	str r3, [r4, #8]
	b .L_080ec6b2
.L_080ec630:
	ldr r3, .L_080ec7bc
	ldr r1, .L_080ec7c0
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrh r1, [r1, r3]
	lsrs r3, r0, #16
	cmp r1, r3
	beq .L_080ec6b2
	ldr r0, .L_080ec794
	add r5, sp, #72
	ldr r3, [r0, #4]
	str r3, [r5]
	ldr r2, [sp, #28]
	str r2, [r5, #4]
	adds r2, r5, #0
	ldr r3, [r0, #8]
	str r3, [r5, #8]
	ldr r0, [r0, #24]
	bl Vector_AddPolarOffset
	ldr r3, [r5]
	movs r4, #128
	lsls r4, r4, #13
	cmp r3, r4
	bge .L_080ec66e
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r5]
.L_080ec66e:
	ldr r2, .L_080ec7c4
	cmp r3, r2
	ble .L_080ec676
	str r2, [r5]
.L_080ec676:
	ldr r3, [r5, #8]
	cmp r3, #0
	bge .L_080ec682
	ldr r0, [sp, #28]
	movs r3, #0
	str r0, [r5, #8]
.L_080ec682:
	ldr r2, .L_080ec7c8
	cmp r3, r2
	ble .L_080ec68a
	str r2, [r5, #8]
.L_080ec68a:
	ldr r3, [r5]
	ldr r1, .L_080ec794
	ldr r2, .L_080ec7cc
	str r3, [r1, #4]
	ldr r3, [r5, #8]
	str r3, [r1, #8]
	ldr r3, [r5]
	str r3, [r2]
	adds r2, #4
	ldr r3, [r5, #8]
	str r3, [r2]
	movs r2, #192
	ldr r3, [r1, #24]
	lsls r2, r2, #11
	cmp r3, r2
	bge .L_080ec6b2
	movs r4, #128
	lsls r4, r4, #6
	adds r3, r3, r4
	str r3, [r1, #24]
.L_080ec6b2:
	ldr r5, .L_080ec794
	ldr r7, [sp, #8]
	movs r0, #6
	ldrsh r5, [r5, r0]
	ldr r0, .L_080ec794
	mov r10, r5
	movs r1, #10
	ldrsh r0, [r0, r1]
	movs r1, #0
	mov r8, r0
	mov r9, r1
.L_080ec6c8:
	ldrb r3, [r7]
	cmp r3, #0
	bne .L_080ec6fa
	ldr r3, .L_080ec7d0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_080ec6da
	b .L_080ec95e
.L_080ec6da:
	ldr r3, [sp, #8]
	movs r0, #1
	negs r0, r0
	movs r4, #160
	movs r5, #128
	lsls r4, r4, #4
	str r0, [sp, #24]
	add r1, sp, #64
	add r0, sp, #68
	mov r9, r5
	adds r7, r3, r4
	movs r6, #0
	movs r5, #0
	bl Func_080ec1d0
	b .L_080ec826
.L_080ec6fa:
	movs r2, #12
	ldrsh r1, [r7, r2]
	movs r0, #192
	str r1, [sp, #24]
	lsls r0, r0, #1
	movs r4, #14
	ldrsh r3, [r7, r4]
	str r3, [sp, #12]
	ldrb r3, [r7]
	adds r5, r3, r0
	adds r0, r7, #0
	bl Func_080ec298
	adds r6, r0, #0
	cmp r6, #0
	bne .L_080ec71c
	b .L_080ec950
.L_080ec71c:
	cmp r6, #15
	bne .L_080ec7dc
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #222
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080ec730
	b .L_080ec950
.L_080ec730:
	ldr r1, .L_080ec7d4
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r1, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	beq .L_080ec744
	b .L_080ec950
.L_080ec744:
	movs r5, #2
	negs r5, r5
	str r5, [sp, #24]
	movs r0, #158
	lsls r0, r0, #2
	adds r3, r1, r0
	movs r4, #2
	ldrsh r2, [r3, r4]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #85
	muls r2, r3
	movs r6, #2
	movs r5, #0
	cmp r2, #0
	bge .L_080ec76c
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	adds r2, r2, r0
.L_080ec76c:
	asrs r3, r2, #14
	ldr r2, .L_080ec7d8
	movs r4, #159
	adds r3, r3, r2
	str r3, [sp, #68]
	lsls r4, r4, #2
	adds r3, r1, r4
	movs r0, #2
	ldrsh r2, [r3, r0]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #7
	cmp r3, #0
	bge .L_080ec820
	movs r1, #252
	lsls r1, r1, #6
	adds r1, #255
	adds r3, r3, r1
	b .L_080ec820
	.2byte 0x0000
.L_080ec794:
	.4byte Data_0202a000
.L_080ec798:
	.4byte ResourceTableEntries
.L_080ec79c:
	.4byte 0xffff0000
.L_080ec7a0:
	.4byte Data_080f1100
.L_080ec7a4:
	.4byte Data_0300122c
.L_080ec7a8:
	.4byte Data_0202a644
.L_080ec7ac:
	.4byte Data_0202a648
.L_080ec7b0:
	.4byte IwramFillWords + 0x74
.L_080ec7b4:
	.4byte IwramMulQ16
.L_080ec7b8:
	.4byte IwramRatioMulQ14
.L_080ec7bc:
	.4byte gInput
.L_080ec7c0:
	.4byte Data_080f1120
.L_080ec7c4:
	.4byte 0x01ff0000
.L_080ec7c8:
	.4byte 0x016d0000
.L_080ec7cc:
	.4byte Data_0202a64c
.L_080ec7d0:
	.4byte Data_0202a642
.L_080ec7d4:
	.4byte gPartyState
.L_080ec7d8:
	.4byte 0xfffffef3
.L_080ec7dc:
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080ec7e8
	b .L_080ec950
.L_080ec7e8:
	movs r3, #4
	ldrsh r2, [r7, r3]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #85
	muls r2, r3
	movs r5, #1
	cmp r2, #0
	bge .L_080ec802
	movs r4, #252
	lsls r4, r4, #6
	adds r4, #255
	adds r2, r2, r4
.L_080ec802:
	ldr r0, .L_080ec860
	asrs r3, r2, #14
	movs r1, #6
	ldrsh r2, [r7, r1]
	adds r3, r3, r0
	str r3, [sp, #68]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #7
	cmp r3, #0
	bge .L_080ec820
	movs r2, #252
	lsls r2, r2, #6
	adds r2, #255
	adds r3, r3, r2
.L_080ec820:
	asrs r3, r3, #14
	subs r3, #112
	str r3, [sp, #64]
.L_080ec826:
	ldr r4, [sp, #52]
	movs r0, #13
	ldrb r3, [r4, #5]
	negs r0, r0
	adds r2, r0, #0
	lsls r1, r5, #2
	ands r3, r2
	orrs r3, r1
	strb r3, [r4, #5]
	ldr r2, [sp, #48]
	ldr r3, .L_080ec858
	adds r1, r2, r6
	ands r1, r3
	ldr r2, .L_080ec85c
	ldrh r3, [r4, #8]
	ldr r4, [sp, #52]
	ands r3, r2
	orrs r3, r1
	strh r3, [r4, #8]
	ldr r3, .L_080ec864
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	bne .L_080ec8b0
	b .L_080ec868
.L_080ec858:
	.4byte 0x000003ff
.L_080ec85c:
	.4byte 0xfffffc00
.L_080ec860:
	.4byte 0xfffffef3
.L_080ec864:
	.4byte Data_0202a640
.L_080ec868:
	ldr r1, [sp, #4]
	ldr r2, [sp, #68]
	asrs r3, r1, #16
	subs r2, r2, r3
	subs r3, r2, #1
	ldr r4, [sp, #0]
	mov r11, r3
	ldr r3, [sp, #64]
	asrs r1, r4, #16
	subs r3, r3, r1
	adds r2, #3
	subs r6, r3, #1
	cmp r2, #247
	bhi .L_080ec950
	movs r0, #4
	negs r0, r0
	cmp r6, r0
	blt .L_080ec950
	cmp r6, #163
	bgt .L_080ec950
	ldr r4, [sp, #52]
	movs r3, #128
	lsls r3, r3, #1
	mov r1, r11
	adds r3, #255
	ldrh r2, [r4, #6]
	ands r3, r1
	ldr r1, .L_080ec8ac
	adds r0, r4, #0
	ands r2, r1
	orrs r2, r3
	strh r2, [r0, #6]
	strb r6, [r0, #4]
	b .L_080ec8fc
.L_080ec8ac:
	.4byte 0xfffffe00
.L_080ec8b0:
	ldr r2, [sp, #68]
	movs r1, #160
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r0, r3, #4
	subs r0, r0, r3
	lsls r1, r1, #2
	lsls r0, r0, #2
	bl __divsi3
	ldr r3, .L_080ec8f4
	ldr r1, [sp, #52]
	subs r0, #1
	ands r0, r3
	ldr r2, .L_080ec8f8
	ldrh r3, [r1, #6]
	ands r3, r2
	orrs r3, r0
	adds r2, r1, #0
	strh r3, [r2, #6]
	ldr r2, [sp, #64]
	movs r1, #160
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r0, r3, #4
	subs r0, r0, r3
	lsls r0, r0, #2
	lsls r1, r1, #2
	bl __divsi3
	ldr r3, [sp, #52]
	subs r0, #1
	strb r0, [r3, #4]
	b .L_080ec8fc
.L_080ec8f4:
	.4byte 0x000001ff
.L_080ec8f8:
	.4byte 0xfffffe00
.L_080ec8fc:
	ldr r0, [sp, #68]
	ldr r1, [sp, #64]
	mov r4, r10
	subs r2, r0, r4
	mov r4, r8
	subs r3, r1, r4
	adds r4, r2, #0
	muls r4, r2
	str r2, [sp, #60]
	adds r2, r4, #0
	adds r4, r3, #0
	muls r4, r3
	str r3, [sp, #56]
	adds r3, r4, #0
	adds r2, r2, r3
	ldr r3, [sp, #36]
	cmp r2, r3
	bge .L_080ec932
	mov r4, r9
	str r4, [sp, #40]
	ldr r3, [sp, #24]
	ldr r4, [sp, #12]
	str r3, [sp, #20]
	str r4, [sp, #16]
	str r2, [sp, #36]
	str r0, [sp, #32]
	str r1, [sp, #28]
.L_080ec932:
	cmp r5, #0
	bne .L_080ec942
	ldr r3, .L_080ec9d4
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #7
	bhi .L_080ec950
.L_080ec942:
	ldr r0, [sp, #52]
	movs r1, #246
	adds r5, r0, #0
	adds r5, #12
	str r5, [sp, #52]
	bl Func_080140d8
.L_080ec950:
	movs r0, #1
	add r9, r0
	mov r1, r9
	adds r7, #20
	cmp r1, #127
	bgt .L_080ec95e
	b .L_080ec6c8
.L_080ec95e:
	ldr r3, .L_080ec9d8
	ldr r6, [r3]
	cmp r6, #0
	bne .L_080ec9e8
	ldr r2, [sp, #40]
	movs r3, #1
	negs r3, r3
	cmp r2, r3
	beq .L_080ec9e0
	ldr r4, [sp, #36]
	cmp r4, #2
	bgt .L_080ec98c
	ldr r5, [sp, #32]
	ldr r0, .L_080ec9dc
	lsls r3, r5, #16
	str r3, [r0, #4]
	ldr r1, [sp, #28]
	lsls r3, r1, #16
	str r3, [r0, #8]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #24]
	b .L_080ec9c2
.L_080ec98c:
	ldr r2, [sp, #28]
	ldr r4, [sp, #32]
	mov r3, r8
	mov r5, r10
	subs r1, r4, r5
	subs r0, r2, r3
	bl ArcTan2
	adds r1, r0, #0
	ldr r0, .L_080ec9dc
	add r5, sp, #72
	ldr r3, [r0, #4]
	str r6, [r5, #4]
	str r3, [r5]
	lsls r1, r1, #16
	ldr r3, [r0, #8]
	lsrs r1, r1, #16
	str r3, [r5, #8]
	adds r2, r5, #0
	ldr r0, [r0, #24]
	bl Vector_AddPolarOffset
	ldr r3, [r5]
	ldr r1, .L_080ec9dc
	str r3, [r1, #4]
	ldr r3, [r5, #8]
	str r3, [r1, #8]
.L_080ec9c2:
	ldr r2, .L_080ec9dc
	movs r3, #6
	ldrsh r2, [r2, r3]
	ldr r3, .L_080ec9dc
	mov r10, r2
	movs r4, #10
	ldrsh r3, [r3, r4]
	mov r8, r3
	b .L_080ec9e8
.L_080ec9d4:
	.4byte Data_0300122c
.L_080ec9d8:
	.4byte gInput
.L_080ec9dc:
	.4byte Data_0202a000
.L_080ec9e0:
	ldr r4, .L_080eca48
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r4, #24]
.L_080ec9e8:
	ldr r5, [sp, #40]
	movs r0, #1
	negs r0, r0
	cmp r5, r0
	beq .L_080ecaba
	ldr r3, .L_080eca4c
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #7
	bhi .L_080ecaba
	ldr r1, [sp, #52]
	movs r3, #13
	ldrb r2, [r1, #5]
	negs r3, r3
	ands r3, r2
	strb r3, [r1, #5]
	ldr r2, [sp, #48]
	ldr r3, .L_080eca40
	adds r2, #3
	ands r2, r3
	ldr r3, [sp, #52]
	ldr r4, [sp, #52]
	ldrh r1, [r3, #8]
	ldr r3, .L_080eca50
	ands r3, r1
	orrs r3, r2
	strh r3, [r4, #8]
	ldr r3, .L_080eca54
	movs r5, #0
	ldrsh r3, [r3, r5]
	cmp r3, #0
	bne .L_080eca70
	ldr r0, [sp, #4]
	ldr r1, [sp, #32]
	ldr r3, .L_080eca44
	asrs r2, r0, #16
	subs r2, r1, r2
	subs r2, #2
	ldrh r1, [r4, #6]
	ands r2, r3
	ldr r3, .L_080eca58
	ands r3, r1
	b .L_080eca5c
.L_080eca40:
	.4byte 0x000003ff
.L_080eca44:
	.4byte 0x000001ff
.L_080eca48:
	.4byte Data_0202a000
.L_080eca4c:
	.4byte Data_0300122c
.L_080eca50:
	.4byte 0xfffffc00
.L_080eca54:
	.4byte Data_0202a640
.L_080eca58:
	.4byte 0xfffffe00
.L_080eca5c:
	orrs r3, r2
	ldr r2, [sp, #52]
	strh r3, [r2, #6]
	ldr r4, [sp, #0]
	ldr r5, [sp, #28]
	asrs r3, r4, #16
	subs r3, r5, r3
	subs r3, #2
	strb r3, [r2, #4]
	b .L_080ecab2
.L_080eca70:
	ldr r0, [sp, #32]
	movs r5, #160
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r0, r3, #4
	subs r0, r0, r3
	lsls r5, r5, #2
	adds r1, r5, #0
	lsls r0, r0, #2
	bl __divsi3
	ldr r3, .L_080ecac4
	ldr r1, [sp, #52]
	subs r0, #2
	ldrh r2, [r1, #6]
	ands r0, r3
	ldr r3, .L_080ecac8
	ands r3, r2
	orrs r3, r0
	adds r2, r1, #0
	strh r3, [r2, #6]
	ldr r4, [sp, #28]
	adds r1, r5, #0
	lsls r3, r4, #2
	adds r3, r3, r4
	lsls r0, r3, #4
	subs r0, r0, r3
	lsls r0, r0, #2
	bl __divsi3
	ldr r5, [sp, #52]
	subs r0, #2
	strb r0, [r5, #4]
.L_080ecab2:
	ldr r0, [sp, #52]
	movs r1, #246
	bl Func_080140d8
.L_080ecaba:
	ldr r0, .L_080ecacc
	ldr r3, .L_080ecad0
	str r0, [sp, #52]
	b .L_080ecad4
	.2byte 0x0000
.L_080ecac4:
	.4byte 0x000001ff
.L_080ecac8:
	.4byte 0xfffffe00
.L_080ecacc:
	.4byte Data_0202a620
.L_080ecad0:
	.4byte Data_0202a640
.L_080ecad4:
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_080ecb18
	ldrb r2, [r0, #7]
	movs r3, #63
	negs r3, r3
	ands r3, r2
	strb r3, [r0, #7]
	ldr r3, [sp, #4]
	mov r4, r10
	asrs r2, r3, #16
	ldr r3, .L_080ecb10
	subs r2, r4, r2
	subs r2, #17
	ldrh r1, [r0, #6]
	ands r2, r3
	ldr r3, .L_080ecb14
	ldr r5, [sp, #52]
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #6]
	ldr r0, [sp, #0]
	mov r1, r8
	asrs r3, r0, #16
	subs r3, r1, r3
	adds r3, #1
	strb r3, [r5, #4]
	b .L_080ecb5a
	.2byte 0x0000
.L_080ecb10:
	.4byte 0x000001ff
.L_080ecb14:
	.4byte 0xfffffe00
.L_080ecb18:
	mov r2, r10
	lsls r3, r2, #2
	add r3, r10
	lsls r0, r3, #4
	movs r5, #160
	subs r0, r0, r3
	lsls r5, r5, #2
	adds r1, r5, #0
	lsls r0, r0, #2
	bl __divsi3
	ldr r3, .L_080ecb68
	subs r0, #17
	ands r0, r3
	ldr r3, [sp, #52]
	ldr r4, [sp, #52]
	ldrh r2, [r3, #6]
	ldr r3, .L_080ecb6c
	adds r1, r5, #0
	ands r3, r2
	orrs r3, r0
	mov r0, r8
	strh r3, [r4, #6]
	lsls r3, r0, #2
	add r3, r8
	lsls r0, r3, #4
	subs r0, r0, r3
	lsls r0, r0, #2
	bl __divsi3
	ldr r1, [sp, #52]
	adds r0, #1
	strb r0, [r1, #4]
.L_080ecb5a:
	ldr r0, [sp, #52]
	movs r1, #246
	bl Func_080140d8
	ldr r4, .L_080ecb70
	ldr r5, [sp, #40]
	b .L_080ecb74
.L_080ecb68:
	.4byte 0x000001ff
.L_080ecb6c:
	.4byte 0xfffffe00
.L_080ecb70:
	.4byte Data_0202a000
.L_080ecb74:
	movs r2, #18
	ldrsh r3, [r4, r2]
	cmp r3, r5
	bne .L_080ecb7e
	b .L_080ecc8e
.L_080ecb7e:
	ldr r5, .L_080ecdec
	movs r0, #0
	ldrsh r3, [r5, r0]
	cmp r3, #0
	bne .L_080ecb9a
	ldr r0, [r4, #28]
	bl RenderOutput_PrepareForRedrawFar
	ldr r2, .L_080ecdf0
	movs r3, #1
	strh r3, [r5]
	movs r3, #6
	strh r3, [r2]
	b .L_080ecd44
.L_080ecb9a:
	ldr r2, .L_080ecdf0
	ldrh r3, [r2]
	subs r3, #1
	strh r3, [r2]
	lsls r3, r3, #16
	cmp r3, #0
	ble .L_080ecbaa
	b .L_080ecd44
.L_080ecbaa:
	add r1, sp, #40
	ldr r2, .L_080ecdf4
	ldrh r1, [r1]
	movs r3, #0
	strh r3, [r5]
	strh r1, [r2, #18]
	ldr r2, [sp, #40]
	subs r3, #1
	cmp r2, r3
	bne .L_080ecbc0
	b .L_080ecd44
.L_080ecbc0:
	ldr r4, [sp, #20]
	cmp r4, r3
	bne .L_080ecbcc
	ldr r5, .L_080ecdf8
	str r5, [sp, #20]
	b .L_080ecbea
.L_080ecbcc:
	ldr r0, [sp, #20]
	movs r1, #2
	negs r1, r1
	cmp r0, r1
	bne .L_080ecbdc
	ldr r2, .L_080ecdfc
	str r2, [sp, #20]
	b .L_080ecbea
.L_080ecbdc:
	ldr r0, [sp, #20]
	ldr r1, [sp, #16]
	bl Func_080ca280
	ldr r3, .L_080ece00
	adds r0, r0, r3
	str r0, [sp, #20]
.L_080ecbea:
	add r1, sp, #60
	add r2, sp, #56
	ldr r0, [sp, #20]
	bl UiText_MeasureResourceEntriesFar
	ldr r3, .L_080ece04
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	bne .L_080ecc16
	ldr r5, [sp, #32]
	ldr r1, [sp, #4]
	ldr r0, [sp, #28]
	ldr r2, [sp, #0]
	asrs r3, r1, #16
	subs r5, #1
	subs r5, r5, r3
	subs r0, #11
	asrs r3, r2, #16
	subs r0, r0, r3
	mov r10, r5
	b .L_080ecc44
.L_080ecc16:
	ldr r4, [sp, #32]
	movs r5, #160
	lsls r3, r4, #2
	adds r3, r3, r4
	lsls r0, r3, #4
	subs r0, r0, r3
	lsls r5, r5, #2
	adds r1, r5, #0
	lsls r0, r0, #2
	bl __divsi3
	subs r0, #1
	mov r10, r0
	ldr r0, [sp, #28]
	adds r1, r5, #0
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r0, r3, #4
	subs r0, r0, r3
	lsls r0, r0, #2
	bl __divsi3
	subs r0, #11
.L_080ecc44:
	mov r8, r0
	ldr r4, [sp, #60]
	mov r1, r10
	adds r3, r1, r4
	cmp r3, #239
	ble .L_080ecc5c
	movs r3, #232
	movs r2, #10
	subs r3, r3, r4
	negs r2, r2
	mov r10, r3
	add r8, r2
.L_080ecc5c:
	mov r3, r8
	cmp r3, #0
	bge .L_080ecc66
	movs r4, #0
	mov r8, r4
.L_080ecc66:
	ldr r5, .L_080ecdf4
	ldr r0, [sp, #20]
	ldr r1, [r5, #28]
	mov r2, r10
	mov r3, r8
	bl UiText_DrawResourceFar
	ldr r3, .L_080ece08
	ldr r2, [sp, #60]
	mov r0, r10
	strh r2, [r3]
	ldr r2, [sp, #56]
	adds r3, #2
	strh r2, [r3]
	subs r3, #6
	strh r0, [r3]
	mov r1, r8
	adds r3, #2
	strh r1, [r3]
	b .L_080ecd44
.L_080ecc8e:
	ldr r2, [sp, #40]
	movs r3, #1
	negs r3, r3
	cmp r2, r3
	beq .L_080ecd44
	ldr r3, .L_080ece08
	movs r4, #0
	ldrsh r3, [r3, r4]
	str r3, [sp, #60]
	ldr r3, .L_080ece0c
	movs r5, #0
	ldrsh r3, [r3, r5]
	str r3, [sp, #56]
	ldr r3, .L_080ece04
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	bne .L_080ecccc
	ldr r1, [sp, #32]
	ldr r4, [sp, #4]
	ldr r2, [sp, #28]
	ldr r5, [sp, #0]
	asrs r3, r4, #16
	subs r1, #1
	subs r1, r1, r3
	subs r2, #11
	asrs r3, r5, #16
	subs r2, r2, r3
	mov r10, r1
	mov r8, r2
	b .L_080eccfc
.L_080ecccc:
	ldr r0, [sp, #32]
	movs r5, #160
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r0, r3, #4
	subs r0, r0, r3
	lsls r5, r5, #2
	adds r1, r5, #0
	lsls r0, r0, #2
	bl __divsi3
	ldr r1, [sp, #28]
	subs r0, #1
	lsls r3, r1, #2
	adds r3, r3, r1
	mov r10, r0
	lsls r0, r3, #4
	subs r0, r0, r3
	lsls r0, r0, #2
	adds r1, r5, #0
	bl __divsi3
	subs r0, #11
	mov r8, r0
.L_080eccfc:
	ldr r4, [sp, #60]
	mov r2, r10
	adds r3, r2, r4
	cmp r3, #239
	ble .L_080ecd12
	movs r3, #232
	subs r3, r3, r4
	mov r10, r3
	movs r3, #10
	negs r3, r3
	add r8, r3
.L_080ecd12:
	mov r4, r8
	cmp r4, #0
	bge .L_080ecd1c
	movs r5, #0
	mov r8, r5
.L_080ecd1c:
	ldr r7, .L_080ece10
	mov r2, r10
	movs r0, #0
	ldrsh r1, [r7, r0]
	ldr r5, .L_080ece14
	subs r1, r2, r1
	str r1, [sp, #60]
	mov r4, r8
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, .L_080ecdf4
	subs r2, r4, r2
	ldr r0, [r3, #28]
	str r2, [sp, #56]
	bl Func_080ec484
	mov r4, r10
	mov r0, r8
	strh r4, [r7]
	strh r0, [r5]
.L_080ecd44:
	ldr r0, .L_080ece18
	ldr r1, .L_080ece1c
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_080ecd76
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r0
	strh r2, [r0]
	movs r2, #252
	adds r3, #4
	lsls r2, r2, #6
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_080ecd76:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r3, [r0]
	cmp r3, #31
	bgt .L_080ecdac
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r0]
	ldr r5, [sp, #44]
	movs r3, #16
	lsls r2, r2, #2
	subs r3, r3, r5
	adds r2, r2, r0
	lsls r3, r3, #8
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
.L_080ecdac:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_080ecdda
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r0
	adds r3, #4
	strh r2, [r0]
	movs r2, #0
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #84
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_080ecdda:
	strh r4, [r1]
	add sp, #84
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ecdec:
	.4byte Data_0202a654
.L_080ecdf0:
	.4byte Data_0202a656
.L_080ecdf4:
	.4byte Data_0202a000
.L_080ecdf8:
	.4byte 0x00000e29
.L_080ecdfc:
	.4byte 0x00000e2b
.L_080ece00:
	.4byte 0x00000e58
.L_080ece04:
	.4byte Data_0202a640
.L_080ece08:
	.4byte Data_0202a63c
.L_080ece0c:
	.4byte Data_0202a63e
.L_080ece10:
	.4byte Data_0202a638
.L_080ece14:
	.4byte Data_0202a63a
.L_080ece18:
	.4byte gIoWriteQueue
.L_080ece1c:
	.4byte 0x04000208
