.syntax unified
	.thumb
	.global BattlePres_SetActorRecordMode
	.thumb_func
BattlePres_SetActorRecordMode:
	push {r5, r6, r7, lr}
	adds r5, r1, #0
	bl GetBattleObjectSlot
	cmp r0, #0
	beq .L_08126b60
	ldr r0, [r0]
	cmp r0, #0
	beq .L_08126b60
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	movs r2, #15
	ands r2, r3
	cmp r2, #1
	beq .L_08126b0a
	cmp r2, #2
	beq .L_08126b2e
	b .L_08126b60
.L_08126b0a:
	ldr r4, [r0, #80]
	movs r2, #13
	ldrb r1, [r4, #5]
	movs r3, #3
	negs r2, r2
	ands r5, r3
	adds r3, r2, #0
	lsls r0, r5, #2
	ands r3, r1
	orrs r3, r0
	strb r3, [r4, #5]
	adds r1, r4, #0
	adds r1, #33
	ldrb r3, [r1]
	ands r2, r3
	orrs r2, r0
	strb r2, [r1]
	b .L_08126b60
.L_08126b2e:
	movs r3, #3
	ands r5, r3
	ldr r6, [r0, #80]
	lsls r0, r5, #2
	movs r5, #13
	movs r7, #0
	negs r5, r5
.L_08126b3c:
	ldmia r6!, {r4}
	cmp r4, #0
	beq .L_08126b60
	ldrb r2, [r4, #5]
	adds r3, r5, #0
	ands r3, r2
	orrs r3, r0
	strb r3, [r4, #5]
	adds r1, r4, #0
	adds r1, #33
	ldrb r2, [r1]
	adds r3, r5, #0
	ands r3, r2
	orrs r3, r0
	adds r7, #1
	strb r3, [r1]
	cmp r7, #3
	ble .L_08126b3c
.L_08126b60:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
