.syntax unified
	.thumb
	.global Func_081a6154
	.thumb_func
Func_081a6154:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_081a6178
	movs r2, #128
	ldr r1, .L_081a617c
	lsls r2, r2, #19
	strh r3, [r2]
	sub sp, #36
	adds r6, r0, #0
	movs r3, #0
	movs r7, #0
.L_081a6174:
	movs r5, #31
	b .L_081a6180
.L_081a6178:
	.4byte 0x00000000
.L_081a617c:
	.4byte 0x0600f800
.L_081a6180:
	adds r2, r3, #0
	movs r0, #128
	lsls r3, r2, #16
	lsls r0, r0, #9
	adds r3, r3, r0
	subs r5, #1
	strh r2, [r1]
	asrs r3, r3, #16
	adds r1, #2
	cmp r5, #0
	bge .L_081a6180
	adds r7, #1
	cmp r7, #15
	ble .L_081a6174
	movs r3, #0
	cmp r7, #31
	bgt .L_081a61c0
.L_081a61a2:
	movs r5, #31
.L_081a61a4:
	adds r2, r3, #0
	movs r4, #128
	lsls r3, r2, #16
	lsls r4, r4, #9
	adds r3, r3, r4
	subs r5, #1
	strh r2, [r1]
	asrs r3, r3, #16
	adds r1, #2
	cmp r5, #0
	bge .L_081a61a4
	adds r7, #1
	cmp r7, #31
	ble .L_081a61a2
.L_081a61c0:
	ldr r1, .L_081a61d0
	ldr r0, .L_081a61cc
	movs r3, #0
	movs r7, #0
.L_081a61c8:
	movs r5, #31
	b .L_081a61d4
.L_081a61cc:
	.4byte 0x00001000
.L_081a61d0:
	.4byte 0x0600f000
.L_081a61d4:
	adds r2, r3, #0
	lsls r3, r2, #16
	movs r4, #128
	lsls r2, r2, #16
	lsls r4, r4, #9
	lsrs r2, r2, #16
	adds r3, r3, r4
	orrs r2, r0
	subs r5, #1
	strh r2, [r1]
	asrs r3, r3, #16
	adds r1, #2
	cmp r5, #0
	bge .L_081a61d4
	adds r7, #1
	cmp r7, #15
	ble .L_081a61c8
	movs r3, #0
	cmp r7, #31
	bgt .L_081a6222
	ldr r0, .L_081a622c
.L_081a61fe:
	movs r5, #31
.L_081a6200:
	adds r2, r3, #0
	lsls r3, r2, #16
	movs r4, #128
	lsls r2, r2, #16
	lsls r4, r4, #9
	lsrs r2, r2, #16
	adds r3, r3, r4
	orrs r2, r0
	subs r5, #1
	strh r2, [r1]
	asrs r3, r3, #16
	adds r1, #2
	cmp r5, #0
	bge .L_081a6200
	adds r7, #1
	cmp r7, #31
	ble .L_081a61fe
.L_081a6222:
	ldr r3, .L_081a6230
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #10
	b .L_081a6234
.L_081a622c:
	.4byte 0x00001000
.L_081a6230:
	.4byte 0x00001f02
.L_081a6234:
	strh r3, [r2]
	ldr r3, .L_081a6264
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_081a6268
	adds r2, #68
	strh r3, [r2]
	ldr r3, .L_081a626c
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_081a6270
	movs r2, #0
	movs r5, #3
