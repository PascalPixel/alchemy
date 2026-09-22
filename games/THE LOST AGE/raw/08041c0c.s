.syntax unified
	.thumb
	.global Overlay_08041c0c
Overlay_08041c0c:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #60]
	cmp	r0, #0
	beq.n	.L_08041c34
	movs	r4, #224
	lsls	r4, r4, #4
	movs	r3, #1
	adds	r4, #184
	strb	r3, [r1, #2]
	movs	r2, #128
	movs	r0, #0
	adds	r3, r1, r4
.L_08041c28:
	adds	r2, #1
	strb	r0, [r3, #0]
	adds	r3, #1
	cmp	r2, #255
	ble.n	.L_08041c28
	b.n	.L_08041c4e
.L_08041c34:
	movs	r2, #224
	lsls	r2, r2, #4
	movs	r3, #0
	adds	r2, #184
	strb	r3, [r1, #2]
	movs	r0, #0
	adds	r3, r1, r2
	movs	r2, #127
.L_08041c44:
	subs	r2, #1
	strb	r0, [r3, #0]
	adds	r3, #1
	cmp	r2, #0
	bge.n	.L_08041c44
.L_08041c4e:
	pop	{pc}
