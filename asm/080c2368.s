@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080c2368
	.thumb_func
Func_080c2368:
	push	{lr}
	ldr	r3, [pc, #20]
	lsls	r0, r0, #3
	adds	r0, r0, r3
	ldrb	r3, [r0, #3]
	lsrs	r0, r3, #5
	cmp	r0, #4
	ble.n	.L0
	movs	r0, #1
	negs	r0, r0
.L0:
	pop	{r1}
	bx	r1
	.4byte 0x080c7420
