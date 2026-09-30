.syntax unified
	.thumb
	.global BattleParty_InsertUnitCentered
	.thumb_func
BattleParty_InsertUnitCentered:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	movs r0, #1
	movs r3, #0
	movs r4, #0
	sub sp, #8
	negs r0, r0
	mov r12, r1
	str r3, [sp, #0]
	str r4, [sp, #4]
	movs r6, #0
	adds r1, r0, #0
	movs r4, #0
	adds r5, r7, #0
.L_081203e4:
	ldrh r3, [r5]
	adds r5, #2
	cmp r3, #254
	bne .L_08120412
	cmp r1, #0
	blt .L_0812040e
	cmp r0, #0
	blt .L_0812040e
	subs r2, r4, r1
	cmp r2, #0
	bge .L_081203fc
	subs r2, r1, r4
.L_081203fc:
	subs r3, r0, r1
	cmp r3, #0
	blt .L_08120408
	cmp r2, r3
	blt .L_0812040e
	b .L_0812042c
.L_08120408:
	subs r3, r1, r0
	cmp r2, r3
	bge .L_0812042c
.L_0812040e:
	adds r0, r4, #0
	b .L_0812042c
.L_08120412:
	cmp r3, #255
	beq .L_08120432
	cmp r3, #128
	bne .L_08120420
	movs r3, #1
	eors r6, r3
	adds r1, r4, #0
.L_08120420:
	mov r3, sp
	lsls r2, r6, #2
	adds r2, r2, r3
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0812042c:
	adds r4, #1
	cmp r4, #5
	ble .L_081203e4
.L_08120432:
	cmp r0, #0
	blt .L_0812043e
	lsls r3, r0, #1
	mov r2, r12
	strh r2, [r3, r7]
	b .L_0812044a
.L_0812043e:
	lsls r3, r4, #1
	adds r3, r3, r7
	mov r2, r12
	strh r2, [r3]
	ldr r2, .L_08120450
	strh r2, [r3, #2]
.L_0812044a:
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08120450:
	.4byte 0x000000ff
