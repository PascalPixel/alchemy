@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_08092560
	.thumb_func
Func_08092560:
	push	{r5, r6, lr}
	adds	r5, r1, #0
	adds	r6, r2, #0
	bl	Func_0808ba1c
	cmp	r0, #0
	beq.n	.L0
	adds	r1, r0, #0
	adds	r1, #85
	ldrb	r3, [r1, #0]
	movs	r2, #2
	orrs	r2, r3
	lsls	r3, r5, #16
	strb	r2, [r1, #0]
	str	r3, [r0, #40]
	cmp	r5, #5
	ble.n	.L1
	movs	r0, #153
	bl	Func_080f9010
	b.n	.L2
.L1:
	movs	r0, #152
	bl	Func_080f9010
.L2:
	adds	r0, r6, #0
	bl	Func_0809163c
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
