.syntax unified
	.thumb
	.global Func_08105370
	.thumb_func
Func_08105370:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r0, [r3]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #18
	adds r3, r0, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_081053a6
	movs r3, #176
	lsls r3, r3, #4
	adds r3, #204
	adds r5, r0, r3
	movs r6, #4
.L_08105396:
	adds r0, r5, #0
	movs r1, #240
	subs r6, #1
	bl Runtime_PushSlotEntry
	adds r5, #12
	cmp r6, #0
	bge .L_08105396
.L_081053a6:
	pop {r5, r6, pc}
