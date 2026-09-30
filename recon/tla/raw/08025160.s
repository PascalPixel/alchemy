.syntax unified
	.thumb
	.global Func_08025160
	.thumb_func
Func_08025160:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	str r0, [sp, #4]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #240
	mov r12, r3
	adds r3, #236
	ldr r3, [r3]
	ldr r6, [r0, #104]
	lsls r1, r1, #15
	adds r0, r3, r1
	mov r3, r12
	adds r3, #240
	ldr r2, [r6, #12]
	ldr r3, [r3]
	movs r4, #192
	adds r3, r3, r2
	lsls r4, r4, #15
	adds r1, r3, r4
	mov r3, r12
	adds r3, #244
	ldr r3, [r3]
	ldr r5, .L_08025414
	ldr r7, .L_08025418
	adds r4, r3, r5
	mov r3, r12
	adds r3, #248
	ldr r3, [r3]
	adds r3, r3, r2
	ldr r2, [sp, #4]
	adds r5, r3, r7
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	cmp r6, #0
	bne .L_080251ba
	b .L_080253fa
.L_080251ba:
	ldr r3, [r6]
	cmp r3, #0
	bne .L_080251c2
	b .L_080253fa
.L_080251c2:
	ldr r2, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #16]
	mov r10, r2
	ldr r2, [sp, #4]
	mov r11, r3
	movs r3, #128
	lsls r3, r3, #24
	mov r9, r7
	str r3, [r2, #56]
	str r3, [r2, #60]
	str r3, [r2, #64]
	cmp r10, r0
	bge .L_080251e0
	mov r10, r0
.L_080251e0:
	cmp r9, r1
	bge .L_080251e6
	mov r9, r1
.L_080251e6:
	cmp r10, r4
	ble .L_080251ec
	mov r10, r4
.L_080251ec:
	cmp r9, r5
	ble .L_080251f2
	mov r9, r5
.L_080251f2:
	mov r3, r12
	adds r3, #252
	ldr r3, [r3]
	cmp r3, #0
	beq .L_080252d8
	adds r2, r3, #0
	movs r4, #0
	ldrsh r3, [r2, r4]
	movs r0, #1
	negs r0, r0
	ldrh r1, [r2]
	cmp r3, r0
	beq .L_080252d8
	ldr r6, [r6, #12]
	mov r12, r0
.L_08025210:
	ldr r5, .L_08025414
	lsls r3, r1, #16
	adds r0, r3, r5
	movs r7, #2
	ldrsh r3, [r2, r7]
	ldr r4, .L_08025418
	lsls r3, r3, #16
	adds r3, r3, r6
	adds r1, r3, r4
	movs r5, #4
	ldrsh r3, [r2, r5]
	movs r7, #240
	lsls r3, r3, #16
	lsls r7, r7, #15
	adds r4, r3, r7
	movs r5, #6
	ldrsh r3, [r2, r5]
	movs r7, #192
	lsls r3, r3, #16
	adds r3, r3, r6
	lsls r7, r7, #15
	adds r5, r3, r7
	cmp r10, r0
	blt .L_080252cc
	cmp r9, r1
	blt .L_080252cc
	cmp r10, r4
	bgt .L_080252cc
	cmp r9, r5
	bgt .L_080252cc
	mov r2, r10
	mov r8, r0
	subs r0, r0, r2
	adds r2, r0, #0
	cmp r0, #0
	bge .L_0802525e
	mov r3, r10
	mov r6, r8
	subs r2, r3, r6
.L_0802525e:
	mov r7, r10
	subs r3, r4, r7
	cmp r3, #0
	blt .L_0802526c
	cmp r2, r3
	bgt .L_08025274
	b .L_0802527a
.L_0802526c:
	mov r6, r10
	subs r3, r6, r4
	cmp r2, r3
	ble .L_0802527a
.L_08025274:
	mov r7, r10
	mov r8, r4
	subs r0, r4, r7
.L_0802527a:
	mov r2, r9
	adds r6, r1, #0
	subs r1, r6, r2
	adds r2, r1, #0
	cmp r1, #0
	bge .L_0802528a
	mov r3, r9
	subs r2, r3, r6
.L_0802528a:
	mov r4, r9
	subs r3, r5, r4
	cmp r3, #0
	blt .L_08025298
	cmp r2, r3
	bgt .L_080252a0
	b .L_080252a6
.L_08025298:
	mov r7, r9
	subs r3, r7, r5
	cmp r2, r3
	ble .L_080252a6
.L_080252a0:
	adds r6, r5, #0
	mov r2, r9
	subs r1, r6, r2
.L_080252a6:
	adds r2, r0, #0
	cmp r2, #0
	bge .L_080252b2
	mov r3, r10
	mov r4, r8
	subs r2, r3, r4
.L_080252b2:
	cmp r1, #0
	blt .L_080252bc
	cmp r2, r1
	ble .L_080252c4
	b .L_080252c8
.L_080252bc:
	mov r5, r9
	subs r3, r5, r6
	cmp r2, r3
	bgt .L_080252c8
.L_080252c4:
	mov r10, r8
	b .L_080252d8
.L_080252c8:
	mov r9, r6
	b .L_080252d8
.L_080252cc:
	adds r2, #8
	movs r7, #0
	ldrsh r3, [r2, r7]
	ldrh r1, [r2]
	cmp r3, r12
	bne .L_08025210
.L_080252d8:
	ldr r3, [sp, #4]
	adds r3, #100
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_080252f4
	ldr r2, [sp, #4]
	mov r1, r10
	mov r3, r11
	mov r4, r9
	str r1, [r2, #8]
	str r3, [r2, #12]
	str r4, [r2, #16]
	b .L_080253fa
.L_080252f4:
	ldr r5, [sp, #4]
	mov r6, r10
	ldr r3, [r5, #8]
	subs r0, r6, r3
	cmp r0, #0
	bge .L_08025308
	movs r7, #255
	lsls r7, r7, #8
	adds r7, #255
	adds r0, r0, r7
.L_08025308:
	ldr r2, [sp, #4]
	asrs r0, r0, #16
	ldr r3, [r2, #16]
	mov r4, r9
	mov r8, r0
	subs r0, r4, r3
	cmp r0, #0
	bge .L_08025320
	movs r5, #255
	lsls r5, r5, #8
	adds r5, #255
	adds r0, r0, r5
.L_08025320:
	asrs r6, r0, #16
	mov r7, r8
	mov r0, r8
	muls r0, r7
	adds r3, r6, #0
	muls r3, r6
	adds r0, r0, r3
	ldr r3, .L_0802541c
	mov lr, r3
	.2byte 0xf800
	lsls r7, r0, #16
	ldr r0, [sp, #4]
	mov r1, r10
	ldr r3, [r0, #8]
	mov r2, r11
	subs r1, r1, r3
	ldr r3, [r0, #12]
	movs r5, #128
	subs r2, r2, r3
	str r2, [sp, #0]
	mov r4, r9
	ldr r3, [r0, #16]
	lsls r5, r5, #15
	mov r8, r1
	subs r6, r4, r3
	cmp r7, r5
	bge .L_08025372
	ldr r7, .L_08025420
	mov r0, r8
	mov lr, r7
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r7
	.2byte 0xf800
	adds r5, r5, r0
	adds r0, r5, #0
	bl Func_080149e0
	adds r7, r0, #0
.L_08025372:
	adds r1, r7, #0
	cmp r7, #0
	bge .L_0802537a
	adds r1, r7, #7
.L_0802537a:
	ldr r0, [sp, #4]
	asrs r1, r1, #3
	ldr r3, [r0, #48]
	mov r11, r1
	cmp r11, r3
	ble .L_08025388
	mov r11, r3
.L_08025388:
	movs r1, #128
	lsls r1, r1, #7
	cmp r7, r1
	bge .L_0802539c
	ldr r3, [sp, #4]
	mov r2, r10
	mov r4, r9
	str r2, [r3, #8]
	str r4, [r3, #16]
	b .L_080253d4
.L_0802539c:
	cmp r7, r11
	ble .L_080253c6
	ldr r5, .L_08025424
	mov r1, r8
	mov r10, r5
	adds r0, r7, #0
	mov lr, r10
	.2byte 0xf800
	ldr r5, .L_08025420
	mov r1, r11
	mov lr, r5
	.2byte 0xf800
	adds r1, r6, #0
	mov r8, r0
	adds r0, r7, #0
	mov lr, r10
	.2byte 0xf800
	mov r1, r11
	mov lr, r5
	.2byte 0xf800
	adds r6, r0, #0
.L_080253c6:
	ldr r7, [sp, #4]
	ldr r3, [r7, #8]
	add r3, r8
	str r3, [r7, #8]
	ldr r3, [r7, #16]
	adds r3, r3, r6
	str r3, [r7, #16]
.L_080253d4:
	ldr r3, [sp, #0]
	cmp r3, #0
	bge .L_080253dc
	negs r3, r3
.L_080253dc:
	movs r0, #128
	lsls r0, r0, #8
	cmp r3, r0
	ble .L_080253f0
	ldr r3, [sp, #0]
	cmp r3, #0
	bge .L_080253ec
	adds r3, #3
.L_080253ec:
	asrs r3, r3, #2
	str r3, [sp, #0]
.L_080253f0:
	ldr r1, [sp, #4]
	ldr r2, [sp, #0]
	ldr r3, [r1, #12]
	adds r3, r3, r2
	str r3, [r1, #12]
.L_080253fa:
	ldr r4, [sp, #4]
	movs r0, #1
	ldrh r3, [r4, #4]
	adds r5, r4, #0
	adds r3, #1
	strh r3, [r5, #4]
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08025414:
	.4byte 0xff880000
.L_08025418:
	.4byte 0xffc00000
.L_0802541c:
	.4byte IwramFillWords + 0x74
.L_08025420:
	.4byte IwramMulQ16
.L_08025424:
	.4byte IwramRatioMulQ14
