.syntax unified
	.thumb
	.global Func_080ca3f4
	.thumb_func
Func_080ca3f4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #222
	adds r6, r1, #0
	ldr r5, .L_080ca488
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ca482
	ldr r1, .L_080ca48c
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r1, r2
	movs r2, #0
	strh r2, [r3]
	movs r2, #0
	ldrsh r3, [r5, r2]
	ldr r2, .L_080ca490
	asrs r0, r2, #16
	cmp r3, r0
	beq .L_080ca482
	movs r3, #158
	lsls r3, r3, #2
	adds r3, r3, r1
	adds r4, r2, #0
	movs r2, #159
	mov lr, r3
	lsls r2, r2, #2
	movs r3, #160
	adds r2, r2, r1
	lsls r3, r3, #2
	mov r8, r2
	adds r1, r1, r3
	mov r12, r0
.L_080ca442:
	movs r2, #4
	ldrsh r3, [r5, r2]
	cmp r3, r7
	bne .L_080ca478
	movs r2, #6
	ldrsh r3, [r5, r2]
	asrs r2, r4, #16
	cmp r3, r2
	beq .L_080ca45c
	cmp r6, r2
	beq .L_080ca45c
	cmp r3, r6
	bne .L_080ca478
.L_080ca45c:
	movs r2, #8
	ldrsh r3, [r5, r2]
	mov r2, lr
	lsls r3, r3, #16
	str r3, [r2]
	movs r2, #10
	ldrsh r3, [r5, r2]
	mov r2, r8
	lsls r3, r3, #16
	str r3, [r2]
	movs r2, #12
	ldrsh r3, [r5, r2]
	str r3, [r1]
	b .L_080ca482
.L_080ca478:
	adds r5, #16
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, r12
	bne .L_080ca442
.L_080ca482:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080ca488:
	.4byte Party_PairResolveRules
.L_080ca48c:
	.4byte gPartyState
.L_080ca490:
	.4byte 0xffff0000
