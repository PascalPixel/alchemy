.syntax unified
	.thumb
	.global Func_080fcf14
	.thumb_func
Func_080fcf14:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r0, #252
	lsls r0, r0, #6
	adds r0, #255
	ands r0, r3
	bl BattleAction_Get
	adds r5, r0, #0
	ldrb r0, [r5, #6]
	bl Func_080c8508
	cmp r0, #0
	beq .L_080fcf42
	movs r0, #0
	b .L_080fcf5a
.L_080fcf42:
	ldrb r3, [r5, #8]
	movs r0, #2
	cmp r3, #255
	beq .L_080fcf5a
	ldrb r3, [r5]
	movs r2, #2
	eors r3, r2
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
	movs r3, #1
	subs r0, r3, r0
.L_080fcf5a:
	pop {r5, pc}
