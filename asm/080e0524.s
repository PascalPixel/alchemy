@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_080e0524
	.thumb_func
Func_080e0524:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r1
	adds	r7, r2, #0
	adds	r5, r3, #0
	bl	Func_08002f40
	adds	r6, r0, #0
	cmp	r5, #0
	beq.n	.L0
	movs	r0, #160
	ldr	r3, [pc, #32]
	lsls	r0, r0, #19
	adds	r1, r6, #0
	movs	r2, #128
	bl	Func_080072f0
.L0:
	cmp	r7, #0
	beq.n	.L1
	adds	r6, #128
.L1:
	adds	r0, r6, #0
	mov	r1, r8
	bl	Func_08005340
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001388
