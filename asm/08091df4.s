@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_08091df4
	.thumb_func
Func_08091df4:
	push	{r5, lr}
	ldr	r3, [pc, #36]
	movs	r2, #224
	ldr	r5, [r3, #0]
	lsls	r2, r2, #1
	adds	r3, r5, r2
	adds	r2, #8
	ldr	r0, [r3, #0]
	adds	r3, r5, r2
	ldr	r1, [r3, #0]
	bl	Func_080901c0
	movs	r3, #227
	lsls	r3, r3, #1
	adds	r2, r5, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
