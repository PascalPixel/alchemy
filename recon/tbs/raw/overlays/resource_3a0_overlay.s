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
	bl 0x02009370
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
	bl 0x02009390
	adds r0, r5, #0
	movs r1, #14
	bl 0x02009458
	adds r0, r5, #0
	movs r1, #1
	bl 0x02009398
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
	bl 0x02009370
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
	bl 0x02009390
	adds r0, r5, #0
	movs r1, #15
	bl 0x02009458
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
	.section .text.x0200813c,"ax",%progbits
	.align 2
	.global Effect_Spawn
	.thumb_func
Effect_Spawn:
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
	bl 0x020093d0
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
	bl 0x02009370
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
	bl 0x02009360
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x02009368
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
	bl 0x02009458
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
	bl 0x02009330
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
	bl 0x02009330
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200013c_9:
	bl 0x02009330
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
	bl 0x02009360
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x02009368
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
	.4byte 0x020095a4
	.4byte 0x02008105
	.4byte 0xffff0000
	.global Func_02000314
	.thumb_func
Func_02000314:
	push {lr}
	movs r0, #22
	movs r1, #1
	movs r2, #2
	bl 0x020094a0
	pop {r0}
	bx r0
	.section .text.x02008358,"ax",%progbits
	.align 2
	.global SceneEffect_AdvanceRotatingSprite
	.thumb_func
SceneEffect_AdvanceRotatingSprite:
	.global Func_02000358
	.thumb_func
