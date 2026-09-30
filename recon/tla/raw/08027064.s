.syntax unified
	.thumb
	.global Func_08027064
	.thumb_func
Func_08027064:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r0
	ldr r0, [r0, #80]
	sub sp, #120
	movs r2, #0
	movs r1, #2
	str r0, [sp, #28]
	str r1, [sp, #16]
	str r2, [sp, #12]
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #108]
	ldr r1, .L_08027358
	str r4, [sp, #8]
	movs r0, #143
	ldr r3, [r3, #32]
	str r2, [sp, #36]
	str r3, [sp, #4]
	ldr r3, .L_0802735c
	str r2, [sp, #32]
	lsls r0, r0, #2
	adds r3, r3, r0
	ldrh r2, [r3]
	ldr r3, [r1]
	ands r3, r2
	cmp r3, #0
	beq .L_080270ba
	movs r3, #192
	lsls r3, r3, #9
	mov r1, r8
	str r3, [r1, #48]
	movs r3, #128
	lsls r3, r3, #7
	movs r2, #5
	str r3, [r1, #52]
	str r2, [sp, #16]
	b .L_080270c8
.L_080270ba:
	movs r3, #128
	lsls r3, r3, #9
	mov r4, r8
	str r3, [r4, #48]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r4, #52]
.L_080270c8:
	ldr r3, .L_08027358
	ldr r1, .L_08027360
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
	movs r1, #255
	lsls r3, r3, #16
	lsls r1, r1, #8
	lsrs r7, r3, #16
	adds r1, #255
	str r3, [sp, #0]
	cmp r7, r1
	bne .L_080270f2
	ldr r2, [sp, #36]
	movs r3, #4
	orrs r2, r3
	str r2, [sp, #36]
	b .L_08027664
.L_080270f2:
	movs r3, #0
	str r3, [sp, #36]
	mov r0, r8
	ldr r3, [r0, #8]
	add r4, sp, #108
	str r3, [r4]
	mov r10, r4
	ldr r3, [r0, #12]
	mov r2, r10
	str r3, [r4, #4]
	adds r1, r7, #0
	ldr r3, [r0, #16]
	movs r0, #128
	str r3, [r4, #8]
	lsls r0, r0, #12
	bl Vector_AddPolarOffset
	mov r3, r8
	adds r3, #35
	ldrb r2, [r3]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	bne .L_08027124
	b .L_0802726e
.L_08027124:
	mov r1, r10
	ldr r3, [r1]
	add r6, sp, #96
	str r3, [r6]
	movs r5, #128
	ldr r3, [r1, #4]
	movs r2, #128
	str r3, [r6, #4]
	lsls r5, r5, #10
	ldr r3, [r1, #8]
	lsls r2, r2, #7
	adds r1, r7, r2
	adds r0, r5, #0
	str r3, [r6, #8]
	adds r2, r6, #0
	bl Vector_AddPolarOffset
	movs r3, #84
	mov r4, r10
	add r3, sp
	mov r9, r3
	ldr r3, [r4]
	mov r0, r9
	str r3, [r0]
	ldr r2, .L_08027364
	ldr r3, [r4, #4]
	adds r1, r7, r2
	str r3, [r0, #4]
	mov r2, r9
	ldr r3, [r4, #8]
	mov r11, r9
	str r3, [r0, #8]
	adds r0, r5, #0
	bl Vector_AddPolarOffset
	movs r3, #0
	str r3, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	movs r4, #128
	ldr r7, [r3, #20]
	ldr r5, .L_08027368
	lsls r4, r4, #12
	mov r12, r4
.L_0802717c:
	ldr r3, [r7]
	cmp r3, #0
	beq .L_0802723c
	adds r3, r7, #0
	adds r3, #35
	ldrb r2, [r3]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_0802723c
	cmp r7, r8
	beq .L_0802723c
	mov r0, r8
	ldr r3, [r7, #12]
	ldr r2, [r0, #12]
	ldr r1, .L_0802736c
	subs r2, r2, r3
	adds r3, r2, r1
	cmp r3, #0
	bge .L_080271aa
	movs r3, #128
	lsls r3, r3, #13
	subs r3, r3, r2
.L_080271aa:
	cmp r3, r5
	bgt .L_0802723c
	mov r2, r8
	ldr r4, [r2, #8]
	ldr r1, [r7, #8]
	subs r3, r4, r1
	add r3, r12
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_080271d4
	ldr r3, [r2, #16]
	ldr r2, [r7, #16]
	movs r0, #128
	subs r3, r3, r2
	lsls r0, r0, #12
	adds r3, r3, r0
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_080271d4
	movs r2, #1
	str r2, [sp, #12]
.L_080271d4:
	mov r0, r10
	ldr r3, [r0]
	subs r3, r3, r1
	add r3, r12
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_0802723c
	ldr r3, [r0, #8]
	ldr r2, [r7, #16]
	movs r0, #128
	subs r3, r3, r2
	lsls r0, r0, #12
	adds r3, r3, r0
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_0802723c
	ldr r3, [r6]
	subs r3, r3, r1
	adds r3, r3, r0
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_0802723c
	ldr r3, [r6, #8]
	subs r3, r3, r2
	adds r3, r3, r0
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_0802723c
	mov r0, r11
	ldr r3, [r0]
	subs r3, r3, r1
	movs r1, #128
	lsls r1, r1, #12
	adds r3, r3, r1
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_0802723c
	ldr r3, [r0, #8]
	subs r3, r3, r2
	adds r3, r3, r1
	asrs r3, r3, #20
	cmp r3, #0
	bne .L_0802723c
	ldr r2, [r7, #56]
	movs r3, #128
	lsls r3, r3, #24
	cmp r2, r3
	bne .L_0802723c
	ldr r3, [r7, #64]
	cmp r3, r2
	bne .L_0802723c
	b .L_080274c6
.L_0802723c:
	ldr r4, [sp, #20]
	adds r7, #128
	adds r4, #1
	str r4, [sp, #20]
	cmp r4, #63
	ble .L_0802717c
	ldr r0, [sp, #12]
	cmp r0, #0
	beq .L_0802726e
	mov r0, r8
	mov r1, r10
	bl Func_0802d87c
	cmp r0, #0
	bne .L_0802726e
	ldr r2, [sp, #0]
	movs r1, #0
	asrs r2, r2, #16
	str r2, [sp, #24]
	str r1, [sp, #36]
	mov r3, r8
	lsls r2, r2, #16
	ldr r4, [r3, #8]
	mov r11, r2
	b .L_080274e0
.L_0802726e:
	ldr r3, .L_08027370
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08027290
	ldr r3, .L_08027358
	movs r2, #128
	ldr r3, [r3]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08027290
	ldr r0, [sp, #0]
	movs r4, #0
	asrs r0, r0, #16
	str r4, [sp, #36]
	str r0, [sp, #24]
	b .L_08027664
.L_08027290:
	mov r0, r8
	mov r1, r10
	bl Func_0802d87c
	cmp r0, #0
	bne .L_0802737c
	mov r1, r8
	ldr r3, [r1, #8]
	add r5, sp, #96
	str r3, [r5]
	movs r7, #128
	ldr r3, [r1, #12]
	lsls r7, r7, #12
	str r3, [r5, #4]
	adds r0, r7, #0
	ldr r3, [r1, #16]
	str r3, [r5, #8]
	ldr r2, [sp, #0]
	movs r3, #128
	lsrs r6, r2, #16
	lsls r3, r3, #5
	adds r1, r6, r3
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_0802737c
	mov r4, r8
	ldr r3, [r4, #8]
	ldr r0, .L_08027374
	str r3, [r5]
	adds r1, r6, r0
	ldr r3, [r4, #12]
	adds r0, r7, #0
	str r3, [r5, #4]
	adds r2, r5, #0
	ldr r3, [r4, #16]
	str r3, [r5, #8]
	bl Vector_AddPolarOffset
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_0802737c
	mov r1, r8
	ldr r3, [r1, #8]
	movs r2, #128
	str r3, [r5]
	lsls r2, r2, #6
	ldr r3, [r1, #12]
	adds r0, r7, #0
	str r3, [r5, #4]
	ldr r3, [r1, #16]
	adds r1, r6, r2
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_0802737c
	mov r4, r8
	ldr r3, [r4, #8]
	ldr r0, .L_08027378
	str r3, [r5]
	adds r1, r6, r0
	ldr r3, [r4, #12]
	adds r0, r7, #0
	str r3, [r5, #4]
	adds r2, r5, #0
	ldr r3, [r4, #16]
	str r3, [r5, #8]
	bl Vector_AddPolarOffset
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_0802737c
	ldr r1, [sp, #0]
	movs r3, #84
	asrs r1, r1, #16
	str r1, [sp, #24]
	mov r2, r8
	add r3, sp
	lsls r1, r1, #16
	ldr r4, [r2, #8]
	mov r9, r3
	mov r11, r1
	b .L_080274e0
.L_08027358:
	.4byte gInput
.L_0802735c:
	.4byte gPartyState
.L_08027360:
	.4byte Data_0802ec5c
.L_08027364:
	.4byte 0xffffc000
.L_08027368:
	.4byte 0x0007ffff
.L_0802736c:
	.4byte 0xfff00000
.L_08027370:
	.4byte Data_03001238
.L_08027374:
	.4byte 0xfffff000
.L_08027378:
	.4byte 0xffffe000
.L_0802737c:
	ldr r0, [sp, #0]
	movs r4, #40
	lsrs r3, r0, #16
	movs r1, #128
	ldr r0, .L_080276d4
	add r4, sp
	lsls r1, r1, #5
	mov r9, r4
	adds r2, r3, r1
	strh r2, [r4]
	mov r1, r9
	adds r2, r3, r0
	movs r4, #128
	strh r2, [r1, #2]
	lsls r4, r4, #6
	ldr r1, .L_080276d8
	adds r2, r3, r4
	mov r0, r9
	strh r2, [r0, #4]
	movs r0, #192
	adds r2, r3, r1
	mov r4, r9
	lsls r0, r0, #6
	strh r2, [r4, #6]
	mov r1, r9
	adds r2, r3, r0
	strh r2, [r1, #8]
	ldr r2, .L_080276dc
	movs r0, #0
	adds r3, r3, r2
	strh r3, [r4, #10]
	str r0, [sp, #20]
	mov r7, r10
.L_080273be:
	ldr r1, [sp, #20]
	mov r2, r9
	lsls r3, r1, #1
	ldrsh r2, [r2, r3]
	mov r0, r8
	str r2, [sp, #24]
	lsls r2, r2, #16
	ldr r3, [r0, #8]
	lsrs r6, r2, #16
	str r3, [r7]
	adds r1, r6, #0
	ldr r3, [r0, #12]
	mov r11, r2
	str r3, [r7, #4]
	adds r2, r7, #0
	ldr r3, [r0, #16]
	movs r0, #128
	lsls r0, r0, #12
	str r3, [r7, #8]
	bl Vector_AddPolarOffset
	mov r0, r8
	adds r1, r7, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_0802749a
	mov r1, r8
	ldr r3, [r1, #8]
	add r5, sp, #96
	str r3, [r5]
	movs r2, #128
	ldr r3, [r1, #12]
	lsls r2, r2, #5
	str r3, [r5, #4]
	movs r0, #128
	ldr r3, [r1, #16]
	lsls r0, r0, #12
	adds r1, r6, r2
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_0802749a
	mov r4, r8
	ldr r3, [r4, #8]
	ldr r0, .L_080276d4
	str r3, [r5]
	adds r1, r6, r0
	ldr r3, [r4, #12]
	movs r0, #128
	str r3, [r5, #4]
	lsls r0, r0, #12
	ldr r3, [r4, #16]
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffset
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_0802749a
	mov r1, r8
	ldr r3, [r1, #8]
	movs r2, #128
	str r3, [r5]
	lsls r2, r2, #6
	ldr r3, [r1, #12]
	movs r0, #128
	str r3, [r5, #4]
	lsls r0, r0, #12
	ldr r3, [r1, #16]
	adds r1, r6, r2
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_0802749a
	mov r4, r8
	ldr r3, [r4, #8]
	ldr r0, .L_080276d8
	str r3, [r5]
	adds r1, r6, r0
	ldr r3, [r4, #12]
	movs r0, #128
	str r3, [r5, #4]
	lsls r0, r0, #12
	ldr r3, [r4, #16]
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffset
	mov r0, r8
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	beq .L_080274d6
.L_0802749a:
	ldr r1, [sp, #20]
	adds r1, #1
	str r1, [sp, #20]
	cmp r1, #6
	blt .L_080273be
	mov r3, r8
	ldr r2, [r3, #8]
	mov r4, r10
	str r2, [r4]
	mov r0, r8
	ldr r3, [r3, #12]
	str r3, [r4, #4]
	ldr r3, [r0, #16]
	str r3, [r4, #8]
	ldr r1, [sp, #36]
	movs r3, #1
	adds r4, r2, #0
	movs r2, #84
	orrs r1, r3
	add r2, sp
	str r1, [sp, #36]
	b .L_080274de
.L_080274c6:
	ldr r0, [sp, #0]
	movs r3, #0
	asrs r0, r0, #16
	str r0, [sp, #24]
	lsls r0, r0, #16
	str r3, [sp, #36]
	mov r11, r0
	b .L_080274e0
.L_080274d6:
	mov r1, r8
	movs r2, #84
	ldr r4, [r1, #8]
	add r2, sp
.L_080274de:
	mov r9, r2
.L_080274e0:
	mov r3, r9
	str r4, [r3]
	mov r4, r8
	ldr r3, [r4, #12]
	mov r0, r9
	str r3, [r0, #4]
	mov r2, r11
	ldr r3, [r4, #16]
	lsrs r1, r2, #16
	str r3, [r0, #8]
	movs r0, #128
	lsls r0, r0, #11
	mov r2, r9
	bl Vector_AddPolarOffset
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #20]
	movs r3, #63
	str r3, [sp, #20]
	adds r6, r7, #0
	adds r6, #8
.L_0802750c:
	mov r4, r8
	ldrh r3, [r4, #32]
	subs r1, r3, #2
	ldr r3, [r7]
	cmp r3, #0
	bne .L_0802751a
	b .L_0802763a
.L_0802751a:
	adds r3, r7, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r0, #1
	mov r11, r0
	mov r3, r11
	ands r3, r2
	cmp r3, #0
	bne .L_0802752e
	b .L_0802763a
.L_0802752e:
	cmp r7, r8
	bne .L_08027534
	b .L_0802763a
.L_08027534:
	ldrh r3, [r6, #24]
	adds r0, r6, #0
	subs r3, #2
	mov r2, r9
	bl Func_08026f80
	cmp r0, #0
	blt .L_0802763a
	ldr r3, [r6, #80]
	ldr r2, .L_080276e0
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #2
	cmp r3, r1
	bne .L_08027632
	mov r2, r8
	ldr r3, [r2, #16]
	ldr r0, [r6, #8]
	ldr r1, [r6]
	subs r0, r0, r3
	ldr r3, [r2, #8]
	subs r1, r1, r3
	bl ArcTan2
	ldr r3, [r6]
	add r5, sp, #96
	str r3, [r5]
	lsls r0, r0, #16
	ldr r3, [r6, #4]
	adds r2, r5, #0
	str r3, [r5, #4]
	ldr r3, [r6, #8]
	str r3, [r5, #8]
	asrs r3, r0, #16
	lsrs r0, r0, #16
	mov r10, r0
	movs r0, #128
	lsls r0, r0, #7
	mov r1, r10
	str r3, [sp, #24]
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_08024f20
	cmp r0, #0
	bne .L_08027632
	ldr r3, [r6]
	movs r0, #160
	str r3, [r5]
	lsls r0, r0, #12
	ldr r3, [r6, #4]
	mov r1, r10
	str r3, [r5, #4]
	adds r2, r5, #0
	ldr r3, [r6, #8]
	str r3, [r5, #8]
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08027632
	ldr r3, [r6]
	movs r1, #128
	str r3, [r5]
	lsls r1, r1, #5
	ldr r3, [r6, #4]
	movs r0, #160
	str r3, [r5, #4]
	add r1, r10
	ldr r3, [r6, #8]
	lsls r0, r0, #12
	str r3, [r5, #8]
	adds r2, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08027632
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08027632
	ldr r3, [r6]
	ldr r1, .L_080276d4
	str r3, [r5]
	movs r0, #160
	ldr r3, [r6, #4]
	add r1, r10
	str r3, [r5, #4]
	lsls r0, r0, #12
	ldr r3, [r6, #8]
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffset
	adds r0, r7, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_08027632
	movs r0, #128
	lsls r0, r0, #7
	mov r1, r10
	adds r2, r6, #0
	bl Vector_AddPolarOffset
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r6, #48]
	str r3, [r6, #52]
	str r3, [r6, #56]
	ldr r4, [sp, #32]
	mov r0, r11
	orrs r4, r0
	str r4, [sp, #32]
	b .L_0802763a
.L_08027632:
	ldr r1, [sp, #36]
	movs r3, #2
	orrs r1, r3
	str r1, [sp, #36]
.L_0802763a:
	ldr r2, [sp, #20]
	adds r6, #128
	subs r2, #1
	str r2, [sp, #20]
	adds r7, #128
	cmp r2, #0
	blt .L_0802764a
	b .L_0802750c
.L_0802764a:
	ldr r3, [sp, #36]
	cmp r3, #0
	bne .L_08027664
	ldr r4, [sp, #32]
	cmp r4, #0
	beq .L_08027664
	movs r3, #128
	lsls r3, r3, #7
	mov r0, r8
	str r3, [r0, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r0, #52]
.L_08027664:
	ldr r1, [sp, #8]
	cmp r1, #0
	beq .L_0802769a
	ldr r3, [sp, #36]
	movs r2, #3
	ands r2, r3
	cmp r2, #0
	beq .L_08027682
	movs r4, #194
	lsls r4, r4, #1
	adds r2, r1, r4
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0802768c
.L_08027682:
	ldr r0, [sp, #8]
	movs r1, #194
	lsls r1, r1, #1
	adds r3, r0, r1
	strh r2, [r3]
.L_0802768c:
	ldr r3, .L_080276e4
	ldr r4, [sp, #8]
	ldr r3, [r3]
	movs r0, #195
	lsls r0, r0, #1
	adds r2, r4, r0
	strh r3, [r2]
.L_0802769a:
	ldr r1, [sp, #32]
	cmp r1, #0
	beq .L_080276aa
	mov r0, r8
	movs r1, #8
	bl ObjectDispatch_ApplyArgumentToChildren
	b .L_080276f4
.L_080276aa:
	ldr r2, [sp, #36]
	cmp r2, #0
	beq .L_080276ec
	ldr r3, .L_080276e8
	movs r4, #133
	lsls r4, r4, #2
	adds r3, r3, r4
	ldr r0, [r3]
	bl Owner_GetState
	movs r1, #56
	ldrsh r3, [r0, r1]
	movs r5, #9
	cmp r3, #0
	bne .L_080276ca
	movs r5, #22
.L_080276ca:
	mov r0, r8
	adds r1, r5, #0
	bl ObjectDispatch_ApplyArgumentToChildren
	b .L_080276f4
.L_080276d4:
	.4byte 0xfffff000
.L_080276d8:
	.4byte 0xffffe000
.L_080276dc:
	.4byte 0xffffd000
.L_080276e0:
	.4byte 0xff000200
.L_080276e4:
	.4byte gInput
.L_080276e8:
	.4byte gPartyState
.L_080276ec:
	mov r0, r8
	ldr r1, [sp, #16]
	bl ObjectDispatch_ApplyArgumentToChildren
.L_080276f4:
	ldr r2, [sp, #36]
	cmp r2, #0
	beq .L_0802774e
	movs r3, #128
	mov r4, r8
	lsls r3, r3, #24
	str r3, [r4, #56]
	str r3, [r4, #60]
	str r3, [r4, #64]
	movs r3, #0
	str r3, [r4, #36]
	str r3, [r4, #44]
	movs r3, #3
	ands r3, r2
	cmp r3, #0
	beq .L_08027738
	ldr r0, [sp, #0]
	ldrh r1, [r4, #6]
	lsrs r3, r0, #16
	subs r3, r3, r1
	lsls r3, r3, #16
	movs r2, #128
	asrs r3, r3, #16
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_0802772a
	adds r3, r2, #0
.L_0802772a:
	ldr r2, .L_0802789c
	cmp r3, r2
	bge .L_08027732
	adds r3, r2, #0
.L_08027732:
	adds r3, r1, r3
	mov r1, r8
	strh r3, [r1, #6]
.L_08027738:
	movs r2, #100
	add r2, r8
	mov r10, r2
	movs r3, #0
	mov r4, r10
	mov r2, r8
	strh r3, [r4]
	adds r2, #102
	movs r3, #2
	strh r3, [r2]
	b .L_080277a8
.L_0802774e:
	add r3, sp, #108
	ldr r2, [r3, #4]
	ldr r1, [r3]
	mov r0, r8
	ldr r3, [r3, #8]
	bl Object_SetMoveTarget
	mov r0, r8
	ldr r1, [r0, #36]
	ldr r6, .L_080278a0
	adds r0, r1, #0
	mov lr, r6
	.2byte 0xf800
	mov r2, r8
	ldr r1, [r2, #44]
	adds r5, r0, #0
	adds r0, r1, #0
	mov lr, r6
	.2byte 0xf800
	adds r5, r5, r0
	adds r0, r5, #0
	bl Func_080149e0
	ldr r3, [sp, #36]
	mov r4, r8
	str r3, [r4, #36]
	str r3, [r4, #44]
	ldr r2, [sp, #24]
	lsls r1, r2, #16
	mov r2, r8
	adds r2, #36
	lsrs r1, r1, #16
	bl Vector_AddPolarOffset
	movs r3, #100
	add r3, r8
	mov r10, r3
	ldrh r2, [r3]
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	beq .L_080277a8
	subs r3, r2, #1
	mov r0, r10
	strh r3, [r0]
.L_080277a8:
	ldr r1, [sp, #4]
	movs r3, #12
	ldrb r2, [r1, #23]
	ands r3, r2
	cmp r3, #0
	bne .L_080277b6
	b .L_080278c4
.L_080277b6:
	mov r2, r8
	adds r2, #34
	ldrb r3, [r2]
	cmp r3, #2
	bhi .L_080277d2
	adds r2, r3, #0
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [r1, r3]
	b .L_080277d4
.L_080277d2:
	ldr r2, .L_080278a4
.L_080277d4:
	mov r4, r8
	ldr r3, [r4, #8]
	cmp r3, #0
	bge .L_080277e0
	ldr r0, .L_080278a8
	adds r3, r3, r0
.L_080277e0:
	mov r4, r8
	asrs r1, r3, #20
	ldr r3, [r4, #16]
	cmp r3, #0
	bge .L_080277ee
	ldr r0, .L_080278a8
	adds r3, r3, r0
.L_080277ee:
	asrs r3, r3, #20
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	ldr r1, [sp, #28]
	adds r2, r2, r3
	ldrb r2, [r2, #3]
	ldrb r5, [r1, #26]
	movs r3, #16
	ands r2, r3
	adds r1, r5, #0
	cmp r2, #0
	beq .L_08027872
	movs r6, #8
	adds r3, r5, #0
	ands r3, r6
	cmp r3, #0
	bne .L_0802782e
	ldr r3, [sp, #28]
	ldr r4, [sp, #28]
	ldrb r2, [r3, #17]
	movs r3, #3
	ands r3, r2
	movs r2, #16
	orrs r3, r2
	strb r3, [r4, #17]
	movs r2, #4
	adds r3, r5, #0
	orrs r3, r2
	movs r2, #254
	ands r3, r2
	strb r3, [r4, #26]
.L_0802782e:
	mov r1, r10
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #0
	bne .L_0802788c
	ldr r2, [sp, #36]
	cmp r2, #0
	bne .L_08027892
	ldr r3, [sp, #12]
	cmp r3, #0
	bne .L_080278ac
	ldr r4, [sp, #4]
	mov r0, r8
	ldrb r1, [r4, #23]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Func_08026fc8
	ldr r0, [sp, #16]
	cmp r0, #5
	bne .L_0802785e
	mov r1, r10
	strh r6, [r1]
	b .L_08027864
.L_0802785e:
	movs r3, #12
	mov r2, r10
	strh r3, [r2]
.L_08027864:
	mov r1, r8
	adds r1, #102
	ldrh r3, [r1]
	ldr r2, .L_08027898
	eors r3, r2
	strh r3, [r1]
	b .L_080278ac
.L_08027872:
	movs r3, #8
	ands r3, r5
	cmp r3, #0
	bne .L_080278b0
	movs r3, #1
	ldr r4, [sp, #28]
	adds r2, r5, #0
	orrs r2, r3
	movs r3, #251
	ands r2, r3
	strb r2, [r4, #26]
	ldrb r1, [r4, #26]
	b .L_080278b0
.L_0802788c:
	ldr r0, [sp, #28]
	ldrb r1, [r0, #26]
	b .L_080278b0
.L_08027892:
	ldr r2, [sp, #28]
	ldrb r1, [r2, #26]
	b .L_080278b0
.L_08027898:
	.4byte 0x00000001
.L_0802789c:
	.4byte 0xfffff000
.L_080278a0:
	.4byte IwramMulQ16
.L_080278a4:
	.4byte gMapCellBuffer
.L_080278a8:
	.4byte 0x000fffff
.L_080278ac:
	ldr r3, [sp, #28]
	ldrb r1, [r3, #26]
.L_080278b0:
	cmp r5, r1
	beq .L_080278c0
	ldr r4, [sp, #28]
	movs r3, #1
	strb r3, [r4, #25]
	ldr r0, [sp, #4]
	ldrb r2, [r0, #23]
	b .L_080278c4
.L_080278c0:
	ldr r1, [sp, #4]
	ldrb r2, [r1, #23]
.L_080278c4:
	ldr r3, [sp, #0]
	lsrs r7, r3, #16
	movs r3, #3
	ands r3, r2
	cmp r3, #0
	beq .L_08027990
	mov r0, r10
	movs r4, #0
	ldrsh r3, [r0, r4]
	cmp r3, #0
	bne .L_08027990
	ldr r1, [sp, #36]
	cmp r1, #0
	bne .L_08027990
	mov r2, r8
	mov r4, r8
	movs r0, #135
	ldr r1, [r2, #8]
	lsls r0, r0, #1
	ldr r2, [r2, #12]
	ldr r3, [r4, #16]
	bl Func_08023220
	adds r6, r0, #0
	cmp r6, #0
	beq .L_08027990
	mov r0, r8
	ldr r3, [r0, #20]
	ldr r1, .L_08027978
	str r3, [r6, #20]
	adds r0, r6, #0
	ldr r5, [r6, #80]
	bl ObjectDispatch_Initialize
	adds r2, r6, #0
	add r1, sp, #36
	movs r3, #2
	adds r2, #35
	ldrb r1, [r1]
	strb r3, [r2]
	adds r3, r6, #0
	adds r3, #85
	strb r1, [r3]
	cmp r5, #0
	beq .L_0802793c
	adds r0, r5, #0
	movs r1, #1
	bl Animation_ApplyChildArgument
	movs r4, #128
	add r2, sp, #36
	lsls r4, r4, #7
	ldrb r2, [r2]
	adds r3, r7, r4
	strh r3, [r5, #18]
	ldrb r3, [r5, #9]
	strb r2, [r5, #26]
	movs r2, #12
	orrs r3, r2
	strb r3, [r5, #9]
.L_0802793c:
	mov r7, r8
	adds r7, #102
	movs r0, #0
	ldrsh r3, [r7, r0]
	ldrh r2, [r7]
	cmp r3, #2
	bne .L_0802795a
	movs r1, #2
	adds r0, r5, #0
	bl Animation_ApplyChildArgument
	add r1, sp, #36
	ldrh r1, [r1]
	ldr r2, .L_08027974
	strh r1, [r7]
.L_0802795a:
	lsls r3, r2, #16
	cmp r3, #0
	beq .L_08027966
	movs r3, #128
	lsls r3, r3, #8
	strh r3, [r6, #6]
.L_08027966:
	ldr r2, [sp, #16]
	cmp r2, #5
	bne .L_0802797c
	movs r3, #12
	mov r4, r10
	strh r3, [r4]
	b .L_08027982
.L_08027974:
	.4byte 0x00000000
.L_08027978:
	.4byte Data_0802ec7c
.L_0802797c:
	movs r3, #18
	mov r0, r10
	strh r3, [r0]
.L_08027982:
	ldrh r3, [r7]
	ldr r2, .L_0802798c
	eors r3, r2
	strh r3, [r7]
	b .L_08027990
.L_0802798c:
	.4byte 0x00000001
.L_08027990:
	bl Func_08026e60
	mov r1, r8
	ldrh r3, [r1, #4]
	mov r2, r8
	adds r3, #1
	movs r0, #1
	strh r3, [r2, #4]
	add sp, #120
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
