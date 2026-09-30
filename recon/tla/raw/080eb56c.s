.syntax unified
	.thumb
	.global Func_080eb56c
	.thumb_func
Func_080eb56c:
	push {r5, lr}
	cmp r1, #0
	ble .L_080eb58e
	ldr r5, .L_080eb590
	movs r4, #31
.L_080eb576:
	ldrb r2, [r0]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080eb586
	adds r3, r4, #0
	ands r3, r2
	ldrb r3, [r5, r3]
	strb r3, [r0]
.L_080eb586:
	subs r1, #1
	adds r0, #1
	cmp r1, #0
	bne .L_080eb576
.L_080eb58e:
	pop {r5, pc}
.L_080eb590:
	.4byte Data_080f3c1c
