.syntax unified
	.thumb
	.global Func_080eb544
	.thumb_func
Func_080eb544:
	push {r5, lr}
	cmp r1, #0
	ble .L_080eb566
	ldr r5, .L_080eb568
	movs r4, #31
.L_080eb54e:
	ldrb r2, [r0]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080eb55e
	adds r3, r4, #0
	ands r3, r2
	ldrb r3, [r5, r3]
	strb r3, [r0]
.L_080eb55e:
	subs r1, #1
	adds r0, #1
	cmp r1, #0
	bne .L_080eb54e
.L_080eb566:
	pop {r5, pc}
.L_080eb568:
	.4byte Data_080f3bfc
