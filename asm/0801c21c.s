@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0801c21c
	.thumb_func
Func_0801c21c:
	push	{r5, lr}
	ldr	r3, [pc, #32]
	movs	r2, #195
	ldr	r3, [r3, #0]
	lsls	r2, r2, #2
	adds	r5, r3, r2
	ldrh	r3, [r5, #10]
	cmp	r3, #0
	beq.n	.L0
	ldrh	r0, [r5, #12]
	bl	Func_08003f3c
	movs	r3, #0
	strh	r3, [r5, #10]
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e98
