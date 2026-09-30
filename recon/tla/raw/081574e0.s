.syntax unified
	.thumb
	.global Func_081574e0
	.thumb_func
Func_081574e0:
	push {r5, r6, lr}
	bl GetBattleObjectSlotFar
	adds r6, r0, #0
	ldr r5, [r6]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #40]
	movs r3, #171
	lsls r3, r3, #8
	adds r3, #133
	adds r2, r5, #0
	str r3, [r5, #72]
	adds r2, #90
	movs r3, #0
	str r3, [r5, #68]
	strb r3, [r2]
	subs r2, #2
	movs r3, #1
	strb r3, [r2]
	adds r0, r5, #0
	bl Object_ResetMotion
	ldr r1, [r6, #12]
	adds r0, r5, #0
	ldr r3, [r6, #16]
	movs r2, #0
	bl Object_SetPosition
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetMode
	pop {r5, r6, pc}
