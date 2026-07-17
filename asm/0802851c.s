@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0802851c
	.thumb_func
Func_0802851c:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #76]
	ldr	r5, [r3, #0]
	ldr	r0, [pc, #76]
	bl	Func_08004278
	ldr	r0, [r5, #120]
	cmp	r0, #0
	beq.n	.L0
	movs	r1, #2
	bl	Func_08016418
.L0:
	adds	r2, r5, #0
	adds	r2, #142
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	movs	r6, #0
	cmp	r6, r3
	bge.n	.L1
	adds	r7, r2, #0
	adds	r5, #18
.L2:
	ldrh	r0, [r5, #0]
	bl	Func_08003f3c
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	adds	r6, #1
	adds	r5, #20
	cmp	r6, r3
	blt.n	.L2
.L1:
	movs	r0, #58
	bl	Func_08002dd8
	movs	r0, #1
	bl	Func_080030f8
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f38
	.4byte 0x08028195
