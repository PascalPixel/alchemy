.syntax unified
	.thumb
	.global Func_080d50f8
	.thumb_func
Func_080d50f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_080d5338
	movs r3, #133
	mov r11, r0
	lsls r3, r3, #2
	add r3, r11
	ldr r0, [r3]
	sub sp, #32
	bl Object_GetById
	movs r1, #192
	lsls r1, r1, #18
	ldr r1, [r1, #32]
	movs r2, #1
	str r2, [sp, #8]
	str r1, [sp, #16]
	adds r7, r0, #0
	movs r3, #10
	ldrsh r5, [r7, r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #240
	ands r5, r3
	adds r0, r5, #0
	adds r0, #8
	str r0, [sp, #12]
	movs r2, #8
	movs r1, #14
	ldrsh r6, [r7, r1]
	movs r1, #18
	ldrsh r0, [r7, r1]
	ands r6, r3
	ands r0, r3
	adds r2, r2, r6
	mov r8, r0
	mov r10, r2
	movs r2, #8
	add r2, r8
	mov r9, r2
	bl Func_080d22a8
	adds r3, r7, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080d5166
	ldr r3, [r7, #80]
	ldrb r3, [r3, #26]
	str r3, [sp, #8]
.L_080d5166:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r11, r3
	mov r0, r11
	ldrb r3, [r0]
	cmp r3, #0
	beq .L_080d5178
	b .L_080d5344
.L_080d5178:
	ldr r2, [sp, #12]
	cmp r2, #0
	bge .L_080d5182
	adds r2, r5, #0
	adds r2, #23
.L_080d5182:
	asrs r2, r2, #4
	mov r3, r10
	mov r12, r2
	cmp r3, #0
	bge .L_080d5190
	adds r3, r6, #0
	adds r3, #23
.L_080d5190:
	asrs r3, r3, #4
	mov r2, r9
	mov lr, r3
	cmp r2, #0
	bge .L_080d519e
	mov r2, r8
	adds r2, #23
.L_080d519e:
	adds r6, r7, #0
	adds r6, #34
	ldrb r0, [r6]
	movs r1, #156
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r1, [sp, #16]
	asrs r5, r2, #4
	ldr r3, [r1, r3]
	lsls r2, r5, #7
	add r2, r12
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #2
	adds r1, r3, r2
	ldrb r2, [r3, #3]
	movs r4, #64
	adds r3, r4, #0
	ands r3, r2
	cmp r3, #0
	bne .L_080d51d2
	b .L_080d53a0
.L_080d51d2:
	ldrb r2, [r1, #3]
	adds r3, r4, #0
	ands r3, r2
	cmp r3, #0
	bne .L_080d51de
	b .L_080d53a0
.L_080d51de:
	ldr r3, [sp, #16]
	movs r1, #212
	lsls r1, r1, #1
	adds r2, r3, r1
	mov r1, lr
	subs r3, r5, r1
	lsls r3, r3, #7
	ldr r2, [r2]
	add r3, r12
	lsls r3, r3, #2
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #2
	adds r1, r2, r3
	ldrb r3, [r1, #2]
	cmp r3, #255
	bne .L_080d5202
	b .L_080d53a0
.L_080d5202:
	ldr r1, [sp, #12]
	mov r2, r9
	lsls r1, r1, #16
	lsls r2, r2, #16
	mov r11, r1
	str r2, [sp, #4]
	bl Func_080201c0
	mov r2, r8
	adds r2, #24
	adds r5, r0, #0
	lsls r2, r2, #16
	ldrb r0, [r6]
	mov r1, r11
	bl Func_080201c0
	cmp r5, r0
	bgt .L_080d5228
	b .L_080d53a0
.L_080d5228:
	ldr r3, [r7, #8]
	add r6, sp, #20
	str r3, [r6]
	ldr r3, [r7, #12]
	ldr r0, .L_080d533c
	adds r3, r3, r0
	str r3, [r6, #4]
	ldr r3, [r7, #16]
	adds r0, r6, #0
	str r3, [r6, #8]
	bl Func_08020258
	cmp r0, #0
	beq .L_080d5246
	b .L_080d53a0
.L_080d5246:
	ldrh r3, [r7, #32]
	ldr r1, .L_080d5340
	subs r3, #2
	mov r10, r3
	ldr r3, [r7, #8]
	movs r2, #128
	str r3, [r6]
	ldr r3, [r7, #12]
	lsls r2, r2, #13
	adds r3, r3, r1
	str r3, [r6, #4]
	ldr r3, [r7, #16]
	movs r0, #89
	adds r3, r3, r2
	str r3, [r6, #8]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r4, #0
	adds r0, r0, r5
	mov r8, r0
.L_080d5270:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_080d52a0
	mov r1, r8
	ldrb r2, [r1]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080d52a0
	cmp r5, r7
	beq .L_080d52a0
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	mov r1, r10
	adds r2, r6, #0
	str r4, [sp, #0]
	bl Func_08020348
	ldr r4, [sp, #0]
	cmp r0, #0
	blt .L_080d52a0
	b .L_080d53a0
.L_080d52a0:
	movs r2, #128
	adds r4, #1
	add r8, r2
	adds r5, #128
	cmp r4, #63
	ble .L_080d5270
	ldr r6, .L_080d5338
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r6, r3
	ldr r1, [sp, #12]
	ldr r0, [r5]
	mov r2, r9
	bl ObjectMotion_SetPositionAndCommit
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	ldr r0, [r5]
	bl Func_080d3838
	ldr r0, [r5]
	bl Object_RefreshSelectorById
	adds r1, r7, #0
	adds r1, #90
	movs r3, #1
	strb r3, [r1]
	adds r3, r7, #0
	movs r2, #0
	adds r3, #85
	strb r2, [r3]
	ldr r0, [sp, #8]
	movs r3, #254
	ands r0, r3
	str r0, [sp, #8]
	ldr r1, [sp, #8]
	adds r0, r7, #0
	bl ObjectDispatch_SetSingleChildField26Far
	adds r0, r7, #0
	movs r1, #13
	bl Object_SetMode
	ldr r1, .L_080d533c
	ldr r0, [sp, #4]
	ldr r2, [r7, #12]
	mov r8, r1
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r0, r1
	add r2, r8
	mov r1, r11
	adds r0, r7, #0
	bl Object_SetPosition
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	movs r3, #2
	adds r6, r6, r2
	strb r3, [r6]
	ldr r3, [r7, #16]
	mov r0, r8
	movs r1, #128
	ands r3, r0
	lsls r1, r1, #12
	adds r3, r3, r1
	str r3, [r7, #16]
	b .L_080d5398
.L_080d5338:
	.4byte gPartyState
.L_080d533c:
	.4byte 0xfff00000
.L_080d5340:
	.4byte 0xffe00000
.L_080d5344:
	adds r0, r7, #0
	movs r1, #10
	bl Object_SetMode
	adds r2, r7, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r7, #40]
	ldr r3, [r7, #12]
	ldr r6, .L_080d5394
	str r3, [r7, #20]
	ldr r2, [sp, #8]
	adds r0, r7, #0
	orrs r2, r6
	adds r1, r2, #0
	str r2, [sp, #8]
	bl ObjectDispatch_SetSingleChildField26Far
	movs r0, #6
	bl Battle_WaitMode0
	movs r5, #0
	mov r3, r11
	strb r5, [r3]
	adds r3, r7, #0
	adds r3, #90
	strb r6, [r3]
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r7, #6]
	adds r3, r7, #0
	adds r3, #100
	strh r5, [r3]
	adds r3, #2
	strh r5, [r3]
	b .L_080d5398
	.2byte 0x0000
.L_080d5394:
	.4byte 0x00000001
.L_080d5398:
	bl Func_080d2350
	movs r0, #0
	b .L_080d53a8
.L_080d53a0:
	bl Func_080d2350
	movs r0, #1
	negs r0, r0
.L_080d53a8:
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
