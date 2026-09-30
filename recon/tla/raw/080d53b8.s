.syntax unified
	.thumb
	.global Func_080d53b8
	.thumb_func
Func_080d53b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_080d5554
	sub sp, #24
	ldr r0, [r1]
	bl Object_GetById
	movs r2, #192
	lsls r2, r2, #18
	ldr r2, [r2, #32]
	movs r3, #1
	str r2, [sp, #8]
	str r3, [sp, #4]
	adds r7, r0, #0
	movs r0, #10
	ldrsh r3, [r7, r0]
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #240
	ands r3, r1
	adds r3, #8
	mov r11, r3
	movs r2, #14
	ldrsh r3, [r7, r2]
	movs r2, #16
	negs r2, r2
	ands r3, r2
	adds r5, r3, #0
	movs r0, #18
	ldrsh r3, [r7, r0]
	adds r5, #8
	ands r3, r1
	mov r8, r3
	movs r1, #8
	add r1, r8
	mov r9, r1
	bl Func_080d22a8
	adds r3, r7, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080d541e
	ldr r3, [r7, #80]
	ldrb r3, [r3, #26]
	str r3, [sp, #4]
.L_080d541e:
	ldr r2, .L_080d5558
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_080d5428
	b .L_080d5564
.L_080d5428:
	movs r1, #34
	adds r1, r1, r7
	asrs r5, r5, #4
	mov r0, r9
	mov r12, r5
	asrs r5, r0, #4
	ldrb r0, [r1]
	mov r3, r11
	asrs r6, r3, #4
	lsls r3, r0, #3
	mov r10, r1
	subs r3, r3, r0
	ldr r1, [sp, #8]
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r3, [r1, r3]
	lsls r2, r5, #7
	adds r2, r6, r2
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r2, .L_080d555c
	movs r4, #64
	adds r1, r3, r2
	ldrb r2, [r3, #3]
	adds r3, r4, #0
	ands r3, r2
	cmp r3, #0
	bne .L_080d5466
	b .L_080d5658
.L_080d5466:
	ldrb r2, [r1, #3]
	adds r3, r4, #0
	ands r3, r2
	cmp r3, #0
	bne .L_080d5472
	b .L_080d5658
.L_080d5472:
	ldr r3, [sp, #8]
	movs r1, #212
	lsls r1, r1, #1
	adds r2, r3, r1
	mov r1, r12
	subs r3, r5, r1
	lsls r3, r3, #7
	ldr r2, [r2]
	adds r3, r6, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	ldr r3, .L_080d555c
	adds r1, r2, r3
	ldrb r3, [r1, #2]
	cmp r3, #255
	bne .L_080d5494
	b .L_080d5658
.L_080d5494:
	mov r1, r11
	lsls r5, r1, #16
	mov r3, r9
	lsls r2, r3, #16
	adds r1, r5, #0
	bl Func_080201c0
	mov r2, r8
	mov r1, r10
	subs r2, #8
	adds r6, r0, #0
	lsls r2, r2, #16
	ldrb r0, [r1]
	adds r1, r5, #0
	bl Func_080201c0
	cmp r6, r0
	blt .L_080d54ba
	b .L_080d5658
.L_080d54ba:
	ldr r3, [r7, #8]
	add r0, sp, #12
	str r3, [r0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	str r3, [r0, #8]
	bl Func_08020258
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080d54d4
	b .L_080d5658
.L_080d54d4:
	adds r6, r7, #0
	adds r6, #90
	ldr r2, .L_080d5554
	strb r5, [r6]
	mov r1, r11
	ldr r0, [r2]
	mov r2, r9
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #6
	adds r0, r7, #0
	bl Object_SetMode
	movs r0, #4
	bl WaitFrames
	movs r1, #7
	adds r0, r7, #0
	bl Object_SetMode
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r7, #40]
	movs r0, #4
	bl WaitFrames
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	ldr r0, [sp, #4]
	movs r3, #254
	ands r0, r3
	str r0, [sp, #4]
	ldr r1, [sp, #4]
	adds r0, r7, #0
	bl ObjectDispatch_SetSingleChildField26Far
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r1, #12
	adds r0, r7, #0
	str r5, [r7, #40]
	bl Object_SetMode
	movs r0, #4
	bl WaitFrames
	ldr r1, .L_080d5558
	movs r3, #2
	strb r3, [r1]
	movs r3, #1
	strb r3, [r6]
	movs r0, #8
	bl WaitFrames
	ldr r3, [r7, #16]
	ldr r2, .L_080d5560
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r7, #16]
	b .L_080d5650
.L_080d5554:
	.4byte Data_02000454
.L_080d5558:
	.4byte Data_02000452
.L_080d555c:
	.4byte 0xfffffe00
.L_080d5560:
	.4byte 0xfff00000
.L_080d5564:
	ldrh r3, [r7, #32]
	add r4, sp, #12
	subs r3, #2
	mov r10, r3
	ldr r3, [r7, #8]
	movs r0, #128
	str r3, [r4]
	ldr r3, [r7, #12]
	lsls r0, r0, #13
	adds r3, r3, r0
	str r3, [r4, #4]
	ldr r3, [r7, #16]
	ldr r1, .L_080d5648
	movs r2, #0
	adds r3, r3, r1
	str r3, [r4, #8]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	mov r8, r2
	adds r6, r5, #0
	adds r6, #89
.L_080d5590:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_080d55bc
	ldrb r2, [r6]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080d55bc
	cmp r5, r7
	beq .L_080d55bc
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r2, r4, #0
	adds r0, #8
	subs r3, #2
	mov r1, r10
	str r4, [sp, #0]
	bl Func_08020348
	ldr r4, [sp, #0]
	cmp r0, #0
	bge .L_080d5658
.L_080d55bc:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #128
	adds r5, #128
	cmp r1, #63
	ble .L_080d5590
	movs r2, #0
	mov r8, r2
	adds r6, r7, #0
	adds r6, #85
	mov r3, r8
	strb r3, [r6]
	adds r0, r7, #0
	movs r1, #11
	bl Object_SetMode
	ldr r2, [r7, #12]
	mov r0, r11
	movs r3, #128
	lsls r1, r0, #16
	lsls r3, r3, #12
	mov r0, r9
	adds r2, r2, r3
	lsls r3, r0, #16
	ldr r0, .L_080d5648
	adds r3, r3, r0
	adds r0, r7, #0
	bl Object_SetPosition
	ldr r5, .L_080d564c
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r3, #3
	strb r3, [r6]
	ldr r3, [r7, #12]
	ldr r6, .L_080d5644
	str r3, [r7, #20]
	ldr r2, [sp, #4]
	adds r0, r7, #0
	orrs r2, r6
	adds r1, r2, #0
	str r2, [sp, #4]
	bl ObjectDispatch_SetSingleChildField26Far
	movs r0, #4
	bl Battle_WaitMode0
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	adds r5, r5, r3
	adds r3, r7, #0
	adds r3, #90
	strb r6, [r3]
	mov r1, r8
	adds r3, #10
	strh r1, [r3]
	mov r0, r8
	adds r3, #2
	mov r2, r8
	strb r0, [r5]
	strh r2, [r3]
	b .L_080d5650
.L_080d5644:
	.4byte 0x00000001
.L_080d5648:
	.4byte 0xfff00000
.L_080d564c:
	.4byte gPartyState
.L_080d5650:
	bl Func_080d2350
	movs r0, #0
	b .L_080d5660
.L_080d5658:
	bl Func_080d2350
	movs r0, #1
	negs r0, r0
.L_080d5660:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
