@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08090824
	.thumb_func
Func_08090824:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r1, #168
	mov	r8, r0
	lsls	r1, r1, #3
	movs	r0, #31
	sub	sp, #4
	bl	Func_080048f4
	movs	r6, #0
	adds	r5, r0, #0
	mov	r0, sp
	str	r6, [r0, #0]
	ldr	r3, [pc, #56]
	adds	r1, r5, #0
	ldr	r2, [pc, #56]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #0
	bl	Func_080907b0
	movs	r2, #165
	lsls	r2, r2, #3
	adds	r3, r5, r2
	mov	r2, r8
	strh	r2, [r3, #0]
	ldr	r3, [pc, #40]
	movs	r1, #200
	adds	r5, r5, r3
	lsls	r1, r1, #4
	strh	r6, [r5, #0]
	ldr	r0, [pc, #32]
	bl	Func_080041d8
	movs	r0, #120
	bl	Func_080030f8
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x040000d4
	.4byte 0x85000150
	.4byte 0x0000052a
	.4byte 0x08090659
