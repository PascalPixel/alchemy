.syntax unified
	.thumb
	.global Func_08124cc4
	.thumb_func
Func_08124cc4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r2, #0
	mov r9, r2
	bl Trade_GetOfferStateFar
	movs r2, #8
	adds r3, r0, #0
	adds r2, r2, r3
	mov r8, r2
	movs r2, #0
	mov r10, r2
	movs r2, #148
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r3, [r3]
	cmp r9, r3
	bge .L_08124d2c
	mov r5, r8
.L_08124cf0:
	movs r3, #3
	ldrsb r3, [r5, r3]
	cmp r3, #0
	ble .L_08124d1a
	ldrb r0, [r5, #2]
	bl BattleParty_IsUnitListed
	cmp r0, #0
	beq .L_08124d1a
	ldrb r0, [r5, #2]
	bl Owner_GetState
	movs r2, #56
	ldrsh r3, [r0, r2]
	cmp r3, #0
	beq .L_08124d1a
	ldrb r3, [r5, #3]
	subs r3, #1
	strb r3, [r5, #3]
	movs r3, #1
	mov r9, r3
.L_08124d1a:
	movs r3, #144
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	movs r2, #1
	add r10, r2
	adds r5, #4
	cmp r10, r3
	blt .L_08124cf0
.L_08124d2c:
	movs r3, #0
	mov r10, r3
	movs r3, #144
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	cmp r10, r3
	bge .L_08124e10
	mov r6, r8
.L_08124d3e:
	movs r3, #3
	ldrsb r3, [r6, r3]
	cmp r3, #0
	bne .L_08124dfe
	ldrb r5, [r6, #2]
	adds r0, r5, #0
	bl BattleParty_IsUnitListed
	cmp r0, #0
	beq .L_08124e04
	movs r2, #2
	mov r9, r2
	bl Func_081234a4
	movs r0, #30
	bl Func_08122c88
	movs r0, #0
	adds r1, r5, #0
	bl BattleEv_Push
	ldrb r3, [r6]
	movs r0, #3
	lsls r1, r3, #2
	adds r1, r1, r3
	ldrb r3, [r6, #1]
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r3, #150
	lsls r3, r3, #1
	adds r1, r1, r3
	bl BattleEv_Push
	movs r0, #14
	movs r1, #175
	bl BattleEv_Push
	movs r0, #10
	movs r1, #0
	bl BattleEv_Push
	movs r0, #4
	ldr r1, .L_08124e1c
	bl BattleEv_Push
	movs r0, #11
	adds r1, r5, #0
	bl BattleEv_Push
	adds r0, r5, #0
	bl BattleParty_IsUnitListed
	cmp r0, #0
	beq .L_08124dc6
	adds r0, r5, #0
	bl GetBattleObjectSlot
	movs r1, #3
	ldr r0, [r0]
	bl Object_SetMode
	adds r0, r5, #0
	bl GetBattleObjectSlot
	movs r1, #32
	ldr r0, [r0]
	bl ObjectDispatch_ApplyValueToChildrenFar
.L_08124dc6:
	ldrb r7, [r6]
	ldrb r2, [r6, #1]
	adds r1, r7, #0
	adds r0, r5, #0
	bl Djinn_ActivateFar
	ldrb r1, [r6]
	ldrb r2, [r6, #1]
	adds r0, r5, #0
	bl Trade_RemoveOfferFar
	adds r0, r5, #0
	bl Owner_RecalculateStatsFar
	adds r0, r5, #0
	bl BattleParty_IsUnitListed
	cmp r0, #0
	beq .L_08124df8
	adds r0, r5, #0
	adds r1, r7, #0
	movs r2, #3
	movs r3, #0
	bl Func_08127308
.L_08124df8:
	bl Func_081234f0
	b .L_08124e04
.L_08124dfe:
	movs r2, #1
	adds r6, #4
	add r10, r2
.L_08124e04:
	movs r3, #144
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	cmp r10, r3
	blt .L_08124d3e
.L_08124e10:
	mov r0, r9
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_08124e1c:
	.4byte 0x00000cf7
