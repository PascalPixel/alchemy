@ Reconstructed thumb disassembly. This function reached byte-identity
@ only through inline-asm register pins, which are disallowed in C, so
@ it lives here until an asm-free C match is found. build_asm.py
@ re-verifies it against the private ROM.
.syntax unified
	.thumb
	.global Func_0809335c
	.thumb_func
Func_0809335c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r1
	bl	Func_0808ba1c
	ldr	r1, [pc, #96]
	adds	r6, r0, #0
	movs	r0, #27
	bl	Func_080048f4
	movs	r3, #240
	mov	r8, r0
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #80]
	ldr	r3, [r3, #0]
	cmp	r6, #0
	beq.n	.L0
	adds	r7, r5, #0
	adds	r7, #8
	str	r7, [r3, #0]
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	Func_080090e0
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L0
	ldr	r3, [r6, #8]
	str	r3, [r7, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #12]
	ldr	r3, [r6, #16]
	movs	r0, #1
	str	r3, [r5, #16]
	bl	Func_080030f8
	movs	r3, #207
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	beq.n	.L0
	bl	Func_08009128
.L0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000ccc
	.4byte 0x03001e70
