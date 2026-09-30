.syntax unified
	.thumb
	.global Func_080e0308
	.thumb_func
Func_080e0308:
	push {lr}
	bl ObjectTable_Get
	cmp r0, #0
	beq .L_080e0352
	ldr r1, .L_080e0354
	movs r3, #156
	lsls r3, r3, #2
	adds r2, r1, r3
	ldr r3, [r0, #108]
	str r3, [r2]
	movs r3, #181
	lsls r3, r3, #1
	adds r3, #255
	adds r1, r1, r3
	movs r3, #0
	strb r3, [r1]
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_080e0340
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	cmp r3, #0
	beq .L_080e0340
	ldrb r3, [r3, #5]
	strb r3, [r1]
.L_080e0340:
	ldr r3, .L_080e0358
	adds r2, r0, #0
	str r3, [r0, #108]
	adds r2, #91
	movs r3, #1
	strb r3, [r2]
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildrenFar
.L_080e0352:
	pop {pc}
.L_080e0354:
	.4byte gPartyState
.L_080e0358:
	.4byte BattleEffect_SetRandomTableValueOnObject
