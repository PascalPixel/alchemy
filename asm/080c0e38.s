@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080c0e38
	.thumb_func
Func_080c0e38:
	push	{r5, r6, r7, lr}
	ldr	r2, [pc, #20]
	ldr	r3, [pc, #8]
	ldr	r7, [pc, #20]
	strh	r3, [r2, #0]
	ldr	r6, [pc, #8]
	movs	r5, #1
	b.n	.L0
	.4byte 0x00002044
	.4byte 0x00001010
	.4byte 0x04000050
	.4byte 0x04000052
.L0:
	subs	r3, r6, r5
	strh	r3, [r7, #0]
	movs	r0, #1
	adds	r5, #2
	bl	Func_080030f8
	cmp	r5, #16
	ble.n	.L0
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
