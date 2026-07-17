@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_08079700
	.thumb_func
Func_08079700:
	push	{lr}
	ldr	r1, [pc, #28]
	ldr	r3, [r1, #16]
	ldr	r2, [pc, #28]
	adds	r3, r3, r0
	cmp	r3, r2
	ble.n	.L0
	adds	r3, r2, #0
.L0:
	cmp	r3, #0
	bge.n	.L1
	movs	r3, #0
.L1:
	str	r3, [r1, #16]
	adds	r0, r3, #0
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x000f423f
