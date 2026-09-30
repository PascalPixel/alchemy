.syntax unified
	.thumb
	.global Battle_ApplyActionExtras
	.thumb_func
Battle_ApplyActionExtras:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	str r2, [sp, #4]
	movs r2, #0
	str r1, [sp, #8]
	str r2, [sp, #0]
	mov r9, r0
.L_081253aa:
	movs r3, #0
	movs r7, #0
	mov r11, r3
	mov r10, r3
.L_081253b2:
	add r2, sp, #36
	adds r3, r7, r2
	movs r4, #0
	adds r6, r3, #0
	adds r5, r3, #0
	mov r8, r4
	subs r6, #12
	subs r5, #24
.L_081253c2:
	mov r0, r9
	mov r1, r10
	mov r2, r8
	bl Djinn_IsActiveFar
	cmp r0, #0
	beq .L_081253e6
	mov r3, r10
	mov r4, r8
	movs r2, #1
	adds r7, #1
	strb r3, [r6]
	mov r11, r2
	strb r4, [r5]
	adds r6, #1
	adds r5, #1
	cmp r7, #10
	beq .L_08125442
.L_081253e6:
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #19
	ble .L_081253c2
	add r10, r3
	mov r2, r10
	cmp r2, #3
	ble .L_081253b2
	cmp r7, #0
	bne .L_08125448
	movs r3, #0
	mov r10, r3
.L_08125400:
	add r2, sp, #36
	adds r3, r7, r2
	movs r4, #0
	adds r6, r3, #0
	adds r5, r3, #0
	mov r8, r4
	subs r6, #12
	subs r5, #24
.L_08125410:
	mov r0, r9
	mov r1, r10
	mov r2, r8
	bl Trade_CanOfferDjinnFar
	cmp r0, #0
	beq .L_08125430
	mov r3, r10
	mov r4, r8
	adds r7, #1
	strb r3, [r6]
	strb r4, [r5]
	adds r6, #1
	adds r5, #1
	cmp r7, #10
	beq .L_08125442
.L_08125430:
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #19
	ble .L_08125410
	add r10, r2
	mov r4, r10
	cmp r4, #3
	ble .L_08125400
.L_08125442:
	cmp r7, #0
	bne .L_08125448
	b .L_08125600
.L_08125448:
	bl BattleRandom16Far
	adds r3, r7, #0
	muls r3, r0
	lsrs r7, r3, #16
	add r3, sp, #24
	ldrb r3, [r3, r7]
	mov r2, r11
	mov r10, r3
	add r3, sp, #12
	ldrb r3, [r3, r7]
	mov r8, r3
	cmp r2, #0
	beq .L_081254a6
	mov r1, r10
	mov r2, r8
	mov r0, r9
	bl Trade_AddOfferFar
	mov r0, r9
	mov r1, r10
	mov r2, r8
	bl Djinn_DeactivateFar
	ldr r3, [sp, #4]
	cmp r3, #0
	beq .L_081254ae
	movs r0, #0
	mov r1, r9
	bl BattleEv_Push
	mov r4, r10
	lsls r1, r4, #2
	add r1, r10
	lsls r1, r1, #2
	movs r2, #150
	lsls r2, r2, #1
	add r1, r8
	adds r1, r1, r2
	movs r0, #3
	bl BattleEv_Push
	ldr r1, .L_08125610
	movs r0, #4
	bl BattleEv_Push
	b .L_08125600
.L_081254a6:
	ldr r3, [sp, #4]
	cmp r3, #0
	beq .L_081254ae
	b .L_08125600
.L_081254ae:
	mov r4, r9
	movs r0, #0
	cmp r4, #7
	bls .L_081254b8
	movs r0, #1
.L_081254b8:
	bl Resource_FarCall005
	movs r4, #148
	adds r3, r0, #0
	lsls r4, r4, #1
	adds r0, r3, r4
	adds r2, r3, #0
	ldr r3, [r0]
	movs r1, #0
	adds r2, #8
	cmp r1, r3
	bge .L_081254f6
	movs r5, #1
	negs r5, r5
	movs r4, #254
.L_081254d6:
	ldrb r3, [r2]
	cmp r3, r10
	bne .L_081254ec
	ldrb r3, [r2, #1]
	cmp r3, r8
	bne .L_081254ec
	movs r3, #3
	ldrsb r3, [r2, r3]
	cmp r3, r5
	bne .L_081254ec
	strb r4, [r2, #3]
.L_081254ec:
	ldr r3, [r0]
	adds r1, #1
	adds r2, #4
	cmp r1, r3
	blt .L_081254d6
.L_081254f6:
	mov r2, r9
	movs r0, #0
	cmp r2, #7
	bls .L_08125500
	movs r0, #1
.L_08125500:
	bl Resource_FarCall005
	movs r4, #148
	adds r3, r0, #0
	lsls r4, r4, #1
	movs r2, #2
	adds r5, r3, #0
	adds r0, r3, r4
	negs r2, r2
	adds r5, #8
	mov lr, r2
	mov r11, r0
.L_08125518:
	ldr r3, [r0]
	movs r7, #1
	movs r1, #0
	negs r7, r7
	cmp r1, r3
	bge .L_08125546
	movs r3, #3
	ldrsb r3, [r5, r3]
	cmp r3, lr
	bne .L_08125530
	ldrb r7, [r5, #2]
	b .L_08125546
.L_08125530:
	ldr r3, [r0]
	adds r1, #1
	cmp r1, r3
	bge .L_08125546
	lsls r3, r1, #2
	adds r2, r5, r3
	movs r3, #3
	ldrsb r3, [r2, r3]
	cmp r3, lr
	bne .L_08125530
	ldrb r7, [r2, #2]
.L_08125546:
	movs r3, #1
	negs r3, r3
	cmp r7, r3
	beq .L_081255b0
	adds r4, r3, #0
	ldr r3, [r0]
	cmp r3, #0
	ble .L_08125574
	mov r3, r11
	ldr r1, [r3]
	adds r2, r5, #0
.L_0812555c:
	ldrb r3, [r2, #2]
	cmp r3, r7
	bne .L_0812556c
	movs r3, #3
	ldrsb r3, [r2, r3]
	cmp r3, r4
	ble .L_0812556c
	adds r4, r3, #0
.L_0812556c:
	subs r1, #1
	adds r2, #4
	cmp r1, #0
	bne .L_0812555c
.L_08125574:
	adds r4, #1
	cmp r4, #1
	bgt .L_0812557c
	movs r4, #2
.L_0812557c:
	ldr r3, [r0]
	movs r1, #0
	cmp r1, r3
	bge .L_08125518
	movs r2, #2
	movs r3, #144
	negs r2, r2
	lsls r3, r3, #1
	mov r12, r2
	adds r6, r5, r3
	adds r2, r5, #0
.L_08125592:
	ldrb r3, [r2, #2]
	cmp r3, r7
	bne .L_081255a4
	movs r3, #3
	ldrsb r3, [r2, r3]
	cmp r3, r12
	bne .L_081255a4
	strb r4, [r2, #3]
	adds r4, #1
.L_081255a4:
	ldr r3, [r6]
	adds r1, #1
	adds r2, #4
	cmp r1, r3
	blt .L_08125592
	b .L_08125518
.L_081255b0:
	ldr r4, [sp, #0]
	cmp r4, #0
	bne .L_081255f8
	movs r2, #1
	movs r0, #0
	mov r1, r9
	str r2, [sp, #0]
	bl BattleEv_Push
	ldr r3, [sp, #8]
	cmp r3, #0
	bne .L_081255f0
	mov r4, r10
	lsls r1, r4, #2
	add r1, r10
	lsls r1, r1, #2
	movs r2, #150
	lsls r2, r2, #1
	add r1, r8
	adds r1, r1, r2
	movs r0, #3
	bl BattleEv_Push
	mov r3, r9
	ldr r1, .L_08125614
	cmp r3, #7
	bls .L_081255e8
	adds r1, #1
.L_081255e8:
	movs r0, #4
	bl BattleEv_Push
	b .L_081255f8
.L_081255f0:
	ldr r1, .L_08125618
	movs r0, #4
	bl BattleEv_Push
.L_081255f8:
	ldr r4, [sp, #8]
	cmp r4, #0
	beq .L_08125600
	b .L_081253aa
.L_08125600:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08125610:
	.4byte 0x00000c97
.L_08125614:
	.4byte 0x00000c94
.L_08125618:
	.4byte 0x00000c96
