.syntax unified
	.thumb
	.global Func_080b04bc
	.thumb_func
Func_080b04bc:
	push {r5, r6, lr}
	movs r2, #42
	adds r5, r0, #0
	adds r2, #255
	adds r3, r5, r2
	ldrb r3, [r3]
	movs r0, #1
	cmp r3, #0
	beq .L_080b0512
	adds r0, r5, #0
	movs r1, #1
	bl Inventory_GetEquippedDefinition
	adds r6, r0, #0
	movs r0, #1
	cmp r6, #0
	beq .L_080b0512
	ldrh r3, [r6, #14]
	cmp r3, #0
	beq .L_080b0512
	adds r0, r5, #0
	bl Equipment_GetUnleashRateBonus
	ldrb r2, [r6, #11]
	movs r1, #100
	lsls r3, r2, #2
	adds r3, r3, r2
	adds r0, r0, r3
	lsls r0, r0, #16
	bl Math_Div
	adds r5, r0, #0
	bl Func_080b0378
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r0, r3
	cmp r5, r0
	ble .L_080b0510
	ldrh r0, [r6, #14]
	b .L_080b0512
.L_080b0510:
	movs r0, #1
.L_080b0512:
	pop {r5, r6, pc}
