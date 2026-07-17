@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080cdd14
	.thumb_func
Func_080cdd14:
	push	{r5, lr}
	ldr	r3, [pc, #56]
	ldr	r2, [r3, #0]
	subs	r3, #140
	ldr	r5, [r3, #0]
	movs	r0, #128
	movs	r3, #1
	str	r3, [r2, #12]
	ldr	r1, [pc, #44]
	lsls	r0, r0, #19
	bl	Func_0800387c
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #201
	lsls	r3, r3, #3
	adds	r5, r5, r3
	ldrh	r1, [r5, #0]
	movs	r0, #2
	movs	r2, #0
	bl	Func_080b5038
	movs	r0, #1
	bl	Func_080030f8
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f00
	.4byte 0x00001541
