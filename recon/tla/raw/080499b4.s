.syntax unified
	.thumb
	.global Func_080499b4
	.thumb_func
Func_080499b4:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r7, r0, #0
	movs r0, #1
	cmp r6, #0
	beq .L_08049a00
	adds r0, r6, #0
	bl Item_Get
	adds r5, r0, #0
	ldrb r3, [r5, #12]
	movs r0, #1
	cmp r3, #3
	beq .L_08049a00
	ldrh r3, [r5, #40]
	cmp r3, #0
	beq .L_08049a00
	ldrb r3, [r5, #2]
	cmp r3, #0
	beq .L_080499ec
	adds r0, r7, #0
	adds r1, r6, #0
	bl Djinn_IsActiveFar + 0x10
	cmp r0, #0
	bne .L_080499ec
	movs r0, #1
	b .L_08049a00
.L_080499ec:
	ldrh r0, [r5, #40]
	bl BattleAction_Get
	ldrb r2, [r0, #1]
	movs r3, #128
	ands r3, r2
	movs r0, #2
	cmp r3, #0
	beq .L_08049a00
	movs r0, #0
.L_08049a00:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
