.syntax unified
	.thumb
	.global Func_0811bfd0
	.thumb_func
Func_0811bfd0:
	push {r5, r6, lr}
	bl GetBattleObjectSlot
	adds r6, r0, #0
	ldr r5, [r6]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #48]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #40]
	movs r3, #153
	lsls r3, r3, #8
	adds r3, #153
	adds r2, r5, #0
	str r3, [r5, #72]
	adds r2, #90
	movs r3, #0
	str r3, [r5, #68]
	adds r0, r5, #0
	strb r3, [r2]
	bl Object_ResetMotion
	ldr r3, [r6, #12]
	adds r0, r5, #0
	lsls r1, r3, #1
	adds r1, r1, r3
	lsrs r3, r1, #31
	adds r1, r1, r3
	asrs r1, r1, #1
	ldr r3, [r6, #16]
	movs r2, #0
	bl Object_SetPosition
	pop {r5, r6, pc}
