.syntax unified
	.thumb
	.global Func_080fb410
	.thumb_func
Func_080fb410:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	movs r1, #182
	lsls r1, r1, #1
	adds r3, r7, r1
	ldrh r3, [r3]
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r3
	bl Item_Get
	ldrb r3, [r0, #2]
	mov r8, r0
	cmp r3, #0
	bne .L_080fb448
	movs r2, #1
	movs r3, #1
	negs r2, r2
	strb r3, [r5]
	adds r3, r2, #0
	b .L_080fb452
.L_080fb448:
	movs r1, #1
	negs r1, r1
	adds r3, r1, #0
	strb r3, [r5]
	movs r3, #1
.L_080fb452:
	strb r3, [r5, #1]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #22
	movs r1, #182
	adds r3, r7, r2
	lsls r1, r1, #1
	ldrb r0, [r3]
	adds r3, r7, r1
	ldrh r1, [r3]
	bl Func_080fb638
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_080fb478
	movs r3, #1
	strb r3, [r5]
	b .L_080fb47a
.L_080fb478:
	strb r0, [r5]
.L_080fb47a:
	movs r3, #182
	lsls r3, r3, #1
	adds r6, r7, r3
	ldrh r2, [r6]
	movs r3, #128
	lsls r3, r3, #3
	ands r3, r2
	cmp r3, #0
	beq .L_080fb496
	movs r1, #1
	negs r1, r1
	adds r3, r1, #0
	strb r3, [r5]
	ldrh r2, [r6]
.L_080fb496:
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #22
	adds r3, r7, r1
	subs r1, #23
	ldrb r0, [r3]
	ands r1, r2
	bl Djinn_IsActiveFar + 0x10
	cmp r0, #0
	bne .L_080fb4b4
	movs r2, #1
	negs r2, r2
	adds r3, r2, #0
	strb r3, [r5, #1]
.L_080fb4b4:
	movs r1, #1
	strb r1, [r5, #3]
	strb r1, [r5, #5]
	strb r1, [r5, #2]
	movs r3, #128
	ldrh r2, [r6]
	lsls r3, r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080fb4d4
	strb r1, [r5, #4]
	movs r1, #1
	negs r1, r1
	adds r3, r1, #0
	strb r3, [r5, #1]
	b .L_080fb4dc
.L_080fb4d4:
	movs r2, #1
	negs r2, r2
	adds r3, r2, #0
	strb r3, [r5, #4]
.L_080fb4dc:
	mov r3, r8
	ldrb r2, [r3, #3]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080fb506
	movs r2, #1
	negs r2, r2
	adds r1, r2, #0
	strb r1, [r5, #4]
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r7, r2
	ldrh r2, [r3]
	movs r3, #128
	lsls r3, r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080fb506
	strb r1, [r5, #3]
	strb r1, [r5, #5]
.L_080fb506:
	movs r1, #182
	lsls r1, r1, #1
	adds r3, r7, r1
	ldrh r3, [r3]
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r3
	bl Func_080c8508 + 0x8
	cmp r0, #0
	beq .L_080fb522
	movs r3, #1
	strb r3, [r5]
.L_080fb522:
	movs r2, #139
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r7, r2
	ldrb r3, [r3]
	cmp r3, #1
	bhi .L_080fb538
	movs r1, #1
	negs r1, r1
	adds r3, r1, #0
	strb r3, [r5, #3]
.L_080fb538:
	mov r3, r8
	ldrb r2, [r3, #3]
	movs r3, #8
	ands r3, r2
	cmp r3, #0
	beq .L_080fb54c
	movs r1, #1
	negs r1, r1
	adds r3, r1, #0
	strb r3, [r5, #5]
.L_080fb54c:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
