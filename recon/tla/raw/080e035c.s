.syntax unified
	.thumb
	.global Func_080e035c
	.thumb_func
Func_080e035c:
	push {r5, lr}
	bl ObjectTable_Get
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080e03a0
	ldr r2, [r5, #108]
	ldr r3, .L_080e03a4
	cmp r2, r3
	bne .L_080e0390
	ldr r2, .L_080e03a8
	movs r3, #156
	lsls r3, r3, #2
	adds r1, r2, r3
	ldr r3, [r1]
	str r3, [r5, #108]
	movs r3, #0
	str r3, [r1]
	movs r3, #181
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r2, r3
	movs r1, #0
	ldrsb r1, [r2, r1]
	bl Animation_ApplyChildValuesFar
.L_080e0390:
	adds r2, r5, #0
	adds r2, #91
	movs r3, #0
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
.L_080e03a0:
	pop {r5, pc}
	.2byte 0x0000
.L_080e03a4:
	.4byte BattleEffect_SetRandomTableValueOnObject
.L_080e03a8:
	.4byte gPartyState
