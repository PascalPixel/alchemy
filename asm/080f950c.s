@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080f950c
	.thumb_func
Func_080f950c:
	push	{r5, lr}
	adds	r2, r0, #0
	lsls	r2, r2, #16
	asrs	r5, r2, #16
	ldr	r0, [pc, #20]
	lsrs	r2, r2, #16
	movs	r1, #255
	bl	Func_080fb2cc
	ldr	r3, [pc, #16]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #16]
	strh	r5, [r3, #0]
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x02004290
	.4byte 0x02003034
	.4byte 0x02003008
