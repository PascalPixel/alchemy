.syntax unified
	.thumb
	.global Func_080dc410
	.thumb_func
Func_080dc410:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	mov r11, r1
	ldr r3, [r3, #32]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #182
	mov r8, r3
	adds r3, r7, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r10, r0
	cmp r3, #0
	bne .L_080dc460
	movs r5, #224
	lsls r5, r5, #3
	adds r5, #244
	adds r1, r5, #0
	movs r0, #224
	bl Runtime_AllocateHeapBlock
	adds r6, r0, #0
	ldr r3, .L_080dc45c
	movs r2, #0
	adds r1, r5, #0
	mov lr, r3
	.2byte 0xf800
	b .L_080dc468
	.2byte 0x0000
.L_080dc45c:
	.4byte IwramFillWords
.L_080dc460:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r6, [r3]
.L_080dc468:
	ldr r2, .L_080dc4a0
	mov r3, r10
	strh r3, [r6, #28]
	mov r0, r10
	mov r9, r2
	bl BattleAction_Get
	movs r1, #192
	ldrb r3, [r0, #6]
	lsls r1, r1, #4
	adds r1, #182
	strh r3, [r6, #30]
	adds r3, r7, r1
	movs r5, #0
	ldrsb r5, [r3, r5]
	cmp r5, #0
	beq .L_080dc48c
	b .L_080dc5fc
.L_080dc48c:
	bl Func_080dc0b8
	ldr r3, .L_080dc4a4
	adds r2, r6, #0
	subs r3, r3, r0
	adds r2, #66
	strh r3, [r2]
	adds r3, r6, #0
	adds r3, #33
	b .L_080dc4a8
.L_080dc4a0:
	.4byte 0x00000000
.L_080dc4a4:
	.4byte 0x00000200
.L_080dc4a8:
	mov r2, r11
	strb r2, [r3]
	movs r4, #1
	adds r3, #1
	movs r1, #224
	strb r4, [r3]
	lsls r1, r1, #3
	subs r3, #2
	movs r2, #224
	strb r4, [r3]
	adds r1, #18
	adds r3, #3
	lsls r2, r2, #3
	strb r4, [r3]
	adds r2, #19
	adds r3, r6, r1
	strb r4, [r3]
	mov r1, r9
	adds r3, r6, r2
	adds r2, #177
	strb r1, [r3]
	adds r3, r6, r2
	strh r5, [r3]
	movs r3, #224
	lsls r3, r3, #3
	adds r3, #198
	adds r2, r6, r3
	movs r1, #224
	movs r3, #255
	lsls r3, r3, #8
	lsls r1, r1, #3
	adds r3, #255
	adds r1, #242
	strh r3, [r2]
	adds r3, r6, r1
	strh r5, [r3]
	mov r2, r8
	ldr r3, [r2, #4]
	movs r0, #253
	str r3, [r6, #68]
	lsls r0, r0, #1
	ldr r3, [r2, #8]
	str r3, [r6, #72]
	ldr r3, [r2, #12]
	str r3, [r6, #76]
	ldr r3, .L_080dc608
	mov r12, r3
	add r0, r12
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, .L_080dc60c
	ldrh r1, [r0]
	cmp r2, r3
	bne .L_080dc51a
	adds r3, r6, #0
	adds r3, #65
	strb r4, [r3]
.L_080dc51a:
	ldr r2, .L_080dc610
	lsls r3, r1, #16
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_080dc52c
	adds r3, r6, #0
	adds r3, #65
	strb r4, [r3]
	ldrh r1, [r0]
.L_080dc52c:
	ldr r2, .L_080dc614
	lsls r3, r1, #16
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_080dc53e
	adds r3, r6, #0
	adds r3, #65
	strb r4, [r3]
	ldrh r1, [r0]
.L_080dc53e:
	ldr r2, .L_080dc618
	lsls r3, r1, #16
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_080dc550
	adds r3, r6, #0
	adds r3, #65
	strb r4, [r3]
	ldrh r1, [r0]
.L_080dc550:
	ldr r2, .L_080dc61c
	lsls r3, r1, #16
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_080dc562
	adds r3, r6, #0
	adds r3, #65
	strb r4, [r3]
	ldrh r1, [r0]
.L_080dc562:
	ldr r2, .L_080dc620
	lsls r3, r1, #16
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_080dc574
	adds r3, r6, #0
	adds r3, #65
	strb r4, [r3]
	ldrh r1, [r0]
.L_080dc574:
	ldr r2, .L_080dc624
	lsls r3, r1, #16
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_080dc584
	adds r3, r6, #0
	adds r3, #65
	strb r4, [r3]
.L_080dc584:
	movs r3, #133
	lsls r3, r3, #2
	add r3, r12
	movs r1, #1
	ldr r0, [r3]
	negs r1, r1
	bl Func_080dc62c
	movs r1, #30
	ldrsh r3, [r6, r1]
	ldrh r2, [r6, #30]
	cmp r3, #8
	beq .L_080dc5aa
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #165
	adds r3, r7, r1
	mov r1, r9
	strb r1, [r3]
.L_080dc5aa:
	lsls r3, r2, #16
	movs r2, #224
	lsls r2, r2, #13
	cmp r3, r2
	beq .L_080dc5c0
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #173
	adds r3, r7, r1
	mov r2, r9
	strb r2, [r3]
.L_080dc5c0:
	movs r1, #208
	lsls r1, r1, #4
	adds r1, #49
	adds r3, r7, r1
	mov r2, r9
	adds r1, #1
	strb r2, [r3]
	adds r3, r7, r1
	strb r2, [r3]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #51
	mov r1, r9
	adds r3, r7, r2
	adds r2, #5
	strb r1, [r3]
	adds r3, r7, r2
	strb r1, [r3]
	movs r3, #211
	lsls r3, r3, #4
	adds r2, r7, r3
	movs r3, #255
	strb r3, [r2]
	bl Func_080e15fc
	movs r1, #144
	ldr r0, .L_080dc628
	lsls r1, r1, #3
	bl Func_080145a8
.L_080dc5fc:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080dc608:
	.4byte gPartyState
.L_080dc60c:
	.4byte 0x00000082
.L_080dc610:
	.4byte 0x000000ce
.L_080dc614:
	.4byte 0x000000cf
.L_080dc618:
	.4byte 0x000000d4
.L_080dc61c:
	.4byte 0x00000108
.L_080dc620:
	.4byte 0x00000109
.L_080dc624:
	.4byte 0x0000010a
.L_080dc628:
	.4byte Func_080dc3f0
