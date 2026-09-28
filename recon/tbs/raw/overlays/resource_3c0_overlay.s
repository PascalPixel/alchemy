.syntax unified
	.thumb
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [r0, #80]
	movs r3, #3
	ldrb r2, [r0, #9]
	ands r1, r3
	movs r3, #13
	negs r3, r3
	lsls r1, r1, #2
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #9]
	bx lr
	.2byte 0x0000
	.global Func_02000048
	.thumb_func
Func_02000048:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl 0x02009224
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000048_0
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	adds r2, r5, #0
	strb r3, [r1, #9]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	movs r1, #0
	bl 0x0200923c
	adds r0, r5, #0
	movs r1, #14
	bl 0x020092f4
	adds r0, r5, #0
	movs r1, #1
	bl 0x02009244
	adds r0, r5, #0
	b .L_02000048_1
.L_02000048_0:
	movs r0, #0
.L_02000048_1:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_020000a0
	.thumb_func
Func_020000a0:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl 0x02009224
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020000a0_0
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r2, r5, #0
	strb r3, [r1, #9]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	movs r1, #0
	bl 0x0200923c
	adds r0, r5, #0
	movs r1, #15
	bl 0x020092f4
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	b .L_020000a0_1
.L_020000a0_0:
	movs r0, #0
.L_020000a0_1:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000104
	.thumb_func
Func_02000104:
	ldr r3, [r0, #8]
	ldr r2, [r0, #68]
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
	ldr r1, [r0, #80]
	adds r0, #100
	ldrh r3, [r1, #30]
	ldrh r2, [r0]
	adds r3, r3, r2
	strh r3, [r1, #30]
	bx lr
	.2byte 0x0000
	.global Func_0200013c
	.thumb_func
Func_0200013c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r6, r1, #0
	ldr r1, [sp, #48]
	adds r5, r0, #0
	movs r0, #0
	mov r8, r2
	str r3, [sp, #4]
	mov r10, r1
	ldr r7, [sp, #52]
	bl 0x0200928c
	movs r3, #128
	lsls r3, r3, #13
	mov r2, r10
	ands r3, r2
	mov r9, r0
	cmp r3, #0
	beq .L_0200013c_0
	cmp r7, #0
	beq .L_0200013c_0
	movs r3, #24
	ldrsh r0, [r7, r3]
	adds r2, r6, #0
	b .L_0200013c_1
.L_0200013c_0:
	adds r2, r6, #0
	movs r0, #222
.L_0200013c_1:
	adds r1, r5, #0
	mov r3, r8
	bl 0x02009224
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200013c_2
	b .L_0200013c_3
.L_0200013c_2:
	ldr r1, [r6, #80]
	mov r8, r1
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	bl 0x02009214
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x0200921c
	adds r3, r6, #0
	movs r0, #0
	adds r3, #85
	strb r0, [r3]
	mov r3, r8
	adds r3, #38
	strb r0, [r3]
	ldr r3, [pc, #328]
	str r3, [r6, #108]
	ldr r3, [sp, #4]
	str r3, [r6, #68]
	ldr r3, [sp, #40]
	str r3, [r6, #72]
	ldr r3, [sp, #44]
	mov r1, r9
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	mov r3, r8
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	mov r9, r3
	ands r3, r1
	orrs r3, r2
	adds r2, r6, #0
	mov r1, r8
	adds r2, #100
	strb r3, [r1, #9]
	adds r3, r2, #0
	str r0, [r6, #48]
	str r0, [r6, #52]
	str r2, [sp, #0]
	strh r0, [r3]
	ldr r3, [pc, #276]
	mov r1, r10
	ands r3, r1
	movs r5, #3
	cmp r3, #0
	beq .L_0200013c_3
	cmp r7, #0
	beq .L_0200013c_3
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_4
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl 0x020092f4
.L_0200013c_4:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_5
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	mov r3, r8
	ldrb r2, [r7]
	ldrb r1, [r3, #9]
	ands r2, r5
	mov r3, r9
	ands r3, r1
	lsls r2, r2, #2
	orrs r3, r2
	mov r1, r8
	strb r3, [r1, #9]
.L_0200013c_5:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_0200013c_6
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_0200013c_6:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_7
	ldr r3, [pc, #156]
	mov r1, r11
	ldr r5, [r3, r1]
	cmp r2, #0
	beq .L_0200013c_8
	ldr r0, [r7, #16]
	ldr r3, [r6, #24]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	bl 0x020091ec
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200013c_9
.L_0200013c_8:
	ldr r0, [r7, #16]
	ldr r2, [pc, #128]
	ldr r1, [r5, #12]
	adds r0, r0, r2
	bl 0x020091ec
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200013c_9:
	bl 0x020091ec
	str r0, [r6, #52]
.L_0200013c_7:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_10
	adds r0, r6, #0
	movs r1, #1
	bl 0x02009214
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x0200921c
.L_0200013c_10:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_11
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #30]
.L_0200013c_11:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_12
	ldrh r3, [r7, #34]
	ldr r1, [sp, #0]
	strh r3, [r1]
.L_0200013c_12:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_3
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200013c_3:
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02009424
	.4byte 0x02008105
	.4byte 0xffff0000
	.global Func_02000314
	.thumb_func
Func_02000314:
	push {lr}
	movs r0, #15
	movs r1, #45
	bl 0x02009344
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000324
	.thumb_func
Func_02000324:
	push {r5, lr}
	movs r1, #0
	adds r5, r0, #0
	bl 0x0200923c
	adds r5, #89
	movs r3, #0
	strb r3, [r5]
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.global Func_0200033c
	.thumb_func
Func_0200033c:
	adds r0, #84
	ldrb r3, [r0]
	movs r2, #1
	eors r3, r2
	strb r3, [r0]
	movs r0, #1
	bx lr
	.2byte 0x0000
	.global Func_0200034c
	.thumb_func
Func_0200034c:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_0200034c_0
	ldr r0, [pc, #36]
	b .L_0200034c_1
.L_0200034c_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_0200034c_2
	ldr r0, [pc, #36]
	b .L_0200034c_1
.L_0200034c_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_0200034c_3
	ldr r0, [pc, #32]
	b .L_0200034c_1
.L_0200034c_3:
	ldr r0, [pc, #32]
.L_0200034c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000a4
	.4byte 0x02009488
	.4byte 0x000000a5
	.4byte 0x020094d0
	.4byte 0x000000a6
	.4byte 0x02009548
	.4byte 0x02009458
	.global Func_020003a0
	.thumb_func
Func_020003a0:
	movs r0, #0
	bx lr
	.global Func_020003a4
	.thumb_func
Func_020003a4:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020095c0
	.global Func_020003ac
	.thumb_func
Func_020003ac:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_020003ac_0
	ldr r0, [pc, #36]
	b .L_020003ac_1
.L_020003ac_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020003ac_2
	ldr r0, [pc, #36]
	b .L_020003ac_1
.L_020003ac_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020003ac_3
	ldr r0, [pc, #32]
	b .L_020003ac_1
.L_020003ac_3:
	ldr r0, [pc, #32]
.L_020003ac_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000a4
	.4byte 0x02009610
	.4byte 0x000000a5
	.4byte 0x020096b8
	.4byte 0x000000a6
	.4byte 0x02009790
	.4byte 0x020095f8
	.global Func_02000400
	.thumb_func
Func_02000400:
	push {r5, r6, lr}
	ldr r3, [pc, #84]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r3]
	bl 0x0200928c
	ldr r3, [pc, #72]
	ldr r6, [r3]
	ldr r3, [pc, #72]
	ldr r3, [r3]
	lsls r3, r3, #12
	strh r3, [r0, #6]
	movs r0, #132
	lsls r0, r0, #2
	bl 0x02009264
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000400_0
	cmp r5, #1
	bne .L_02000400_1
	movs r3, #193
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #99
	strh r3, [r2]
	b .L_02000400_0
.L_02000400_1:
	movs r0, #131
	lsls r0, r0, #1
	bl 0x0200924c
	cmp r0, #0
	bne .L_02000400_0
	subs r5, #1
.L_02000400_0:
	movs r0, #132
	lsls r0, r0, #2
	adds r1, r5, #0
	bl 0x0200926c
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x03001ebc
	.4byte 0x03001e40
	.global Func_02000464
	.thumb_func
Func_02000464:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r2, [pc, #212]
	movs r3, #250
	mov r9, r2
	lsls r3, r3, #1
	add r3, r9
	ldr r6, [r3]
	mov r10, r0
	adds r0, r6, #0
	bl 0x0200928c
	adds r5, r0, #0
	mov r0, r10
	bl 0x0200928c
	ldr r0, [pc, #188]
	bl 0x0200924c
	mov r8, r0
	cmp r0, #0
	bne .L_02000464_0
	bl 0x0200927c
	adds r0, r6, #0
	ldr r1, [pc, #172]
	bl 0x02009324
	adds r0, r6, #0
	movs r1, #9
	bl 0x020092d4
	mov r0, r10
	bl 0x0200928c
	cmp r0, #0
	beq .L_02000464_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	adds r0, r6, #0
	bl 0x020092ac
.L_02000464_1:
	adds r0, r6, #0
	bl 0x020092c4
	movs r0, #244
	bl 0x02009374
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #124]
	adds r7, r5, #0
	bl 0x020091fc
	adds r7, #85
	mov r2, r8
	strb r2, [r7]
	movs r3, #128
	ldr r2, [r5, #12]
	lsls r3, r3, #14
	ldr r1, [r5, #8]
	adds r2, r2, r3
	adds r0, r5, #0
	ldr r3, [r5, #16]
	bl 0x0200922c
	adds r0, r6, #0
	bl 0x020092c4
	mov r2, r8
	str r2, [r5, #40]
	movs r2, #249
	movs r3, #4
	lsls r2, r2, #1
	add r2, r9
	strb r3, [r7]
	movs r3, #2
	strb r3, [r2]
	ldr r0, [pc, #60]
	bl 0x02009254
	movs r0, #134
	lsls r0, r0, #2
	mov r1, r10
	bl 0x0200926c
	movs r0, #132
	lsls r0, r0, #2
	movs r1, #180
	bl 0x0200926c
	bl 0x02009284
	ldr r3, [pc, #40]
	movs r2, #190
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r2, r8
	strh r2, [r3]
.L_02000464_0:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x0000020f
	.4byte 0x00000101
	.4byte 0x02008401
	.4byte 0x03001ebc
	.global Func_02000558
	.thumb_func
Func_02000558:
	push {lr}
	movs r0, #8
	bl 0x02008464
	pop {r0}
	bx r0
	.global Func_02000564
	.thumb_func
Func_02000564:
	push {lr}
	movs r0, #9
	bl 0x02008464
	pop {r0}
	bx r0
	.global Func_02000570
	.thumb_func
Func_02000570:
	push {lr}
	movs r0, #10
	bl 0x02008464
	pop {r0}
	bx r0
	.global Func_0200057c
	.thumb_func
Func_0200057c:
	push {lr}
	movs r0, #11
	bl 0x02008464
	pop {r0}
	bx r0
	.global Func_02000588
	.thumb_func
Func_02000588:
	push {lr}
	movs r0, #12
	bl 0x02008464
	pop {r0}
	bx r0
	.global Func_02000594
	.thumb_func
Func_02000594:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #572]
	movs r1, #250
	lsls r1, r1, #1
	adds r3, r3, r1
	mov r10, r0
	ldr r0, [r3]
	bl 0x0200928c
	adds r5, r0, #0
	mov r0, r10
	bl 0x0200928c
	adds r7, r0, #0
	movs r0, #208
	lsls r0, r0, #2
	bl 0x0200924c
	movs r3, #10
	ldrsh r2, [r5, r3]
	mov r9, r2
	movs r2, #18
	ldrsh r1, [r5, r2]
	mov r8, r0
	mov r11, r1
	ldr r6, [pc, #528]
	bl 0x0200927c
	movs r0, #244
	bl 0x02009374
	movs r2, #0
.L_02000594_3:
	movs r1, #128
	lsls r1, r1, #4
	lsls r3, r2, #11
	adds r3, r3, r1
	movs r1, #128
	str r3, [r7, #24]
	lsls r1, r1, #5
	lsls r3, r2, #12
	adds r3, r3, r1
	str r3, [r7, #28]
	mov r3, r8
	cmp r3, #0
	bne .L_02000594_0
	ldr r1, [pc, #492]
	adds r5, r2, #1
	ldrh r3, [r6]
	adds r0, r3, #0
	strh r6, [r6]
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_02000594_1
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r1]
	movs r3, #16
	lsls r2, r2, #2
	subs r3, r3, r5
	adds r2, r2, r1
	lsls r3, r3, #8
	adds r2, #4
	orrs r3, r5
	stmia r2!, {r3}
	ldr r3, [pc, #456]
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2]
.L_02000594_1:
	strh r0, [r6]
	b .L_02000594_2
.L_02000594_0:
	adds r5, r2, #1
.L_02000594_2:
	movs r0, #1
	bl 0x020091f4
	adds r2, r5, #0
	cmp r2, #15
	ble .L_02000594_3
	movs r0, #255
	lsls r0, r0, #1
	add r0, r10
	bl 0x02009254
	movs r0, #208
	lsls r0, r0, #2
	bl 0x02009254
	movs r0, #154
	lsls r0, r0, #4
	bl 0x0200924c
	cmp r0, #0
	bne .L_02000594_4
	b .L_02000594_5
.L_02000594_4:
	ldr r0, [pc, #400]
	bl 0x0200924c
	cmp r0, #0
	beq .L_02000594_6
	b .L_02000594_5
.L_02000594_6:
	ldr r0, [pc, #388]
	bl 0x02009254
	movs r1, #128
	movs r2, #128
	movs r0, #13
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009294
	movs r1, #192
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009294
	movs r0, #0
	bl 0x0200928c
	cmp r0, #0
	beq .L_02000594_7
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #13
	bl 0x020092cc
.L_02000594_7:
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #13
	bl 0x020092ec
	movs r0, #13
	bl 0x0200928c
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	mov r2, r11
	strb r3, [r0]
	subs r2, #16
	mov r1, r9
	movs r0, #13
	bl 0x020092b4
	movs r0, #0
	bl 0x0200928c
	adds r0, #90
	ldrb r3, [r0]
	mov r1, r9
	ands r5, r3
	mov r2, r11
	adds r1, #8
	subs r2, #40
	strb r5, [r0]
	movs r0, #0
	bl 0x020092bc
	movs r0, #1
	bl 0x02009274
	movs r0, #0
	bl 0x0200928c
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	movs r0, #13
	bl 0x020092d4
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #0
	bl 0x02009314
	ldr r0, [pc, #224]
	bl 0x020092fc
	movs r1, #131
	movs r2, #60
	movs r0, #13
	lsls r1, r1, #1
	bl 0x0200931c
	movs r0, #13
	movs r1, #0
	bl 0x02009304
	movs r1, #129
	movs r2, #60
	movs r0, #13
	lsls r1, r1, #1
	bl 0x0200931c
	movs r0, #13
	movs r1, #0
	bl 0x02009304
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02009314
	movs r2, #60
	movs r0, #13
	ldr r1, [pc, #168]
	bl 0x0200931c
	movs r1, #0
	movs r0, #13
	bl 0x02009304
	movs r0, #10
	bl 0x02009274
	movs r1, #192
	movs r2, #30
	movs r0, #13
	lsls r1, r1, #8
	bl 0x02009314
	movs r0, #13
	movs r1, #2
	bl 0x020092e4
	movs r0, #13
	movs r1, #0
	bl 0x02009304
	movs r0, #13
	movs r1, #3
	bl 0x020092dc
	movs r0, #13
	movs r1, #0
	bl 0x02009304
	movs r1, #128
	movs r2, #128
	movs r0, #13
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009294
	movs r0, #13
	movs r1, #2
	bl 0x020092d4
	movs r0, #0
	bl 0x0200928c
	cmp r0, #0
	beq .L_02000594_8
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #13
	bl 0x020092ac
.L_02000594_8:
	movs r0, #13
	bl 0x020092c4
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x020092cc
.L_02000594_5:
	bl 0x02009284
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x04000208
	.4byte 0x02002090
	.4byte 0x04000052
	.4byte 0x000009b6
	.4byte 0x0000262e
	.4byte 0x00000101
	.global Func_020007fc
	.thumb_func
Func_020007fc:
	push {lr}
	movs r0, #8
	bl 0x02008594
	pop {r0}
	bx r0
	.global Func_02000808
	.thumb_func
Func_02000808:
	push {lr}
	movs r0, #9
	bl 0x02008594
	pop {r0}
	bx r0
	.global Func_02000814
	.thumb_func
Func_02000814:
	push {lr}
	movs r0, #10
	bl 0x02008594
	pop {r0}
	bx r0
	.global Func_02000820
	.thumb_func
Func_02000820:
	push {lr}
	movs r0, #11
	bl 0x02008594
	pop {r0}
	bx r0
	.global Func_0200082c
	.thumb_func
Func_0200082c:
	push {lr}
	movs r0, #12
	bl 0x02008594
	pop {r0}
	bx r0
	.global Func_02000838
	.thumb_func
Func_02000838:
	push {lr}
	movs r0, #154
	lsls r0, r0, #4
	bl 0x0200924c
	cmp r0, #0
	bne .L_02000838_0
	b .L_02000838_1
.L_02000838_0:
	ldr r0, [pc, #360]
	bl 0x0200924c
	cmp r0, #0
	beq .L_02000838_2
	b .L_02000838_1
.L_02000838_2:
	movs r0, #155
	lsls r0, r0, #4
	bl 0x0200924c
	cmp r0, #0
	bne .L_02000838_3
	b .L_02000838_1
.L_02000838_3:
	ldr r0, [pc, #340]
	bl 0x02009254
	bl 0x0200927c
	ldr r0, [pc, #332]
	bl 0x020092fc
	movs r0, #0
	bl 0x0200928c
	cmp r0, #0
	beq .L_02000838_4
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #13
	bl 0x020092cc
.L_02000838_4:
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020092ec
	movs r1, #220
	movs r2, #157
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #3
	bl 0x020092bc
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009314
	movs r1, #222
	movs r2, #155
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #3
	bl 0x020092bc
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200931c
	movs r1, #128
	movs r2, #30
	movs r0, #0
	lsls r1, r1, #7
	bl 0x02009314
	movs r0, #13
	movs r1, #4
	bl 0x020092dc
	movs r0, #13
	movs r1, #0
	bl 0x02009304
	movs r0, #0
	ldr r1, [pc, #216]
	movs r2, #60
	bl 0x0200931c
	movs r2, #60
	movs r0, #13
	ldr r1, [pc, #204]
	bl 0x0200931c
	movs r1, #0
	movs r0, #13
	bl 0x02009304
	movs r0, #30
	bl 0x02009274
	movs r0, #13
	movs r1, #2
	bl 0x020092e4
	movs r0, #13
	movs r1, #0
	bl 0x02009304
	movs r1, #192
	movs r2, #30
	movs r0, #13
	lsls r1, r1, #8
	bl 0x02009314
	movs r1, #0
	movs r0, #13
	bl 0x0200930c
	movs r0, #30
	bl 0x02009274
	movs r1, #131
	movs r2, #60
	movs r0, #13
	lsls r1, r1, #1
	bl 0x0200931c
	movs r0, #13
	movs r1, #0
	bl 0x02009304
	movs r0, #13
	movs r1, #3
	bl 0x020092dc
	movs r0, #13
	movs r1, #0
	bl 0x02009304
	movs r0, #13
	ldr r1, [pc, #108]
	ldr r2, [pc, #112]
	bl 0x02009294
	movs r1, #220
	movs r2, #157
	lsls r2, r2, #3
	movs r0, #13
	lsls r1, r1, #1
	bl 0x020092bc
	movs r0, #13
	movs r1, #0
	bl 0x02009304
	movs r0, #0
	movs r1, #3
	bl 0x020092dc
	movs r0, #13
	movs r1, #2
	bl 0x020092d4
	movs r0, #0
	bl 0x0200928c
	cmp r0, #0
	beq .L_02000838_5
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #13
	bl 0x020092ac
.L_02000838_5:
	movs r0, #13
	bl 0x020092c4
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x020092cc
	bl 0x02009284
.L_02000838_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000001b7
	.4byte 0x000009b5
	.4byte 0x00002633
	.4byte 0x00000105
	.4byte 0x0000b333
	.4byte 0x00005999
	.global Func_020009cc
	.thumb_func
Func_020009cc:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #248]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #56
	bl 0x0200928c
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r6, #52]
	movs r3, #192
	lsls r3, r3, #9
	adds r7, r6, #0
	str r3, [r6, #48]
	adds r7, #85
	movs r3, #0
	strb r3, [r7]
	movs r1, #0
	adds r5, r0, #0
	adds r0, r6, #0
	bl 0x0200923c
	adds r1, r6, #0
	adds r1, #84
	ldrb r3, [r1]
	movs r2, #1
	eors r3, r2
	movs r0, #130
	strb r3, [r1]
	lsls r0, r0, #1
	bl 0x0200924c
	cmp r0, #0
	beq .L_020009cc_0
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r6, #56]
	str r3, [r6, #60]
	b .L_020009cc_1
.L_020009cc_0:
	ldr r3, [r5, #8]
	str r3, [r6, #56]
	ldr r3, [r5, #20]
	str r3, [r6, #60]
	ldr r3, [r5, #16]
	str r3, [r6, #64]
	ldr r1, [r6, #8]
	ldr r3, [r5, #8]
	subs r2, r1, r3
	cmp r2, #0
	bge .L_020009cc_2
	subs r2, r3, r1
.L_020009cc_2:
	ldr r0, [r6, #16]
	ldr r1, [r5, #16]
	subs r3, r0, r1
	cmp r3, #0
	blt .L_020009cc_3
	adds r3, r2, r3
	movs r2, #128
	lsls r2, r2, #12
	cmp r3, r2
	blt .L_020009cc_4
	b .L_020009cc_5
.L_020009cc_3:
	subs r3, r1, r0
	adds r3, r2, r3
	movs r2, #128
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_020009cc_5
.L_020009cc_4:
	ldr r3, [pc, #112]
	ldr r2, [r3]
	adds r3, r5, #0
	adds r3, #85
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_020009cc_6
	movs r3, #193
	lsls r3, r3, #1
	adds r2, r2, r3
	movs r3, #55
	strh r3, [r2]
.L_020009cc_6:
	movs r3, #3
	strb r3, [r7]
	ldr r3, [r5, #8]
	str r3, [r6, #56]
	ldr r3, [r5, #12]
	str r3, [r6, #60]
	ldr r3, [r5, #16]
.L_020009cc_1:
	str r3, [r6, #64]
.L_020009cc_5:
	ldr r3, [pc, #76]
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	cmp r7, #0
	bne .L_020009cc_7
	ldr r3, [pc, #68]
	add r5, sp, #16
	str r3, [r5, #8]
	str r3, [r5, #12]
	bl 0x0200920c
	movs r2, #248
	lsls r0, r0, #12
	lsls r2, r2, #8
	lsrs r0, r0, #16
	adds r0, r0, r2
	strh r0, [r5, #34]
	ldr r3, [pc, #48]
	ldr r0, [r6, #8]
	str r3, [sp, #8]
	ldr r1, [r6, #12]
	ldr r2, [r6, #16]
	movs r3, #0
	str r7, [sp, #0]
	str r7, [sp, #4]
	str r5, [sp, #12]
	bl 0x0200813c
.L_020009cc_7:
	movs r0, #1
	sub sp, #-56
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x03001ebc
	.4byte 0x03001e40
	.4byte 0x0000cccc
	.4byte 0x00880001
	.global Func_02000adc
	.thumb_func
Func_02000adc:
	push {lr}
	ldr r0, [pc, #40]
	bl 0x0200924c
	cmp r0, #0
	bne .L_02000adc_0
	ldr r0, [pc, #32]
	bl 0x02009254
	movs r1, #240
	movs r2, #206
	movs r0, #12
	lsls r1, r1, #15
	lsls r2, r2, #18
	bl 0x020092cc
	ldr r1, [pc, #16]
	movs r0, #12
	bl 0x0200929c
.L_02000adc_0:
	pop {r0}
	bx r0
	.4byte 0x000009b7
	.4byte 0x0000020e
	.4byte 0x020097a8
	.global Func_02000b14
	.thumb_func
Func_02000b14:
	ldr r3, [pc, #8]
	ldr r3, [r3]
	movs r2, #1
	adds r3, #52
	strb r2, [r3]
	bx lr
	.4byte 0x03001f30
	push	{r5, r6, lr}
	ldr	r3, [pc, #184]
	adds	r6, r1, #0
	ldr	r3, [r3, #0]
	movs	r1, #193
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #99
	bne.n	.L_02000b3e
	movs	r3, #0
	strh	r3, [r2, #0]
.L_02000b3e:
	ldr	r0, [pc, #164]
	bl 0x0200925c
	ldr	r3, [pc, #160]
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #152]
	cmp	r2, r3
	bne.n	.L_02000b60
	ldr	r2, [pc, #152]
	adds	r0, r6, r2
	bl 0x02009254
	b.n	.L_02000b6e
.L_02000b60:
	ldr	r3, [pc, #144]
	cmp	r2, r3
	bne.n	.L_02000b6e
	ldr	r3, [pc, #144]
	adds	r0, r6, r3
	bl 0x02009254
.L_02000b6e:
	movs	r0, #132
	lsls	r0, r0, #2
	movs	r1, #0
	bl 0x0200926c
	movs	r0, #98
	movs	r1, #5
	bl 0x0200933c
	ldr	r1, [pc, #100]
	ldr	r3, [pc, #120]
	adds	r2, r1, r3
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r5, r1, #0
	movs	r1, #224
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #92]
	cmp	r2, r3
	bne.n	.L_02000bc6
	cmp	r6, #11
	bne.n	.L_02000baa
	movs	r0, #98
	movs	r1, #7
	bl 0x0200933c
	b.n	.L_02000bc6
.L_02000baa:
	cmp	r6, #12
	bne.n	.L_02000bc6
	movs	r1, #6
	movs	r0, #98
	bl 0x0200933c
	movs	r0, #12
	bl 0x020092a4
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x020092cc
.L_02000bc6:
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl 0x0200928c
	movs	r3, #3
	adds	r0, #85
	strb	r3, [r0, #0]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0000020f
	.4byte 0x02000240
	.4byte 0x000000a4
	.4byte 0x000002f9
	.4byte 0x000000a5
	.4byte 0x00000309
	.2byte 0x022b
	.2byte 0x0000
	.global Func_02000c00
	.thumb_func
Func_02000c00:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #134
	lsls r0, r0, #2
	bl 0x02009264
	ldr r5, [pc, #196]
	movs r1, #250
	lsls r1, r1, #1
	adds r5, r5, r1
	adds r6, r0, #0
	ldr r0, [r5]
	bl 0x0200928c
	adds r7, r0, #0
	adds r0, r6, #0
	bl 0x0200928c
	mov r8, r0
	bl 0x0200927c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r1, r1
	negs r0, r0
	bl 0x0200932c
	movs r0, #219
	bl 0x02009374
	ldr r0, [r5]
	movs r1, #0
	bl 0x0200923c
	mov r2, r8
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	adds r2, r7, #0
	adds r2, #85
	strb r3, [r2]
	str r3, [r7, #40]
	adds r2, #12
	movs r3, #1
	strb r3, [r2]
	mov r2, r8
	adds r2, #97
	strb r3, [r2]
	ldr r6, [pc, #108]
	movs r5, #59
.L_02000c00_0:
	ldr r3, [r7, #40]
	adds r3, r3, r6
	str r3, [r7, #40]
	mov r2, r8
	ldr r3, [r2, #40]
	adds r3, r3, r6
	str r3, [r2, #40]
	movs r0, #1
	subs r5, #1
	bl 0x020091f4
	cmp r5, #0
	bge .L_02000c00_0
	bl 0x0200934c
	bl 0x02009354
	bl 0x02009284
	movs r0, #145
	lsls r0, r0, #1
	bl 0x02009254
	ldr r3, [pc, #56]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02000c00_1
	movs r0, #134
	lsls r0, r0, #2
	bl 0x02009264
	cmp r0, #11
	bne .L_02000c00_1
	ldr r0, [pc, #36]
	movs r1, #77
	bl 0x02009334
	b .L_02000c00_2
.L_02000c00_1:
	ldr r0, [pc, #28]
	movs r1, #27
	bl 0x02009334
.L_02000c00_2:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x00003333
	.4byte 0x000000a5
	.4byte 0x00000002
	.global Func_02000ce4
	.thumb_func
Func_02000ce4:
	push {lr}
	ldr r3, [pc, #48]
	ldr r3, [r3]
	movs r2, #63
	ands r3, r2
	lsls r3, r3, #16
	lsrs r2, r3, #16
	cmp r2, #31
	bls .L_02000ce4_0
	ldr r3, [pc, #28]
	subs r3, r3, r2
	lsls r3, r3, #16
.L_02000ce4_0:
	lsrs r3, r3, #17
	adds r3, #7
	lsls r1, r3, #5
	lsls r2, r3, #10
	orrs r2, r1
	orrs r3, r2
	lsls r3, r3, #16
	ldr r2, [pc, #16]
	lsrs r3, r3, #16
	strh r3, [r2]
	b .L_02000ce4_1
	.2byte 0x0000
	.4byte 0x00000040
	.4byte 0x03001e40
	.4byte 0x0500019e
.L_02000ce4_1:
	pop {r0}
	bx r0
	.global Func_02000d24
	.thumb_func
Func_02000d24:
	push {lr}
	ldr r3, [pc, #140]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #132]
	sub sp, #8
	cmp r2, r3
	bne 0x02008dae
	movs r0, #14
	bl 0x0200928c
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r0, #14
	bl 0x0200928c
	movs r3, #3
	adds r0, #85
	strb r3, [r0]
	movs r1, #0
	movs r0, #14
	movs r2, #0
	bl 0x020092cc
	movs r3, #15
	movs r2, #44
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #16
	movs r1, #44
	movs r2, #1
	bl 0x02009234
	movs r0, #100
	movs r1, #0
	movs r2, #0
	bl 0x0200935c
	movs r3, #127
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #12
	movs r1, #71
	movs r2, #1
	movs r3, #1
	bl 0x02009234
	movs r3, #12
	movs r2, #71
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #71
.L_02000d96:
	movs r2, #1
	movs r3, #1
	movs r0, #11
	bl 0x02009234
	ldr r0, [pc, #24]
.L_02000da2:
	bl 0x02009204
	ldr r3, [pc, #24]
	ldrh r2, [r3]
	ldr r3, [pc, #24]
	strh r2, [r3]
.L_02000dae:
	sub sp, #-8
	pop {r1}
	bx r1
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x00a5
	.2byte 0x0000
	.2byte 0x8ce5
	.2byte 0x0200
	.2byte 0x9a00
	.2byte 0x0200
	.2byte 0x019e
	.2byte 0x0500
	.global Func_02000dc8
	.thumb_func
Func_02000dc8:
	push {r5, lr}
	ldr r3, [pc, #132]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #124]
	sub sp, #8
	cmp r2, r3
	bne .L_02000dc8_0
	movs r0, #14
	bl 0x0200928c
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r0, #14
	bl 0x0200928c
	movs r5, #0
	adds r0, #85
	movs r1, #248
	movs r2, #178
	strb r5, [r0]
	lsls r1, r1, #16
	movs r0, #14
	lsls r2, r2, #18
	bl 0x020092cc
	movs r3, #15
	movs r2, #44
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #31
	movs r1, #95
	movs r2, #1
	bl 0x02009234
	movs r1, #1
	movs r2, #1
	movs r0, #100
	negs r1, r1
	negs r2, r2
	bl 0x0200935c
	bl 0x02009364
	movs r3, #12
	movs r2, #71
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #127
	movs r1, #127
	movs r2, #1
	movs r3, #1
	bl 0x02009234
	movs r1, #200
	ldr r0, [pc, #20]
	lsls r1, r1, #4
	bl 0x020091fc
.L_02000dc8_0:
	sub sp, #-8
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x000000a5
	.4byte 0x02008ce5
	.global Func_02000e5c
	.thumb_func
Func_02000e5c:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000e5c_0
	ldr r0, [pc, #16]
	b .L_02000e5c_1
.L_02000e5c_0:
	ldr r0, [pc, #16]
.L_02000e5c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000a6
	.4byte 0x020099c4
	.4byte 0x020097b4
	.global Func_02000e8c
	.thumb_func
Func_02000e8c:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #156]
	movs r1, #224
	ldr r7, [r3]
	ldr r3, [r3, #76]
	lsls r1, r1, #1
	ldr r2, [pc, #148]
	adds r3, r3, r1
	movs r0, #132
	str r2, [r3]
	lsls r0, r0, #2
	bl 0x02009264
	cmp r0, #0
	beq .L_02000e8c_0
	ldr r3, [pc, #136]
	movs r1, #249
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #2
	movs r1, #200
	strb r3, [r2]
	ldr r0, [pc, #124]
	lsls r1, r1, #4
	bl 0x020091fc
.L_02000e8c_0:
	ldr r5, [pc, #112]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r6, [pc, #108]
	cmp r2, r6
	beq .L_02000e8c_1
	ldr r3, [pc, #108]
	cmp r2, r3
	bne .L_02000e8c_2
.L_02000e8c_1:
	ldr r2, [pc, #104]
	ldr r3, [pc, #108]
	ldrh r2, [r2]
	strh r2, [r3]
	bl 0x02008d24
.L_02000e8c_2:
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	cmp r2, r6
	bne .L_02000e8c_3
	bl 0x02008f50
	b .L_02000e8c_4
.L_02000e8c_3:
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_02000e8c_5
	bl 0x02009094
	b .L_02000e8c_4
.L_02000e8c_5:
	movs r0, #144
	lsls r0, r0, #1
	bl 0x02009374
.L_02000e8c_4:
	ldr r3, [pc, #36]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_02000e8c_6
	ldrh r2, [r7, #20]
	ldr r3, [pc, #44]
	ands r3, r2
	strh r3, [r7, #20]
.L_02000e8c_6:
	movs r0, #0
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x03001e70
	.4byte 0x00000201
	.4byte 0x02000240
	.4byte 0x02008401
	.4byte 0x000000a4
	.4byte 0x000000a5
	.4byte 0x0500019e
	.4byte 0x02009a00
	.4byte 0x0000fdff
	.global Func_02000f50
	.thumb_func
Func_02000f50:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #0
	mov r8, r0
	ldr r0, [pc, #256]
	bl 0x0200924c
	cmp r0, #0
	beq .L_02000f50_0
	ldr r0, [pc, #248]
	bl 0x02009254
.L_02000f50_0:
	ldr r0, [pc, #248]
	bl 0x0200924c
	cmp r0, #0
	beq .L_02000f50_1
	ldr r0, [pc, #240]
	bl 0x02009254
.L_02000f50_1:
	ldr r0, [pc, #240]
	bl 0x0200924c
	cmp r0, #0
	beq .L_02000f50_2
	movs r0, #130
	lsls r0, r0, #2
	bl 0x02009254
.L_02000f50_2:
	movs r0, #193
	lsls r0, r0, #2
	bl 0x0200924c
	cmp r0, #0
	beq .L_02000f50_3
	ldr r0, [pc, #212]
	bl 0x02009254
.L_02000f50_3:
	ldr r0, [pc, #212]
	bl 0x0200924c
	cmp r0, #0
	beq .L_02000f50_4
	ldr r0, [pc, #204]
	bl 0x02009254
.L_02000f50_4:
	movs r7, #128
	movs r6, #8
	lsls r7, r7, #4
.L_02000f50_7:
	adds r0, r6, #0
	bl 0x0200928c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000f50_5
	ldr r0, [pc, #184]
	bl 0x0200924c
	cmp r0, #0
	bne .L_02000f50_6
	str r7, [r5, #24]
	str r7, [r5, #28]
.L_02000f50_6:
	ldr r3, [r5, #80]
	movs r2, #0
	adds r3, #38
	strb r2, [r3]
.L_02000f50_5:
	adds r6, #1
	cmp r6, #12
	ble .L_02000f50_7
	ldr r6, [pc, #160]
	ldr r5, [pc, #164]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_02000f50_8
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r6
	strh r2, [r6]
	ldr r2, [pc, #140]
	adds r3, #4
	stmia r3!, {r2}
	ldr r2, [pc, #140]
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_02000f50_8:
	strh r1, [r5]
	movs r0, #208
	lsls r0, r0, #2
	bl 0x0200924c
	cmp r0, #0
	beq .L_02000f50_9
	movs r3, #16
	movs r0, #244
	mov r8, r3
	bl 0x0200936c
.L_02000f50_9:
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r3, [r6]
	cmp r3, #31
	bgt .L_02000f50_10
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	mov r0, r8
	strh r3, [r6]
	movs r3, #16
	lsls r2, r2, #2
	subs r3, r3, r0
	adds r2, r2, r6
	lsls r3, r3, #8
	adds r2, #4
	orrs r3, r0
	stmia r2!, {r3}
	ldr r3, [pc, #72]
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2]
.L_02000f50_10:
	strh r1, [r5]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00000301
	.4byte 0x00000206
	.4byte 0x00000302
	.4byte 0x00000207
	.4byte 0x00000303
	.4byte 0x00000209
	.4byte 0x00000305
	.4byte 0x0000020a
	.4byte 0x00000109
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x00003f42
	.4byte 0x04000050
	.4byte 0x04000052
	.global Func_02001094
	.thumb_func
Func_02001094:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #0
	mov r8, r0
	ldr r0, [pc, #280]
	bl 0x0200924c
	cmp r0, #0
	beq .L_02001094_0
	ldr r0, [pc, #272]
	bl 0x02009254
.L_02001094_0:
	ldr r0, [pc, #272]
	bl 0x0200924c
	cmp r0, #0
	beq .L_02001094_1
	ldr r0, [pc, #264]
	bl 0x02009254
.L_02001094_1:
	ldr r0, [pc, #264]
	bl 0x0200924c
	cmp r0, #0
	beq .L_02001094_2
	movs r0, #130
	lsls r0, r0, #2
	bl 0x02009254
.L_02001094_2:
	movs r7, #128
	movs r6, #8
	lsls r7, r7, #4
.L_02001094_5:
	adds r0, r6, #0
	bl 0x0200928c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001094_3
	ldr r0, [pc, #232]
	bl 0x0200924c
	cmp r0, #0
	bne .L_02001094_4
	str r7, [r5, #24]
	str r7, [r5, #28]
.L_02001094_4:
	ldr r0, [r5, #80]
	adds r2, r0, #0
	adds r2, #38
	movs r3, #0
	strb r3, [r2]
.L_02001094_3:
	adds r6, #1
	cmp r6, #10
	ble .L_02001094_5
	movs r0, #11
	bl 0x0200928c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001094_6
	ldr r0, [r5, #80]
	ldr r2, [r0, #40]
	cmp r2, #0
	beq .L_02001094_7
	movs r3, #10
	strb r3, [r2, #5]
.L_02001094_7:
	adds r1, r0, #0
	movs r2, #1
	adds r1, #37
	strb r2, [r1]
	adds r2, r0, #0
	movs r3, #0
	adds r2, #38
	strb r3, [r2]
.L_02001094_6:
	ldr r0, [pc, #164]
	bl 0x0200924c
	cmp r0, #0
	beq .L_02001094_8
	ldr r0, [pc, #160]
	bl 0x02009254
.L_02001094_8:
	ldr r6, [pc, #156]
	ldr r5, [pc, #160]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_02001094_9
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r6
	strh r2, [r6]
	ldr r2, [pc, #136]
	adds r3, #4
	stmia r3!, {r2}
	ldr r2, [pc, #136]
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_02001094_9:
	strh r1, [r5]
	movs r0, #208
	lsls r0, r0, #2
	bl 0x0200924c
	cmp r0, #0
	beq .L_02001094_10
	movs r3, #16
	movs r0, #244
	mov r8, r3
	bl 0x0200936c
.L_02001094_10:
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r3, [r6]
	cmp r3, #31
	bgt .L_02001094_11
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	mov r0, r8
	strh r3, [r6]
	movs r3, #16
	lsls r2, r2, #2
	subs r3, r3, r0
	adds r2, r2, r6
	lsls r3, r3, #8
	adds r2, #4
	orrs r3, r0
	stmia r2!, {r3}
	ldr r3, [pc, #68]
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2]
.L_02001094_11:
	strh r1, [r5]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00000311
	.4byte 0x00000206
	.4byte 0x00000312
	.4byte 0x00000207
	.4byte 0x00000313
	.4byte 0x00000109
	.4byte 0x00000315
	.4byte 0x000009b7
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x00003f42
	.4byte 0x04000050
	.4byte 0x04000052
	.section .rodata,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
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
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
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
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
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
	.4byte 0x0000001b
	.4byte 0x0200937c
	.4byte 0x020093b4
	.4byte 0x020093ec
	.4byte 0x00000022
	.4byte 0x02008325
	.4byte 0x00000022
	.4byte 0x0200833d
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x02008325
	.4byte 0x00000022
	.4byte 0x0200833d
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x00000100
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000108
	.4byte 0x40000038
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000018
	.4byte 0x000003a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000001e8
	.4byte 0x800001c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001e8
	.4byte 0x800004b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000001b8
	.4byte 0x40000270
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000038
	.4byte 0x40000270
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000048
	.4byte 0xc00000f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001b8
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000278
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000288
	.4byte 0xc0000118
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000a4
	.4byte 0x0011c002
	.4byte 0x002010a5
	.4byte 0x000000a5
	.4byte 0x001020a4
	.4byte 0x00237002
	.4byte 0x003040a6
	.4byte 0x004010a6
	.4byte 0x000000a6
	.4byte 0x001040a5
	.4byte 0x002030a6
	.4byte 0x003020a6
	.4byte 0x004030a5
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03010121
	.4byte 0x02009430
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0x03020121
	.4byte 0x02009430
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0x03030121
	.4byte 0x02009430
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x01024000
	.4byte 0x03040121
	.4byte 0x02009430
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x01024000
	.4byte 0x03050121
	.4byte 0x02009430
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x01024000
	.4byte 0xffff0039
	.4byte 0x00000001
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
	.4byte 0x03110121
	.4byte 0x02009430
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0x03120121
	.4byte 0x02009430
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0x03130121
	.4byte 0x02009430
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01024000
	.4byte 0x03140121
	.4byte 0x02009444
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00024000
	.4byte 0x03150121
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x005d005c
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x020089cd
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000c401
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0x09b50009
	.4byte 0x02008839
	.4byte 0x00000002
	.4byte 0x03010029
	.4byte 0x02008559
	.4byte 0x00000002
	.4byte 0x0302002a
	.4byte 0x02008565
	.4byte 0x00000002
	.4byte 0x0303002b
	.4byte 0x02008571
	.4byte 0x00000002
	.4byte 0x0304002c
	.4byte 0x0200857d
	.4byte 0x00000002
	.4byte 0x0305002d
	.4byte 0x02008589
	.4byte 0x00000002
	.4byte 0x03110033
	.4byte 0x02008559
	.4byte 0x00000002
	.4byte 0x03120034
	.4byte 0x02008565
	.4byte 0x00000002
	.4byte 0x03130035
	.4byte 0x02008571
	.4byte 0x00000002
	.4byte 0x0206003d
	.4byte 0x020087fd
	.4byte 0x00000002
	.4byte 0x0207003e
	.4byte 0x02008809
	.4byte 0x00000002
	.4byte 0x0208003f
	.4byte 0x02008815
	.4byte 0x00000002
	.4byte 0x02090040
	.4byte 0x02008821
	.4byte 0x00000002
	.4byte 0x020a0041
	.4byte 0x0200882d
	.4byte 0x00000002
	.4byte 0x02060047
	.4byte 0x020087fd
	.4byte 0x00000002
	.4byte 0x02070048
	.4byte 0x02008809
	.4byte 0x00000002
	.4byte 0x02080049
	.4byte 0x02008815
	.4byte 0x00000002
	.4byte 0x03140036
	.4byte 0x0200857d
	.4byte 0x00000002
	.4byte 0x020e0051
	.4byte 0x02008add
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008315
	.4byte 0x00000003
	.4byte 0x03510064
	.4byte 0x00300000
	.4byte 0x00000013
	.4byte 0x0fb10067
	.4byte 0x0010008d
	.4byte 0x00000013
	.4byte 0x0fb20068
	.4byte 0x001000e5
	.4byte 0x00000013
	.4byte 0x0fb30065
	.4byte 0x001000c0
	.4byte 0x00000013
	.4byte 0x0ef60066
	.4byte 0x00500006
	.4byte 0x10002115
	.4byte 0x120f0008
	.4byte 0x02008b15
	.4byte 0x10002115
	.4byte 0x120f0009
	.4byte 0x02008b15
	.4byte 0x10002115
	.4byte 0x120f000a
	.4byte 0x02008b15
	.4byte 0x10002115
	.4byte 0x120f000b
	.4byte 0x02008b15
	.4byte 0x10002115
	.4byte 0x120f000c
	.4byte 0x02008b15
	.4byte 0x00002115
	.4byte 0x120f0008
	.4byte 0x02008b25
	.4byte 0x00002115
	.4byte 0x120f0009
	.4byte 0x02008b25
	.4byte 0x00002115
	.4byte 0x120f000a
	.4byte 0x02008b25
	.4byte 0x00002115
	.4byte 0x120f000b
	.4byte 0x02008b25
	.4byte 0x00002115
	.4byte 0x120f000c
	.4byte 0x02008b25
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008dc9
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008d25
	.4byte 0x00000006
	.4byte 0xffff0063
	.4byte 0x02008c01
	.4byte 0x00000006
	.4byte 0xffff0037
	.4byte 0x02008589
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
