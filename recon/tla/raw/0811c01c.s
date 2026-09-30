.syntax unified
	.thumb
	.global Func_0811c01c
	.thumb_func
Func_0811c01c:
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
	movs r1, #166
	lsls r1, r1, #9
	ldr r0, [r6, #12]
	ldr r3, .L_0811c070
	adds r1, #204
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r6, #16]
	adds r1, r0, #0
	movs r2, #0
	adds r0, r5, #0
	bl Object_SetPosition
	adds r0, r5, #0
	movs r1, #5
	bl Object_SetMode
	pop {r5, r6, pc}
.L_0811c070:
	.4byte IwramMulQ16