Func_02000358:
	push {lr}
	movs r0, #19
	bl 0x020093d0
	ldr r2, [r0, #80]
	movs r1, #160
	ldrh r3, [r2, #30]
	lsls r1, r1, #5
	adds r3, r3, r1
	strh r3, [r2, #30]
	pop {r0}
	bx r0
	.global SceneEffect_SpawnPeriodicEffect
	.thumb_func
SceneEffect_SpawnPeriodicEffect:
	.global Func_02000370
	.thumb_func
Func_02000370:
	push {r5, lr}
	movs r0, #14
	sub sp, #56
	bl 0x020093d0
	ldr r3, [pc, #68]
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	adds r5, r0, #0
	cmp r3, #0
	bne .L_02000370_0
	add r4, sp, #16
	movs r3, #1
	str r3, [r4]
	movs r3, #9
	str r3, [r4, #4]
	movs r3, #169
	strh r3, [r4, #24]
	ldr r3, [pc, #44]
	ldr r2, [r5, #16]
	str r3, [r4, #28]
	ldr r3, [pc, #40]
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	adds r2, r2, r3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r3, #204
	lsls r3, r3, #14
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #12]
	bl 0x0200813c
.L_02000370_0:
	sub sp, #-56
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e40
	.4byte 0x02009740
	.4byte 0xffff0000
	.global SceneEffect_CalculatePositionDistance
	.thumb_func
SceneEffect_CalculatePositionDistance:
	.global Func_020003cc
	.thumb_func
Func_020003cc:
	push {r5, lr}
	ldmia r0!, {r5}
	ldmia r1!, {r3}
	ldmia r0!, {r4}
	subs r5, r5, r3
	ldmia r1!, {r3}
	ldr r2, [r1]
	subs r4, r4, r3
	ldr r3, [r0]
	subs r3, r3, r2
	asrs r5, r5, #16
	asrs r4, r4, #16
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
	ldr r3, [pc, #8]
	bl 0x020094cc
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x030001d8
	.section .text.x0200859c,"ax",%progbits
	.align 2
	.global Func_0200059c
	.thumb_func
Func_0200059c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020097e8
	.global Func_020005a4
	.thumb_func
Func_020005a4:
	movs r0, #0
	bx lr
	.global Func_020005a8
	.thumb_func
Func_020005a8:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020098d8
	.global Func_020005b0
	.thumb_func
Func_020005b0:
	push {lr}
	ldr r0, [pc, #24]
	bl 0x020093a0
	cmp r0, #0
	beq .L_020005b0_0
	ldr r3, [pc, #16]
	movs r2, #0
	adds r3, #190
	strb r2, [r3]
.L_020005b0_0:
	ldr r0, [pc, #8]
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00000895
	.4byte 0x02009900
	.global Func_020005d4
	.thumb_func
Func_020005d4:
	push {lr}
	bl 0x020093c0
	ldr r0, [pc, #20]
	bl 0x02009460
	movs r1, #0
	movs r0, #9
	bl 0x02009478
	bl 0x020093c8
	pop {r0}
	bx r0
	.4byte 0x000017e8
	.global Func_020005f4
	.thumb_func
Func_020005f4:
	push {r5, r6, r7, lr}
	movs r0, #20
	sub sp, #8
	bl 0x020093d0
	adds r6, r0, #0
	bl 0x020093c0
	movs r0, #18
	bl 0x020093d0
	movs r7, #0
	str r7, [r0, #108]
	movs r0, #128
	lsls r0, r0, #2
	bl 0x020093a0
	cmp r0, #0
	bne .L_020005f4_0
	movs r0, #18
	bl 0x020093d0
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #19
	bgt .L_020005f4_1
.L_020005f4_0:
	movs r0, #18
	bl 0x020093d0
	movs r1, #0
	movs r2, #0
	ldrh r5, [r0, #6]
	movs r0, #18
	bl 0x02009448
	movs r0, #10
	bl 0x020093b8
	ldr r0, [pc, #732]
	bl 0x02009460
	movs r0, #128
	lsls r0, r0, #2
	bl 0x020093a0
	cmp r0, #0
	bne .L_020005f4_2
	ldr r3, [pc, #720]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r0, #18
	bl 0x02009468
	movs r0, #18
	bl 0x020093d0
	adds r0, #100
	strh r7, [r0]
	movs r0, #18
	bl 0x020093d0
	strh r5, [r0, #6]
	b .L_020005f4_3
.L_020005f4_2:
	movs r0, #18
	movs r1, #0
	bl 0x02009468
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009480
.L_020005f4_3:
	movs r0, #18
	bl 0x020093d0
	ldr r3, [pc, #652]
	str r3, [r0, #108]
	bl 0x020093c8
	b 0x02008916
.L_020005f4_1:
	movs r0, #0
	bl 0x020093d0
	ldr r3, [r0, #16]
	asrs r3, r3, #19
	cmp r3, #27
	ble .L_020005f4_4
	movs r0, #0
	bl 0x020093d0
	ldr r3, [r0, #16]
	asrs r3, r3, #19
	cmp r3, #29
	bgt .L_020005f4_4
	movs r0, #0
	bl 0x020093d0
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #26
	beq .L_020005f4_4
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x020093d8
	movs r2, #0
	movs r1, #18
	movs r0, #0
	bl 0x02009448
	movs r0, #5
	bl 0x020093b8
	movs r0, #0
	bl 0x020093d0
	adds r5, r0, #0
	movs r0, #18
	bl 0x020093d0
	ldr r2, [r5, #8]
	ldr r3, [r0, #8]
	cmp r2, r3
	bge .L_020005f4_5
	movs r0, #0
	bl 0x020093d0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r0, #18
	bl 0x020093d0
	ldr r1, [r0, #8]
	asrs r1, r1, #20
	lsls r1, r1, #4
	subs r1, #8
	movs r0, #0
	movs r2, #232
	bl 0x02009400
	movs r7, #1
	b .L_020005f4_6
.L_020005f4_5:
	movs r0, #0
	bl 0x020093d0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r0, #18
	bl 0x020093d0
	ldr r1, [r0, #8]
	asrs r1, r1, #20
	lsls r1, r1, #4
	adds r1, #24
	movs r0, #0
	movs r2, #232
	bl 0x02009400
.L_020005f4_6:
	movs r0, #0
	bl 0x02009420
.L_020005f4_4:
	movs r0, #18
	bl 0x020093d0
	movs r5, #128
	lsls r5, r5, #24
	str r5, [r0, #56]
	movs r0, #18
	bl 0x020093d0
	str r5, [r0, #60]
	movs r0, #18
	bl 0x020093d0
	movs r1, #1
	str r5, [r0, #64]
	movs r0, #18
	bl 0x020093e0
	movs r0, #18
	movs r1, #1
	bl 0x02009430
	movs r1, #2
	movs r0, #18
	bl 0x02009440
	movs r0, #10
	bl 0x020093b8
	movs r0, #228
	bl 0x020094b8
	ldr r3, [pc, #404]
	movs r0, #18
	str r3, [r6, #24]
	str r3, [r6, #28]
	bl 0x020093d0
	ldr r5, [r0, #8]
	movs r0, #18
	bl 0x020093d0
	ldr r2, [r0, #16]
	asrs r5, r5, #20
	movs r3, #128
	lsls r3, r3, #12
	asrs r2, r2, #20
	lsls r5, r5, #20
	adds r5, r5, r3
	lsls r2, r2, #20
	adds r2, r2, r3
	adds r1, r5, #0
	movs r0, #20
	bl 0x02009428
	movs r0, #18
	bl 0x020093d0
	ldr r5, [r0, #8]
	movs r0, #18
	bl 0x020093d0
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #1
	asrs r5, r5, #20
	movs r0, #16
	movs r1, #16
	str r5, [sp, #0]
	bl 0x02009380
	movs r1, #2
	movs r0, #20
	bl 0x02009488
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	ldr r5, [pc, #304]
.L_020005f4_7:
	movs r0, #3
	bl 0x02009338
	ldr r3, [r6, #28]
	ldr r2, [r6, #24]
	adds r3, r3, r5
	str r3, [r6, #28]
	ldr r3, [pc, #292]
	adds r2, r2, r5
	str r2, [r6, #24]
	cmp r2, r3
	ble .L_020005f4_7
	movs r0, #18
	ldr r1, [pc, #284]
	movs r2, #70
	bl 0x02009490
	movs r1, #0
	movs r2, #0
	movs r0, #18
	bl 0x02009448
.L_0200082a:
	movs r0, #20
	bl 0x020093b8
	movs r2, #0
	movs r0, #18
	ldr r1, [pc, #260]
	bl 0x02009490
	movs r1, #2
	movs r0, #18
	bl 0x02009438
	movs r0, #70
	bl 0x020093b8
	ldr r0, [pc, #244]
	bl 0x02009460
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x02009470
	bl 0x020094b0
	movs r0, #0
	bl 0x020093d0
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #26
	bne .L_0200082a_0
	movs r0, #0
	bl 0x020093d0
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #13
	ble .L_0200082a_0
	movs r7, #1
.L_0200082a_0:
	cmp r7, #0
	beq .L_0200082a_1
	movs r0, #0
	ldr r1, [pc, #192]
	ldr r2, [pc, #196]
	bl 0x020093d8
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #10
	movs r0, #0
	bl 0x02009480
	movs r0, #0
	bl 0x020093d0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #0
	bl 0x02009430
	movs r1, #0
	movs r0, #0
	movs r2, #16
	bl 0x02009418
	movs r0, #0
	bl 0x02009420
	movs r0, #0
	movs r1, #1
	bl 0x02009430
.L_0200082a_1:
	movs r0, #18
	ldr r1, [pc, #124]
	ldr r2, [pc, #124]
	bl 0x020093d8
	movs r0, #18
	bl 0x020093d0
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #14
	beq .L_0200082a_2
	movs r0, #18
	bl 0x020093d0
	movs r2, #232
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r0, #18
	bl 0x02009408
.L_0200082a_2:
	movs r1, #140
	movs r2, #232
	lsls r1, r1, #1
	movs r0, #18
	bl 0x02009408
	movs r0, #128
	lsls r0, r0, #2
	bl 0x020093a8
	movs r0, #0
	bl 0x020093d0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	bl 0x020093c8
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x17fb
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x8501
	.2byte 0x0200
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x1999
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.4byte 0x00000103
	.4byte 0x000017fa
	.4byte 0x0000cccc
	.4byte 0x00006666
	.global Func_0200094c
	.thumb_func
Func_0200094c:
	push {lr}
	movs r0, #0
	bl 0x020093d0
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #13
	bgt .L_0200094c_0
	movs r0, #20
	movs r1, #1
	bl 0x02009488
.L_0200094c_0:
	pop {r0}
	bx r0
	.global Func_02000968
	.thumb_func
Func_02000968:
	push {r5, lr}
	sub sp, #8
	bl 0x020093c0
	movs r0, #20
	bl 0x020093d0
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #253
	ands r3, r2
	strb r3, [r0]
	movs r0, #20
	bl 0x020093d0
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #20
.L_0200098e:
	bl 0x020093d0
	ldr r5, [r0, #8]
	movs r0, #20
	bl 0x020093d0
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #1
	asrs r5, r5, #20
	movs r0, #3
	movs r1, #17
	str r5, [sp, #0]
	bl 0x02009380
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #28]
	bl 0x02009340
	ldr r0, [pc, #28]
	bl 0x020093a8
	movs r0, #20
	movs r1, #2
	bl 0x02009488
	bl 0x020093c8
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x02008325
	.4byte 0x00000201
	.section .text.x02008cec,"ax",%progbits
	.align 2
	.global Func_02000cec
	.thumb_func
Func_02000cec:
	push {lr}
	bl 0x020093c0
	ldr r0, [pc, #20]
	bl 0x02009460
.L_02000cf8:
	movs r1, #0
	movs r0, #17
	bl 0x02009478
	bl 0x020093c8
	pop {r0}
	bx r0
	.2byte 0x17f7
	.2byte 0x0000
	.section .text.x02008de8,"ax",%progbits
	.align 2
	.global Func_02000de8
	.thumb_func
Func_02000de8:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #0
	bl 0x020093d0
	movs r3, #0
	adds r0, #85
	movs r1, #128
	movs r2, #128
	strb r3, [r0]
	lsls r1, r1, #8
	movs r0, #0
	lsls r2, r2, #7
	bl 0x020093d8
	cmp r5, #6
	bne .L_02000de8_0
	movs r0, #0
	movs r1, #2
	bl 0x02009430
	movs r2, #16
	movs r0, #0
	movs r1, #0
	negs r2, r2
	bl 0x02009418
	b .L_02000de8_1
.L_02000de8_0:
	movs r2, #16
	movs r0, #0
	movs r1, #2
	negs r2, r2
	bl 0x02009410
.L_02000de8_1:
	ldr r3, [pc, #24]
	movs r2, #228
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #16
	str r2, [r3]
	adds r0, r5, #0
	bl 0x02009498
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.global Func_02000e4c
	.thumb_func
Func_02000e4c:
	push {r5, lr}
	ldr r3, [pc, #196]
	ldr r5, [r3]
	bl 0x020093c0
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #6
	bhi .L_02000e4c_0
	ldr r2, [pc, #176]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	ldrh r4, [r1, #52]
	lsls r0, r0, #8
	ldrh r6, [r3, #52]
	lsls r0, r0, #8
	ldrh r2, [r5, #52]
	lsls r0, r0, #8
	ldrh r6, [r6, #52]
	lsls r0, r0, #8
	ldrh r0, [r1, #54]
	lsls r0, r0, #8
	ldrh r2, [r3, #54]
	lsls r0, r0, #8
	ldrh r4, [r5, #54]
	lsls r0, r0, #8
	movs r0, #158
	bl 0x020094b8
	ldr r0, [pc, #136]
	movs r1, #81
	movs r2, #18
	bl 0x02009378
	b .L_02000e4c_0
	.2byte 0x209e
	.2byte 0xf000
	.2byte 0xfb0a
	.2byte 0x481e
	.2byte 0x2153
	.2byte 0xe01c
	.2byte 0x209e
	.2byte 0xf000
	.2byte 0xfb04
	.2byte 0x481b
	.2byte 0x2156
	.2byte 0xe016
	.2byte 0x209e
	.2byte 0xf000
	.2byte 0xfafe
	.2byte 0x4819
	.2byte 0x2154
	.2byte 0x2218
	.2byte 0xf000
	.2byte 0xfa59
	.2byte 0xe019
	.2byte 0x209e
	.2byte 0xf000
	.2byte 0xfaf5
	.2byte 0x4815
	.2byte 0x2148
	.2byte 0x2207
	.2byte 0xf000
	.2byte 0xfa50
	.2byte 0xe010
	.2byte 0x20bc
	.2byte 0xf000
	.2byte 0xfaec
	.2byte 0x4811
	.2byte 0x2145
	.2byte 0x220b
	.2byte 0xf000
	.2byte 0xfa47
	.2byte 0xe007
	.2byte 0x209e
	.2byte 0xf000
	.2byte 0xfae3
	.2byte 0x480e
	.2byte 0x2153
	.2byte 0x2207
	.2byte 0xf000
	.2byte 0xfa3e
.L_02000e4c_0:
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl 0x02008de8
	bl 0x020093c8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x02008e70
	.4byte 0x02009778
	.2byte 0x978e
	.2byte 0x0200
	.2byte 0x97a4
	.2byte 0x0200
	.2byte 0x97ba
	.2byte 0x0200
	.2byte 0x97d0
	.2byte 0x0200
	.section .text.x02009060,"ax",%progbits
	.align 2
	.global Func_02001060
	.thumb_func
Func_02001060:
	push {r5, r6, lr}
	bl 0x020093c0
	movs r1, #1
	movs r0, #18
	bl 0x020093e0
	movs r0, #18
	bl 0x020093d0
	movs r6, #0
	str r6, [r0, #108]
	movs r0, #18
	bl 0x020093d0
	movs r5, #128
	lsls r5, r5, #24
	str r5, [r0, #56]
	movs r0, #18
	bl 0x020093d0
	str r5, [r0, #64]
	movs r0, #18
	bl 0x020093d0
	str r6, [r0, #36]
	movs r0, #18
	bl 0x020093d0
	str r6, [r0, #44]
	movs r0, #18
	bl 0x020093d0
	str r6, [r0, #48]
	movs r0, #18
	bl 0x020093d0
	movs r2, #0
	str r6, [r0, #52]
	ldr r1, [pc, #132]
	movs r0, #18
	bl 0x02009490
	movs r1, #2
	movs r0, #18
	bl 0x02009438
	movs r0, #60
	bl 0x020093b8
	movs r1, #192
	movs r2, #192
	movs r0, #18
.L_020010ca:
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x020093d8
	movs r1, #192
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x020093d8
	movs r1, #140
	movs r0, #18
	lsls r1, r1, #1
	movs r2, #232
	bl 0x02009400
	movs r1, #148
	lsls r1, r1, #1
	movs r2, #232
.L_020010f2:
	movs r0, #0
	bl 0x02009408
	movs r0, #18
	bl 0x02009420
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02009480
	movs r1, #129
	movs r2, #60
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02009490
	ldr r1, [pc, #32]
	movs r0, #18
	bl 0x020093e0
	movs r0, #18
	bl 0x020093d0
	ldr r3, [pc, #20]
	str r3, [r0, #108]
	bl 0x020093c8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0103
	.2byte 0x0000
	.4byte 0x020095b0
	.4byte 0x02008501
	.global Func_02001140
	.thumb_func
Func_02001140:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009ac8
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
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
	.4byte 0x020094fc
	.4byte 0x02009534
	.4byte 0x0200956c
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00006666
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x80010000
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x000000b4
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000010
	.global ShianMura_Actor19Motion
ShianMura_Actor19Motion:
	.4byte 0x00000015
	.4byte 0x0000001d
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00030000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00018000
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00300000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00030000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000015
	.4byte 0x0000001d
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000010
	.global ShianMura_EffectScript
ShianMura_EffectScript:
	.4byte 0x00000000
	.4byte 0x00000012
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00300040
	.4byte 0x00020001
	.4byte 0x003f0004
	.4byte 0x00010030
	.4byte 0x00040002
	.4byte 0x0045ffff
	.4byte 0x00020032
	.4byte 0x00040002
	.4byte 0x00320041
	.4byte 0x00020002
	.4byte 0xffff0004
	.4byte 0x002e0042
	.4byte 0x00020003
	.4byte 0x003f0004
	.4byte 0x0003002e
	.4byte 0x00040002
	.4byte 0x0043ffff
	.4byte 0x00040032
	.4byte 0x00040002
	.4byte 0x0032003f
	.4byte 0x00020004
	.4byte 0xffff0004
	.4byte 0x002c0042
	.4byte 0x00020003
	.4byte 0x003f0004
	.4byte 0x0003002c
	.4byte 0x00040002
	.4byte 0x0000ffff
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000178
	.4byte 0xc00001f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000158
	.4byte 0x40000148
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000178
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x000001a8
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000198
	.4byte 0x400001a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x000000d8
	.4byte 0x400000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000188
	.4byte 0x400000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x000000b0
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000048
	.4byte 0x00101049
	.4byte 0x00202049
	.4byte 0x00303049
	.4byte 0x00404049
	.4byte 0x00506049
	.4byte 0x00620009
	.4byte 0x0070203d
	.4byte 0x0080e002
	.4byte 0x000001ff
	.4byte 0xffff009c
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0xffff009f
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00002000
	.4byte 0xffff00a0
	.4byte 0x00000001
	.4byte 0x01a40000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00004000
	.4byte 0xffff00a4
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00003000
	.4byte 0xffff00a6
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0xffff00a8
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00013000
	.4byte 0xffff00a9
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00028000
	.4byte 0xffff00aa
	.4byte 0x00000001
	.4byte 0x01ac0000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x00008000
	.4byte 0xffff009f
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0000c000
	.4byte 0x189a0027
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01f00000
	.4byte 0x00033000
	.4byte 0xffff009f
	.4byte 0x020095b0
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00028000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00008000
	.4byte 0xffff00e3
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00008000
	.4byte 0x0046005b
	.4byte 0x00000001
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00008000
	.4byte 0xffff00df
	.4byte 0x00000001
	.4byte 0x01aa0000
	.4byte 0x00000000
	.4byte 0x01ac0000
	.4byte 0x00008000
	.4byte 0xffff00df
	.4byte 0x00000001
	.4byte 0x012b0000
	.4byte 0x00000000
	.4byte 0x00de0000
	.4byte 0x01008000
	.4byte 0xffff00df
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008e4d
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008e4d
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008e4d
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008e4d
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008e4d
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008e4d
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008e4d
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000202
	.4byte 0x02010014
	.4byte 0x02008d0d
	.4byte 0x00004602
	.4byte 0x02010015
	.4byte 0x02008d0d
	.4byte 0x00004602
	.4byte 0x02010002
	.4byte 0x02008d0d
	.4byte 0x00004602
	.4byte 0x02010003
	.4byte 0x02008d0d
	.4byte 0x00000000
	.4byte 0x08950008
	.4byte 0x000017e7
	.4byte 0x00000000
	.4byte 0x08950009
	.4byte 0x020085d5
	.4byte 0x00000000
	.4byte 0x0895000a
	.4byte 0x000017eb
	.4byte 0x00000000
	.4byte 0x0895000b
	.4byte 0x000017ef
	.4byte 0x00000000
	.4byte 0x0895000c
	.4byte 0x000017f0
	.4byte 0x00000000
	.4byte 0x0895000d
	.4byte 0x000017f1
	.4byte 0x00000000
	.4byte 0x0895000e
	.4byte 0x020089dd
	.4byte 0x00000000
	.4byte 0x0895000f
	.4byte 0x000017f5
	.4byte 0x00000000
	.4byte 0x08950010
	.4byte 0x000017f6
	.4byte 0x00000000
	.4byte 0x08950011
	.4byte 0x02008ced
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x020085f5
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001a24
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001a25
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001a26
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001a27
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001a28
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001a29
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001a2a
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001a2b
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001a2c
	.4byte 0x10001815
	.4byte 0x02010014
	.4byte 0x0200894d
	.4byte 0x00001815
	.4byte 0x02010014
	.4byte 0x02008969
	.4byte 0x00008e15
	.4byte 0xffff0015
	.4byte 0x02008f31
	.4byte 0x00008d15
	.4byte 0x08950008
	.4byte 0x000017ff
	.4byte 0x00008d15
	.4byte 0x08950009
	.4byte 0x00001800
	.4byte 0x00008d15
	.4byte 0x0895000a
	.4byte 0x00001801
	.4byte 0x00008d15
	.4byte 0x0895000b
	.4byte 0x00001803
	.4byte 0x00008d15
	.4byte 0x0895000c
	.4byte 0x00001804
	.4byte 0x00008d15
	.4byte 0x0895000d
	.4byte 0x00001805
	.4byte 0x00008d15
	.4byte 0x0895000e
	.4byte 0x00001806
	.4byte 0x00008d15
	.4byte 0x0895000f
	.4byte 0x00001807
	.4byte 0x00008d15
	.4byte 0x08950010
	.4byte 0x00001808
	.4byte 0x00008d15
	.4byte 0x08950011
	.4byte 0x00001809
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000180a
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a2d
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a2e
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001a2f
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001a30
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001a31
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001a32
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001a33
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001a34
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001a35
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001a36
	.4byte 0x00000023
	.4byte 0x0f6e0065
	.4byte 0x001000e5
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02008315
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte 0x02009061
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
