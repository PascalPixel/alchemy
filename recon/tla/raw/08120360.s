.syntax unified
	.thumb
	.global BattleEv_Push
	.thumb_func
BattleEv_Push:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #36]
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #228
	adds r5, r4, r3
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #40
	adds r4, r4, r3
	ldr r2, [r4]
	lsls r3, r2, #2
	strb r0, [r5, r2]
	adds r3, #64
	adds r2, #1
	str r1, [r5, r3]
	str r2, [r4]
	pop {r5, pc}
