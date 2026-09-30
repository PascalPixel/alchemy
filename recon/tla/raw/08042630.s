.syntax unified
	.thumb
	.global Func_08042630
	.thumb_func
Func_08042630:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #64]
	ldr r3, [r3, #60]
	adds r6, r0, #0
	ldrb r3, [r3, #5]
	movs r7, #4
	cmp r3, #0
	beq .L_0804264e
	movs r0, #0
	bl BattleParty_PrepareActiveOwnersFar
	movs r7, #3
	b .L_08042658
.L_0804264e:
	bl Func_080ad0f0
	cmp r0, #4
	bls .L_08042658
	movs r0, #4
.L_08042658:
	movs r3, #1
	ands r3, r6
	cmp r3, #0
	beq .L_08042664
	adds r7, #1
	b .L_0804266a
.L_08042664:
	movs r3, #3
	negs r3, r3
	ands r6, r3
.L_0804266a:
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r0, r3, #1
	movs r3, #2
	ands r3, r6
	adds r1, r0, #1
	cmp r3, #0
	beq .L_0804267c
	adds r1, r0, #6
.L_0804267c:
	movs r3, #30
	subs r3, r3, r1
	movs r2, #0
	strh r3, [r5, #4]
	strh r2, [r5, #6]
	strh r1, [r5, #8]
	strh r7, [r5, #10]
	strh r6, [r5, #12]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
