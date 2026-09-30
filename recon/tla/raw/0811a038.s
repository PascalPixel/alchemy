.syntax unified
	.thumb
	.global BattleParty_PrepareActiveOwners
	.thumb_func
BattleParty_PrepareActiveOwners:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	sub sp, #4
	adds r3, #68
	ldrb r3, [r3]
	adds r5, r0, #0
	movs r6, #4
	cmp r3, #0
	beq .L_0811a054
	movs r6, #3
.L_0811a054:
	bl Func_080ad0f0
	adds r7, r0, #0
	cmp r7, r6
	ble .L_0811a060
	adds r7, r6, #0
.L_0811a060:
	cmp r7, #0
	ble .L_0811a096
	ldr r3, .L_0811a0ac
	movs r1, #134
	lsls r1, r1, #2
	adds r2, r3, r1
	movs r3, #2
	mov r8, r3
	adds r6, r7, #0
.L_0811a072:
	ldrb r0, [r2]
	adds r2, #1
	cmp r5, #0
	beq .L_0811a07e
	strh r0, [r5]
	adds r5, #2
.L_0811a07e:
	str r2, [sp, #0]
	bl Owner_GetState
	movs r1, #149
	lsls r1, r1, #1
	adds r3, r0, r1
	subs r6, #1
	mov r1, r8
	strb r1, [r3]
	ldr r2, [sp, #0]
	cmp r6, #0
	bne .L_0811a072
.L_0811a096:
	cmp r5, #0
	beq .L_0811a09e
	ldr r3, .L_0811a0a8
	strh r3, [r5]
.L_0811a09e:
	adds r0, r7, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0811a0a8:
	.4byte 0x000000ff
.L_0811a0ac:
	.4byte gPartyState
