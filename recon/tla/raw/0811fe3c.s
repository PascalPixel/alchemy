.syntax unified
	.thumb
	.global Func_0811fe3c
	.thumb_func
Func_0811fe3c:
	push {r5, lr}
	adds r5, r0, #0
	bl Owner_GetState
	movs r4, #48
	adds r1, r0, #0
	adds r4, #255
	movs r0, #0
	movs r2, #3
	adds r3, r1, r4
.L_0811fe50:
	subs r2, #1
	strb r0, [r3]
	subs r3, #1
	cmp r2, #0
	bge .L_0811fe50
	movs r0, #50
	adds r0, #255
	movs r4, #153
	movs r3, #0
	adds r2, r1, r0
	lsls r4, r4, #1
	strb r3, [r2]
	adds r0, #2
	adds r2, r1, r4
	strb r3, [r2]
	adds r4, #2
	adds r2, r1, r0
	strb r3, [r2]
	adds r0, #2
	adds r2, r1, r4
	strb r3, [r2]
	adds r4, #2
	adds r2, r1, r0
	strb r3, [r2]
	adds r0, #2
	adds r2, r1, r4
	strb r3, [r2]
	adds r4, #2
	adds r2, r1, r0
	strb r3, [r2]
	adds r0, #2
	adds r2, r1, r4
	strb r3, [r2]
	adds r4, #2
	adds r2, r1, r0
	strb r3, [r2]
	adds r0, #2
	adds r2, r1, r4
	strb r3, [r2]
	adds r4, #2
	adds r2, r1, r0
	strb r3, [r2]
	adds r0, #2
	adds r2, r1, r4
	strb r3, [r2]
	adds r4, #2
	adds r2, r1, r0
	strb r3, [r2]
	adds r0, #2
	adds r2, r1, r4
	strb r3, [r2]
	adds r4, #2
	adds r2, r1, r0
	strb r3, [r2]
	adds r0, #2
	adds r2, r1, r4
	strb r3, [r2]
	adds r4, #2
	adds r2, r1, r0
	strb r3, [r2]
	adds r0, #2
	adds r2, r1, r4
	strb r3, [r2]
	adds r4, #2
	adds r2, r1, r0
	strb r3, [r2]
	adds r0, #2
	adds r2, r1, r4
	strb r3, [r2]
	adds r4, #2
	adds r2, r1, r0
	strb r3, [r2]
	adds r0, #2
	adds r2, r1, r4
	strb r3, [r2]
	adds r4, #2
	adds r2, r1, r0
	strb r3, [r2]
	adds r2, r1, r4
	strb r3, [r2]
	adds r0, r5, #0
	bl Owner_RecalculateStatsFar
	adds r0, r5, #0
	bl GetBattleObjectSlot
	adds r1, r0, #0
	adds r0, r5, #0
	bl Func_0811b4d8
	pop {r5, pc}
	.2byte 0x0000
