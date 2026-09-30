.syntax unified
	.thumb
	.global Func_08123574
	.thumb_func
Func_08123574:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r6, r1, #0
	movs r1, #0
	sub sp, #20
	mov r11, r1
	movs r0, #1
	cmp r5, #7
	bls .L_08123594
	movs r0, #2
.L_08123594:
	add r2, sp, #4
	mov r9, r2
	mov r1, r9
	bl BattleParty_ListActorIds
	adds r7, r0, #0
	movs r0, #0
	cmp r5, #7
	bls .L_081235a8
	movs r0, #1
.L_081235a8:
	bl Resource_FarCall005
	adds r0, #8
	mov r8, r0
	cmp r6, #0
	beq .L_081235c2
	movs r2, #0
	adds r3, r6, #3
	mov r12, r6
.L_081235ba:
	strb r2, [r3]
	subs r3, #1
	cmp r3, r12
	bge .L_081235ba
.L_081235c2:
	movs r2, #144
	lsls r2, r2, #1
	movs r3, #0
	add r2, r8
	mov r10, r3
	ldr r3, [r2]
	cmp r3, #0
	beq .L_08123636
	str r2, [sp, #0]
	movs r1, #0
	mov r12, r9
	mov r5, r8
	mov lr, r1
.L_081235dc:
	movs r3, #3
	ldrsb r3, [r5, r3]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_08123624
	movs r4, #0
	cmp r4, r7
	bge .L_0812360c
	mov r3, r12
	ldrh r2, [r3]
	ldrb r3, [r5, #2]
	cmp r2, r3
	beq .L_0812360c
	adds r1, r5, #0
	mov r0, r9
.L_081235fc:
	adds r4, #1
	cmp r4, r7
	bge .L_0812360c
	adds r0, #2
	ldrh r2, [r0]
	ldrb r3, [r1, #2]
	cmp r2, r3
	bne .L_081235fc
.L_0812360c:
	cmp r4, r7
	beq .L_08123624
	cmp r6, #0
	beq .L_08123620
	mov r3, r8
	mov r1, lr
	ldrb r2, [r1, r3]
	ldrb r3, [r6, r2]
	adds r3, #1
	strb r3, [r6, r2]
.L_08123620:
	movs r1, #1
	add r11, r1
.L_08123624:
	ldr r1, [sp, #0]
	movs r3, #1
	add r10, r3
	ldr r3, [r1]
	movs r2, #4
	adds r5, #4
	add lr, r2
	cmp r10, r3
	bne .L_081235dc
.L_08123636:
	mov r0, r11
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
