@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0809c3a4
	.thumb_func
Func_0809c3a4:
	push	{r5, r6, lr}
	ldr	r1, [pc, #92]
	movs	r0, #27
	bl	Func_080048f4
	movs	r2, #207
	lsls	r2, r2, #1
	adds	r3, r0, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L0
	bl	Func_0800430c
	bl	Func_0809c314
	bl	Func_08091660
	movs	r1, #6
	ldr	r0, [pc, #60]
	bl	Func_080936a0
	ldr	r1, [pc, #56]
	movs	r2, #128
	ldr	r3, [r1, #0]
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	adds	r6, r1, #0
	adds	r5, r2, #0
.L2:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r6, #0]
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L2
.L1:
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #6
	bl	Func_080936a0
	bl	Func_080043e0
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x00000ccc
	.4byte 0x00009d89
	.4byte 0x03001ae8
