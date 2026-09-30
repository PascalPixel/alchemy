.syntax unified
	.thumb
	.global Func_080dc0d8
	.thumb_func
Func_080dc0d8:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	movs r0, #0
	cmp r5, #0
	beq .L_080dc108
	cmp r6, #0
	bne .L_080dc0f2
	ldrb r3, [r5, #17]
	movs r2, #1
	orrs r3, r2
	strb r3, [r5, #17]
	b .L_080dc106
.L_080dc0f2:
	ldrb r0, [r5, #16]
	bl Func_08014274
	ldrb r3, [r6, #16]
	movs r2, #1
	strb r3, [r5, #16]
	ldrb r3, [r5, #17]
	orrs r3, r2
	strb r3, [r5, #17]
	adds r5, r6, #0
.L_080dc106:
	adds r0, r5, #0
.L_080dc108:
	pop {r5, r6, pc}
	.2byte 0x0000