.L_081a624e:
	subs r5, #1
	strh r2, [r3, #2]
	strh r2, [r3]
	adds r3, #4
	cmp r5, #0
	bge .L_081a624e
	cmp r6, #0
	bne .L_081a6274
	bl Func_081a814c
	b .L_081a6274
.L_081a6264:
	.4byte 0x00001e05
.L_081a6268:
	.4byte 0x00000244
.L_081a626c:
	.4byte 0x00000a0b
.L_081a6270:
	.4byte Data_03001120
.L_081a6274:
	movs r0, #2
	movs r1, #0
	bl Func_081a81f4
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_081a81d4
	ldr r3, .L_081a629c
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	cmp r6, #0
	bne .L_081a62a0
	movs r0, #1
	bl Func_081a8228
	b .L_081a62a6
	.2byte 0x0000
.L_081a629c:
	.4byte 0x00000600
.L_081a62a0:
	movs r0, #10
	bl Func_081a8228
.L_081a62a6:
	cmp r6, #0
	beq .L_081a62ac
	b .L_081a6424
.L_081a62ac:
	mov r5, sp
	movs r3, #30
	str r3, [r5]
	movs r7, #2
	subs r3, #33
	str r7, [r5, #4]
	str r6, [r5, #8]
	str r3, [r5, #12]
	str r6, [r5, #16]
	str r6, [r5, #20]
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r6, [r5, #32]
	adds r0, r5, #0
	bl Func_081a60c0
	cmp r0, #0
	bne .L_081a62d2
	b .L_081a6562
.L_081a62d2:
	movs r3, #2
	movs r0, #10
	negs r3, r3
	mov r10, r0
	str r0, [r5]
	movs r2, #0
	str r7, [r5, #4]
	str r6, [r5, #8]
	str r3, [r5, #12]
	str r6, [r5, #16]
	str r6, [r5, #20]
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r6, [r5, #32]
	adds r0, r5, #0
	mov r11, r2
	mov r8, r3
	bl Func_081a60c0
	cmp r0, #0
	bne .L_081a62fe
	b .L_081a6562
.L_081a62fe:
	mov r0, r8
	mov r4, r10
	movs r7, #1
	str r0, [r5, #12]
	str r4, [r5]
	str r7, [r5, #4]
	str r6, [r5, #8]
	str r6, [r5, #16]
	str r6, [r5, #20]
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r6, [r5, #32]
	adds r0, r5, #0
	bl Func_081a60c0
	cmp r0, #0
	bne .L_081a6322
	b .L_081a6562
.L_081a6322:
	movs r3, #1
	negs r3, r3
	mov r2, r10
	str r2, [r5]
	str r7, [r5, #4]
	str r6, [r5, #8]
	str r3, [r5, #12]
	str r6, [r5, #16]
	str r6, [r5, #20]
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r6, [r5, #32]
	adds r0, r5, #0
	mov r8, r3
	bl Func_081a60c0
	cmp r0, #0
	bne .L_081a6348
	b .L_081a6562
.L_081a6348:
	mov r0, r8
	mov r4, r10
	str r0, [r5, #12]
	str r4, [r5]
	str r7, [r5, #4]
	str r6, [r5, #8]
	str r6, [r5, #16]
	str r7, [r5, #20]
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r6, [r5, #32]
	adds r0, r5, #0
	bl Func_081a60c0
	cmp r0, #0
	bne .L_081a636a
	b .L_081a6562
.L_081a636a:
	mov r2, r10
	mov r3, r8
	str r2, [r5]
	str r6, [r5, #4]
	str r6, [r5, #8]
	str r3, [r5, #12]
	str r6, [r5, #16]
	str r6, [r5, #20]
	str r6, [r5, #24]
	str r7, [r5, #28]
	str r6, [r5, #32]
	adds r0, r5, #0
	bl Func_081a60c0
	cmp r0, #0
	bne .L_081a638c
	b .L_081a6562
.L_081a638c:
	movs r4, #20
	mov r0, r8
	str r0, [r5, #4]
	str r4, [r5]
	str r6, [r5, #8]
	str r6, [r5, #12]
	str r6, [r5, #16]
	str r7, [r5, #20]
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r6, [r5, #32]
	adds r0, r5, #0
	mov r10, r4
	bl Func_081a60c0
	cmp r0, #0
	bne .L_081a63b0
	b .L_081a6562
.L_081a63b0:
	movs r4, #3
	mov r2, r10
	mov r3, r8
	str r2, [r5]
	str r3, [r5, #4]
	str r6, [r5, #8]
	str r7, [r5, #12]
	str r6, [r5, #16]
	str r4, [r5, #20]
	str r6, [r5, #24]
	str r7, [r5, #28]
	str r6, [r5, #32]
	adds r0, r5, #0
	mov r9, r4
	bl Func_081a60c0
	cmp r0, #0
	bne .L_081a63d6
	b .L_081a6562
.L_081a63d6:
	mov r0, r10
	mov r2, r8
	mov r3, r9
	str r0, [r5]
	str r2, [r5, #4]
	str r6, [r5, #8]
	str r7, [r5, #12]
	str r6, [r5, #16]
	str r3, [r5, #20]
	str r6, [r5, #24]
	str r7, [r5, #28]
	str r6, [r5, #32]
	adds r0, r5, #0
	bl Func_081a60c0
	cmp r0, #0
	bne .L_081a63fa
	b .L_081a6562
.L_081a63fa:
	movs r0, #2
	movs r1, #0
	bl Func_081a81d4
	movs r0, #60
	bl Func_081a8228
	mov r0, r11
	movs r3, #60
	mov r4, r8
	mov r2, r9
	str r0, [r5, #8]
	str r0, [r5, #16]
	str r0, [r5, #24]
	str r0, [r5, #32]
	str r3, [r5]
	str r4, [r5, #4]
	str r7, [r5, #12]
	str r2, [r5, #20]
	str r7, [r5, #28]
	b .L_081a6558
.L_081a6424:
	cmp r6, #1
	beq .L_081a642a
	b .L_081a6566
.L_081a642a:
	mov r5, sp
	movs r3, #40
	movs r7, #0
	str r3, [r5]
	subs r3, #41
	str r6, [r5, #4]
	str r6, [r5, #8]
	str r3, [r5, #12]
	str r3, [r5, #16]
	str r6, [r5, #20]
	str r7, [r5, #24]
	str r6, [r5, #28]
	str r7, [r5, #32]
	adds r0, r5, #0
	mov r8, r3
	bl Func_081a60c0
	cmp r0, #0
	bne .L_081a6452
	b .L_081a6562
.L_081a6452:
	mov r0, r8
	movs r4, #20
	movs r2, #3
	str r0, [r5, #12]
	str r0, [r5, #16]
	str r4, [r5]
	str r6, [r5, #4]
	str r6, [r5, #8]
	str r2, [r5, #20]
	str r7, [r5, #24]
	str r6, [r5, #28]
	str r7, [r5, #32]
	adds r0, r5, #0
	mov r11, r4
	mov r10, r2
	bl Func_081a60c0
	cmp r0, #0
	beq .L_081a6562
	movs r3, #10
	mov r4, r8
	mov r0, r10
	mov r9, r3
	str r3, [r5]
	movs r3, #7
	str r0, [r5, #24]
	str r6, [r5, #4]
	str r6, [r5, #8]
	str r4, [r5, #12]
	str r4, [r5, #16]
	str r3, [r5, #20]
	str r6, [r5, #28]
	str r7, [r5, #32]
	adds r0, r5, #0
	bl Func_081a60c0
	cmp r0, #0
	beq .L_081a6562
	mov r2, r9
	mov r3, r8
	mov r4, r10
	str r2, [r5]
	str r7, [r5, #4]
	str r6, [r5, #8]
	str r7, [r5, #12]
	str r3, [r5, #16]
	str r7, [r5, #20]
	str r4, [r5, #24]
	str r7, [r5, #28]
	str r7, [r5, #32]
	adds r0, r5, #0
	bl Func_081a60c0
	cmp r0, #0
	beq .L_081a6562
	mov r0, r8
	movs r2, #15
	movs r3, #30
	str r0, [r5, #4]
	str r0, [r5, #16]
	str r3, [r5]
	str r7, [r5, #8]
	str r7, [r5, #12]
	str r2, [r5, #20]
	str r6, [r5, #24]
	str r7, [r5, #28]
	str r7, [r5, #32]
	adds r0, r5, #0
	mov r10, r2
	bl Func_081a60c0
	cmp r0, #0
	beq .L_081a6562
	mov r4, r8
	mov r0, r10
	mov r3, r11
	str r0, [r5, #20]
	str r3, [r5]
	str r4, [r5, #4]
	str r4, [r5, #8]
	str r7, [r5, #12]
	str r4, [r5, #16]
	str r6, [r5, #24]
	str r7, [r5, #28]
	str r7, [r5, #32]
	adds r0, r5, #0
	bl Func_081a60c0
	cmp r0, #0
	beq .L_081a6562
	movs r4, #2
	movs r2, #60
	mov r3, r8
	negs r4, r4
	mov r0, r10
	str r0, [r5, #20]
	str r2, [r5]
	str r3, [r5, #4]
	str r3, [r5, #8]
	str r7, [r5, #12]
	str r4, [r5, #16]
	str r6, [r5, #24]
	str r7, [r5, #28]
	str r7, [r5, #32]
	adds r0, r5, #0
	mov r11, r2
	mov r9, r4
	bl Func_081a60c0
	cmp r0, #0
	beq .L_081a6562
	movs r0, #2
	movs r1, #0
	bl Func_081a81d4
	movs r0, #60
	bl Func_081a8228
	mov r3, r8
	mov r0, r10
	mov r2, r11
	mov r4, r9
	str r0, [r5, #20]
	str r2, [r5]
	str r3, [r5, #4]
	str r3, [r5, #8]
	str r7, [r5, #12]
	str r4, [r5, #16]
	str r6, [r5, #24]
	str r7, [r5, #28]
	str r7, [r5, #32]
.L_081a6558:
	adds r0, r5, #0
	bl Func_081a60c0
	cmp r0, #0
	bne .L_081a6624
.L_081a6562:
	movs r0, #0
	b .L_081a6626
.L_081a6566:
	mov r5, sp
	movs r3, #60
	str r3, [r5]
	subs r3, #61
	movs r6, #0
	str r3, [r5, #8]
	str r3, [r5, #12]
	movs r3, #1
	str r6, [r5, #4]
	str r3, [r5, #16]
	str r6, [r5, #20]
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r6, [r5, #32]
	adds r0, r5, #0
	bl Func_081a60c0
	cmp r0, #0
	beq .L_081a6562
	movs r3, #16
	movs r2, #8
	str r3, [r5]
	subs r3, #24
	str r2, [r5, #4]
	str r3, [r5, #8]
	str r2, [r5, #12]
	str r2, [r5, #16]
	str r6, [r5, #20]
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r6, [r5, #32]
	adds r0, r5, #0
	bl Func_081a60c0
	cmp r0, #0
	beq .L_081a6562
	movs r2, #128
	lsls r2, r2, #19
	ldr r6, .L_081a6634
	adds r2, #82
	movs r7, #2
	movs r5, #11
	mov r8, r2
.L_081a65bc:
	lsls r3, r5, #8
	orrs r3, r5
	mov r4, r8
	strh r3, [r4]
	movs r0, #1
	ldrh r3, [r6, #6]
	adds r3, r3, r7
	adds r3, #16
	strh r3, [r6, #6]
	ldrh r3, [r6, #10]
	adds r3, r3, r7
	adds r3, #8
	strh r3, [r6, #10]
	adds r7, #1
	bl Func_081a6094
	cmp r0, #0
	bne .L_081a6562
	ldrh r3, [r6, #6]
	movs r0, #1
	adds r3, r3, r7
	adds r3, #16
	strh r3, [r6, #6]
	ldrh r3, [r6, #10]
	adds r3, r3, r7
	adds r3, #8
	strh r3, [r6, #10]
	adds r7, #1
	bl Func_081a6094
	cmp r0, #0
	bne .L_081a6562
	adds r5, #1
	cmp r5, #15
	ble .L_081a65bc
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_081a81d4
	movs r0, #1
	bl Func_081a8228
	movs r0, #2
	bl Func_081a6094
	cmp r0, #0
	beq .L_081a6624
	bl Func_08013fdc
	b .L_081a6562
.L_081a6624:
	movs r0, #1
.L_081a6626:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081a6634:
	.4byte Data_03001120
