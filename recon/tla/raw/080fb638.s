.syntax unified
	.thumb
	.global Func_080fb638
	.thumb_func
Func_080fb638:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #128
	lsls r3, r3, #1
	adds r6, r1, #0
	adds r3, #255
	ands r6, r3
	mov r10, r0
	adds r0, r6, #0
	bl Item_Get
	movs r7, #1
	adds r5, r0, #0
	adds r0, r6, #0
	negs r7, r7
	bl Func_080c8508 + 0x8
	cmp r0, #0
	beq .L_080fb666
	movs r0, #0
	b .L_080fb6ca
.L_080fb666:
	ldrh r3, [r5, #40]
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	ands r0, r3
	bl BattleAction_Get
	ldrh r3, [r5, #40]
	mov r8, r0
	cmp r3, #0
	beq .L_080fb6c8
	ldrb r3, [r5, #2]
	cmp r3, #0
	beq .L_080fb694
	ldrb r3, [r5, #12]
	cmp r3, #3
	beq .L_080fb696
	mov r0, r10
	adds r1, r6, #0
	bl Djinn_IsActiveFar + 0x10
	cmp r0, #0
	beq .L_080fb696
.L_080fb694:
	movs r7, #1
.L_080fb696:
	cmp r7, #1
	bne .L_080fb6c8
	mov r3, r8
	ldrb r2, [r3, #1]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	beq .L_080fb6b8
	mov r3, r8
	ldrb r2, [r3, #8]
	movs r3, #255
	eors r2, r3
	negs r3, r2
	orrs r3, r2
	lsrs r7, r3, #31
	movs r3, #2
	b .L_080fb6c6
.L_080fb6b8:
	movs r3, #128
	ands r3, r2
	negs r2, r3
	orrs r2, r3
	lsrs r2, r2, #31
	adds r7, r2, #0
	movs r3, #0
.L_080fb6c6:
	subs r7, r3, r7
.L_080fb6c8:
	adds r0, r7, #0
.L_080fb6ca:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
