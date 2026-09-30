.syntax unified
	.thumb
	.global Func_0815842c
	.thumb_func
Func_0815842c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #68
	str r0, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	ldr r3, [r3, #96]
	mov r11, r0
	movs r0, #0
	str r3, [sp, #16]
	bl Func_081435e0
	ldr r2, [sp, #20]
	movs r3, #65
	ldr r1, [r2, #4]
	adds r0, r2, #0
	lsls r1, r1, #4
	orrs r1, r3
	mov r3, sp
	adds r3, #56
	str r3, [sp, #12]
	ldr r2, [sp, #12]
	add r3, sp, #44
	bl Func_0815585c
	ldr r4, [sp, #20]
	mov r1, sp
	ldr r0, [r4, #4]
	adds r1, #24
	str r1, [sp, #8]
	bl Func_08144aac
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_081585a0
	add r1, r11
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_081585a4
	bl Func_080145a8
	ldr r3, [sp, #20]
	add r5, sp, #32
	movs r2, #36
	ldrsh r0, [r3, r2]
	adds r1, r5, #0
	bl Func_0815e21c
	ldr r0, .L_081585a8
	movs r4, #15
	movs r1, #127
	movs r7, #0
	mov r9, r4
	mov r8, r0
	mov r10, r1
.L_081584c4:
	bl Random16
	movs r6, #254
	lsls r6, r6, #7
	adds r6, #255
	movs r2, #128
	lsls r2, r2, #7
	ands r6, r0
	adds r6, r6, r2
	bl Random16
	movs r5, #128
	lsls r5, r5, #1
	adds r5, #255
	ands r5, r0
	bl Random16
	ldr r3, [sp, #32]
	mov r4, r9
	lsrs r2, r3, #31
	adds r3, r3, r2
	ands r0, r4
	asrs r3, r3, #1
	adds r3, r3, r0
	subs r3, #8
	mov r0, r8
	lsls r3, r3, #16
	str r3, [r0]
	ldr r3, [sp, #36]
	adds r5, #128
	adds r3, #8
	lsls r3, r3, #16
	str r3, [r0, #4]
	adds r0, r6, #0
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	mov r1, r8
	asrs r3, r3, #9
	str r3, [r1, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	mov r2, r8
	asrs r3, r3, #6
	str r3, [r2, #16]
	bl Random16
	mov r3, r10
	mov r4, r8
	ands r0, r3
	str r0, [r4, #8]
	bl Random16
	mov r1, r10
	mov r2, r8
	ands r0, r1
	str r0, [r2, #20]
	bl Random16
	mov r3, r9
	ands r0, r3
	adds r0, #32
	mov r4, r8
	str r0, [r4, #24]
	adds r7, #1
	movs r0, #28
	add r8, r0
	cmp r7, #64
	bne .L_081584c4
	movs r1, #0
	mov r10, r1
.L_0815855a:
	mov r2, r10
	cmp r2, #47
	ble .L_08158572
	ldr r2, .L_08158598
	ldr r1, .L_0815859c
	movs r3, #128
	mov r4, r10
	lsls r3, r3, #19
	subs r2, r2, r4
	adds r3, #82
	orrs r2, r1
	strh r2, [r3]
.L_08158572:
	mov r0, r10
	cmp r0, #1
	bne .L_081585b8
	movs r1, #176
	lsls r1, r1, #4
	ldr r0, .L_081585ac
	add r1, r11
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r1, #216
	lsls r1, r1, #7
	adds r1, #192
	ldr r0, .L_081585b0
	add r1, r11
	movs r2, #1
	movs r3, #0
	b .L_081585b4
.L_08158598:
	.4byte 0x00000040
.L_0815859c:
	.4byte 0x00001000
.L_081585a0:
	.4byte 0x0000012f
.L_081585a4:
	.4byte Func_08143000
.L_081585a8:
	.4byte gMapCellBuffer
.L_081585ac:
	.4byte 0x0000017c
.L_081585b0:
	.4byte 0x00000155
.L_081585b4:
	bl Func_08157cf4
.L_081585b8:
	ldr r1, [sp, #20]
	ldr r3, [r1, #28]
	cmp r3, #1
	bne .L_08158640
	mov r2, r10
	lsls r5, r2, #11
	adds r0, r5, #0
	bl Trig_Sin
	ldr r4, [sp, #12]
	negs r0, r0
	ldr r3, [r4]
	lsls r0, r0, #2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	asrs r0, r0, #16
	adds r0, r0, r3
	subs r0, #10
	mov r9, r0
	adds r0, r5, #0
	bl Trig_Cos
	ldr r1, [sp, #12]
	lsls r0, r0, #1
	ldr r3, [r1, #4]
	asrs r0, r0, #16
	adds r0, r0, r3
	adds r5, r0, #0
	mov r2, r10
	subs r5, #22
	cmp r2, #69
	ble .L_08158602
	lsls r3, r2, #1
	subs r3, r5, r3
	adds r5, r3, #0
	adds r5, #138
.L_08158602:
	movs r3, #20
	movs r6, #216
	movs r7, #40
	str r3, [sp, #0]
	ldr r0, [sp, #8]
	str r7, [sp, #4]
	lsls r6, r6, #7
	adds r6, #192
	add r6, r11
	ldr r4, [r0, #4]
	adds r1, r6, #0
	mov r8, r3
	ldr r0, [sp, #16]
	mov r2, r9
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
	mov r1, r10
	cmp r1, #3
	bgt .L_08158640
	mov r2, r8
	str r2, [sp, #0]
	ldr r3, [sp, #8]
	str r7, [sp, #4]
	ldr r0, [sp, #16]
	ldr r4, [r3, #4]
	adds r1, r6, #0
	mov r2, r9
	adds r3, r5, #0
	mov lr, r4
	.2byte 0xf800
.L_08158640:
	ldr r4, .L_081587d0
	movs r7, #0
	mov r8, r4
.L_08158646:
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0815864e
	adds r3, r7, #3
.L_0815864e:
	asrs r3, r3, #2
	adds r3, #4
	cmp r10, r3
	blt .L_081586b2
	mov r0, r8
	ldr r4, [r0, #8]
	cmp r4, #0
	bge .L_08158660
	adds r4, #127
.L_08158660:
	ldr r2, .L_081587d4
	movs r3, #3
	asrs r4, r4, #7
	ands r4, r3
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	movs r2, #176
	lsls r2, r2, #4
	mov r0, r8
	add r1, r11
	adds r1, r1, r2
	movs r3, #2
	ldrsh r2, [r0, r3]
	ldr r3, .L_081587d8
	movs r5, #1
	ldrb r6, [r3, r4]
	ands r5, r7
	lsrs r3, r6, #1
	subs r2, r2, r3
	movs r3, #6
	ldrsh r0, [r0, r3]
	lsls r5, r5, #2
	mov r12, r0
	ldr r0, .L_081587dc
	mov r3, r12
	ldrb r4, [r0, r4]
	str r6, [sp, #0]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #8]
	ldr r4, [r5, r0]
	ldr r0, [sp, #16]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	mov r0, r8
	movs r1, #63
	lsls r2, r2, #5
	bl BattleFxKernels_IntegrateVector3
.L_081586b2:
	movs r1, #28
	adds r7, #1
	add r8, r1
	cmp r7, #64
	bne .L_08158646
	mov r2, r10
	cmp r2, #8
	bne .L_081586f2
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	add r3, r11
	str r2, [r3]
	movs r0, #134
	bl Func_081180e8
	ldr r4, [sp, #20]
	movs r1, #7
	movs r3, #36
	ldrsh r0, [r4, r3]
	movs r3, #16
	str r3, [sp, #0]
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	ldr r2, [sp, #20]
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #3
	bl Func_08118088
.L_081586f2:
	mov r3, r10
	lsls r5, r3, #2
	cmp r5, #32
	ble .L_081586fc
	movs r5, #32
.L_081586fc:
	ldr r4, [sp, #20]
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_08158742
	movs r0, #224
	lsls r0, r0, #3
	add r0, r11
	movs r1, #120
	movs r7, #0
	mov r8, r0
	movs r6, #32
	mov r9, r1
.L_08158714:
	mov r2, r10
	lsls r1, r7, #5
	cmp r2, #0
	bge .L_0815871e
	adds r2, #3
.L_0815871e:
	movs r3, #31
	asrs r2, r2, #2
	ands r2, r3
	ldr r3, [sp, #24]
	mov r4, r9
	subs r2, r1, r2
	mov r12, r3
	str r6, [sp, #0]
	str r6, [sp, #4]
	ldr r0, [sp, #16]
	mov r1, r8
	subs r3, r4, r5
	adds r7, #1
	mov lr, r12
	.2byte 0xf800
	cmp r7, #5
	bne .L_08158714
	b .L_08158780
.L_08158742:
	movs r0, #224
	lsls r0, r0, #3
	add r0, r11
	movs r1, #120
	movs r7, #0
	mov r8, r0
	movs r6, #32
	mov r9, r1
.L_08158752:
	mov r2, r10
	lsls r1, r7, #5
	cmp r2, #0
	bge .L_0815875c
	adds r2, #3
.L_0815875c:
	movs r3, #31
	asrs r2, r2, #2
	ands r2, r3
	ldr r3, [sp, #24]
	adds r2, r1, r2
	mov r4, r9
	mov r12, r3
	subs r2, #32
	str r6, [sp, #0]
	str r6, [sp, #4]
	ldr r0, [sp, #16]
	mov r1, r8
	subs r3, r4, r5
	adds r7, #1
	mov lr, r12
	.2byte 0xf800
	cmp r7, #5
	bne .L_08158752
.L_08158780:
	movs r1, #8
	movs r0, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r11
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	movs r0, #1
	add r10, r0
	mov r1, r10
	cmp r1, #64
	beq .L_081587aa
	b .L_0815855a
.L_081587aa:
	ldr r0, .L_081587e0
	bl Func_08014644
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081587d0:
	.4byte gMapCellBuffer
.L_081587d4:
	.4byte Data_0819851c
.L_081587d8:
	.4byte Data_08198513
.L_081587dc:
	.4byte Data_08198517
.L_081587e0:
	.4byte Func_08143000
